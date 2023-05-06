using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartCurve {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ParticleType,
		ForceField,
		Collider
	}

	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartCurveGetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartCurveSetInterpolation(nativeCurve, (int)value);
			UpdateSimulation();
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartCurveGetNumPoints(nativeCurve);
		}
	}

	public int CacheSize {
		get {
			return Plugin.PixelpartCurveGetCacheSize(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;

	public PixelpartCurve(IntPtr nativeCurvePtr, IntPtr nativeEffectPr, ObjectType type) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPr;
		objectType = type;
	}

	public float Get(float t) {
		return Plugin.PixelpartCurveGet(nativeCurve, t);
	}
	public float GetPoint(int index) {
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
	public void SetPoint(int index, float value) {
		Plugin.PixelpartCurveSetPoint(nativeCurve, index, value);
		UpdateSimulation();
	}
	public void MovePoint(int index, float delta) {
		Plugin.PixelpartCurveMovePoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void ShiftPoint(int index, float delta) {
		Plugin.PixelpartCurveShiftPoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartCurveRemovePoint(nativeCurve, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartCurveClear(nativeCurve);
		UpdateSimulation();
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
	public void EnableFixedCache(int size) {
		Plugin.PixelpartCurveEnableFixedCache(nativeCurve, size);
		UpdateSimulation();
	}

	private void UpdateSimulation() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		switch(objectType) {
			case ObjectType.ForceField:
				Plugin.PixelpartUpdateForceSolver(nativeEffect);
				break;
			case ObjectType.Collider:
				Plugin.PixelpartUpdateCollisionSolver(nativeEffect);
				break;
		}
	}
}
}