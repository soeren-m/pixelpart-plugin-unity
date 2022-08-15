using System;
using System.Text;
using UnityEngine;

namespace pixelpart {
public class PixelpartPath {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ForceField,
		Collider,
		Sprite
	}

	private IntPtr nativePath = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;

	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartPathGetInterpolation(nativePath);
		}
		set {
			Plugin.PixelpartPathSetInterpolation(nativePath, (int)value);
			UpdateSimulation();
		}
	}

	public PixelpartPath(IntPtr nativePtr, IntPtr nativeEff, ObjectType type) {
		nativePath = nativePtr;
		nativeEffect = nativeEff;
		objectType = type;
	}

	public Vector2 Get(float t) {
		return new Vector2(
			Plugin.PixelpartPathGetX(nativePath, t),
			Plugin.PixelpartPathGetY(nativePath, t));
	}
	public Vector2 GetPoint(uint index) {
		return new Vector2(
			Plugin.PixelpartPathGetPointX(nativePath, index),
			Plugin.PixelpartPathGetPointY(nativePath, index));
	}

	public void Set(Vector2 value) {
		Plugin.PixelpartPathSet(nativePath, value.x, value.y);
		UpdateSimulation();
	}
	public void AddPoint(float t, Vector2 value) {
		Plugin.PixelpartPathAddPoint(nativePath, t, value.x, value.y);
		UpdateSimulation();
	}
	public void SetPoint(uint index, Vector2 value) {
		Plugin.PixelpartPathSetPoint(nativePath, index, value.x, value.y);
		UpdateSimulation();
	}
	public void MovePoint(uint index, Vector2 delta) {
		Plugin.PixelpartPathMovePoint(nativePath, index, delta.x, delta.y);
		UpdateSimulation();
	}
	public void ShiftPoint(uint index, float delta) {
		Plugin.PixelpartPathShiftPoint(nativePath, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(uint index) {
		Plugin.PixelpartPathRemovePoint(nativePath, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartPathClear(nativePath);
		UpdateSimulation();
	}
	public uint GetNumPoints() {
		return Plugin.PixelpartPathGetNumPoints(nativePath);
	}

	public void Move(Vector2 delta) {
		Plugin.PixelpartPathMove(nativePath, delta.x, delta.y);
		UpdateSimulation();
	}
	public void Shift(float delta) {
		Plugin.PixelpartPathShift(nativePath, delta);
		UpdateSimulation();
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartPathEnableAdaptiveCache(nativePath);
		UpdateSimulation();
	}
	public void EnableFixedCache(uint size) {
		Plugin.PixelpartPathEnableFixedCache(nativePath, size);
		UpdateSimulation();
	}
	public uint GetCacheSize() {
		return Plugin.PixelpartPathGetCacheSize(nativePath);
	}

	private void UpdateSimulation() {
		if(nativeEffect != IntPtr.Zero) {
			if(objectType == ObjectType.ForceField) {
				Plugin.PixelpartUpdateEffectForceSolver(nativeEffect);
			}
			else if(objectType == ObjectType.Collider) {
				Plugin.PixelpartUpdateEffectCollisionSolver(nativeEffect);
			}
		}
	}
}
}