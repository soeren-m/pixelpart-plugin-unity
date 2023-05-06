using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartCurve4 {
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

	public PixelpartCurve4(IntPtr nativeCurvePtr, IntPtr nativeEffectPr, ObjectType type) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPr;
		objectType = type;
	}

	public Vector4 Get(float t) {
		return new Vector4(
			Plugin.PixelpartCurve4GetX(nativeCurve, t),
			Plugin.PixelpartCurve4GetY(nativeCurve, t),
			Plugin.PixelpartCurve4GetZ(nativeCurve, t),
			Plugin.PixelpartCurve4GetW(nativeCurve, t));
	}
	public Vector4 GetPoint(int index) {
		return new Vector4(
			Plugin.PixelpartCurve4GetPointX(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointY(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointZ(nativeCurve, index),
			Plugin.PixelpartCurve4GetPointW(nativeCurve, index));
	}

	public void Set(Vector4 value) {
		Plugin.PixelpartCurve4Set(nativeCurve, value.x, value.y, value.z, value.w);
		UpdateSimulation();
	}
	public void AddPoint(float t, Vector4 value) {
		Plugin.PixelpartCurve4AddPoint(nativeCurve, t, value.x, value.y, value.z, value.w);
		UpdateSimulation();
	}
	public void SetPoint(int index, Vector4 value) {
		Plugin.PixelpartCurve4SetPoint(nativeCurve, index, value.x, value.y, value.z, value.w);
		UpdateSimulation();
	}
	public void MovePoint(int index, Vector4 delta) {
		Plugin.PixelpartCurve4MovePoint(nativeCurve, index, delta.x, delta.y, delta.z, delta.w);
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

	public void Move(Vector4 delta) {
		Plugin.PixelpartCurve4Move(nativeCurve, delta.x, delta.y, delta.z, delta.w);
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