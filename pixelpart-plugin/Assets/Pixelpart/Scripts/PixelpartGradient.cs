using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartGradient {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ParticleType,
		ForceField,
		Collider
	}

	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartCurve4GetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartCurve4SetInterpolation(nativeCurve, (int)value);
			UpdateSimulation();
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartCurve4GetNumPoints(nativeCurve);
		}
	}

	public int CacheSize {
		get {
			return Plugin.PixelpartCurve4GetCacheSize(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;

	public PixelpartGradient(IntPtr nativeCurvePtr, IntPtr nativeEffectPr, ObjectType type) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPr;
		objectType = type;
	}

	public Color Get(float t) {
		return new Color(
			Plugin.PixelpartCurve4GetX(nativeCurve, t),
			Plugin.PixelpartCurve4GetY(nativeCurve, t),
			Plugin.PixelpartCurve4GetZ(nativeCurve, t),
			Plugin.PixelpartCurve4GetW(nativeCurve, t));
	}
	public Color GetPoint(int index) {
		return new Color(
			Plugin.PixelpartCurve4GetPointX(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointY(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointZ(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointW(nativeCurve, index));
	}

	public void Set(Color value) {
		Plugin.PixelpartCurve4Set(nativeCurve, value.r, value.g, value.b, value.a);
		UpdateSimulation();
	}
	public void AddPoint(float t, Color value) {
		Plugin.PixelpartCurve4AddPoint(nativeCurve, t, value.r, value.g, value.b, value.a);
		UpdateSimulation();
	}
	public void SetPoint(int index, Color value) {
		Plugin.PixelpartCurve4SetPoint(nativeCurve, index, value.r, value.g, value.b, value.a);
		UpdateSimulation();
	}
	public void MovePoint(int index, Color delta) {
		Plugin.PixelpartCurve4MovePoint(nativeCurve, index, delta.r, delta.g, delta.b, delta.a);
		UpdateSimulation();
	}
	public void ShiftPoint(int index, float delta) {
		Plugin.PixelpartCurve4ShiftPoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartCurve4RemovePoint(nativeCurve, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartCurve4Clear(nativeCurve);
		UpdateSimulation();
	}

	public void Move(Color delta) {
		Plugin.PixelpartCurve4Move(nativeCurve, delta.r, delta.g, delta.b, delta.a);
		UpdateSimulation();
	}
	public void Shift(float delta) {
		Plugin.PixelpartCurve4Shift(nativeCurve, delta);
		UpdateSimulation();
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartCurve4EnableAdaptiveCache(nativeCurve);
		UpdateSimulation();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartCurve4EnableFixedCache(nativeCurve, size);
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