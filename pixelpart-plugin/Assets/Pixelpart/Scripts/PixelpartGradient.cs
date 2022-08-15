using System;
using System.Text;
using UnityEngine;

namespace pixelpart {
public class PixelpartGradient {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ForceField,
		Collider,
		Sprite
	}

	private IntPtr nativeGradient = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;

	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartGradientGetInterpolation(nativeGradient);
		}
		set {
			Plugin.PixelpartGradientSetInterpolation(nativeGradient, (int)value);
			UpdateSimulation();
		}
	}

	public PixelpartGradient(IntPtr nativePtr, IntPtr nativeEff, ObjectType type) {
		nativeGradient = nativePtr;
		nativeEffect = nativeEff;
		objectType = type;
	}

	public Color Get(float t) {
		return new Color(
			Plugin.PixelpartGradientGetR(nativeGradient, t),
			Plugin.PixelpartGradientGetG(nativeGradient, t),
			Plugin.PixelpartGradientGetB(nativeGradient, t),
			Plugin.PixelpartGradientGetA(nativeGradient, t));
	}
	public Color GetPoint(uint index) {
		return new Color(
			Plugin.PixelpartGradientGetPointR(nativeGradient, index),
			Plugin.PixelpartGradientGetPointG(nativeGradient, index),
			Plugin.PixelpartGradientGetPointB(nativeGradient, index),
			Plugin.PixelpartGradientGetPointA(nativeGradient, index));
	}

	public void Set(Color value) {
		Plugin.PixelpartGradientSet(nativeGradient, value.r, value.g, value.b, value.a);
		UpdateSimulation();
	}
	public void AddPoint(float t, Color value) {
		Plugin.PixelpartGradientAddPoint(nativeGradient, t, value.r, value.g, value.b, value.a);
		UpdateSimulation();

	}
	public void SetPoint(uint index, Color value) {
		Plugin.PixelpartGradientSetPoint(nativeGradient, index, value.r, value.g, value.b, value.a);
		UpdateSimulation();
	}
	public void MovePoint(uint index, Color delta) {
		Plugin.PixelpartGradientMovePoint(nativeGradient, index, delta.r, delta.g, delta.b, delta.a);
		UpdateSimulation();
	}
	public void ShiftPoint(uint index, float delta) {
		Plugin.PixelpartGradientShiftPoint(nativeGradient, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(uint index) {
		Plugin.PixelpartGradientRemovePoint(nativeGradient, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartGradientClear(nativeGradient);
		UpdateSimulation();
	}
	public uint GetNumPoints() {
		return Plugin.PixelpartGradientGetNumPoints(nativeGradient);
	}

	public void Move(Color delta) {
		Plugin.PixelpartGradientMove(nativeGradient, delta.r, delta.g, delta.b, delta.a);
		UpdateSimulation();
	}
	public void Shift(float delta) {
		Plugin.PixelpartGradientShift(nativeGradient, delta);
		UpdateSimulation();
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartGradientEnableAdaptiveCache(nativeGradient);
		UpdateSimulation();
	}
	public void EnableFixedCache(uint size) {
		Plugin.PixelpartGradientEnableFixedCache(nativeGradient, size);
		UpdateSimulation();
	}
	public uint GetCacheSize() {
		return Plugin.PixelpartGradientGetCacheSize(nativeGradient);
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