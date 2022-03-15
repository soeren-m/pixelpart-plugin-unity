using System;
using System.Text;
using UnityEngine;

namespace pixelpart {
public class PixelpartCurve {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ForceField,
		Collider,
		Sprite
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;
	
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartCurveGetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartCurveSetInterpolation(nativeCurve, (int)value);
			UpdateSimulation();
		}
	}

	public PixelpartCurve(IntPtr nativePtr, IntPtr nativeEff, ObjectType type) {
		nativeCurve = nativePtr;
		nativeEffect = nativeEff;
		objectType = type;
	}

	public float Get(float t) {
		return Plugin.PixelpartCurveGet(nativeCurve, t);
	}
	public float GetPoint(uint index) {
		return Plugin.PixelpartCurveGetPoint(nativeCurve, index);
	}

	public void Set(float value) {
		Plugin.PixelpartCurveSet(nativeCurve, value);
		UpdateSimulation();
	}
	public void AddPoint(float t, float value) {
		Plugin.PixelpartCurveAddPoint(nativeCurve, t, value);
		UpdateSimulation();
	}
	public void SetPoint(uint index, float value) {
		Plugin.PixelpartCurveSetPoint(nativeCurve, index, value);
		UpdateSimulation();
	}
	public void MovePoint(uint index, float delta) {
		Plugin.PixelpartCurveMovePoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void ShiftPoint(uint index, float delta) {
		Plugin.PixelpartCurveShiftPoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(uint index) {
		Plugin.PixelpartCurveRemovePoint(nativeCurve, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartCurveClear(nativeCurve);
		UpdateSimulation();
	}
	public uint GetNumPoints() {
		return Plugin.PixelpartCurveGetNumPoints(nativeCurve);
	}

	public void Move(float delta) {
		Plugin.PixelpartCurveMove(nativeCurve, delta);
		UpdateSimulation();
	}
	public void Shift(float delta) {
		Plugin.PixelpartCurveShift(nativeCurve, delta);
		UpdateSimulation();
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartCurveEnableAdaptiveCache(nativeCurve);
		UpdateSimulation();
	}
	public void EnableFixedCache(uint size) {
		Plugin.PixelpartCurveEnableFixedCache(nativeCurve, size);
		UpdateSimulation();
	}
	public uint GetCacheSize() {
		return Plugin.PixelpartCurveGetCacheSize(nativeCurve);
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