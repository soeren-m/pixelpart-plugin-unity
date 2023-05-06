using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartCurve3 {
	public enum ObjectType {
		None,
		ParticleEmitter,
		ParticleType,
		ForceField,
		Collider
	}

	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartCurve3GetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartCurve3SetInterpolation(nativeCurve, (int)value);
			UpdateSimulation();
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartCurve3GetNumPoints(nativeCurve);
		}
	}

	public int CacheSize {
		get {
			return Plugin.PixelpartCurve3GetCacheSize(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;
	private ObjectType objectType = ObjectType.None;

	public PixelpartCurve3(IntPtr nativeCurvePtr, IntPtr nativeEffectPr, ObjectType type) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPr;
		objectType = type;
	}

	public Vector3 Get(float t) {
		return new Vector3(
			Plugin.PixelpartCurve3GetX(nativeCurve, t),
			Plugin.PixelpartCurve3GetY(nativeCurve, t),
			Plugin.PixelpartCurve3GetZ(nativeCurve, t));
	}
	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartCurve3GetPointX(nativeCurve, index),
			Plugin.PixelpartCurve3GetPointY(nativeCurve, index),
			Plugin.PixelpartCurve3GetPointZ(nativeCurve, index));
	}

	public void Set(Vector3 value) {
		Plugin.PixelpartCurve3Set(nativeCurve, value.x, value.y, value.z);
		UpdateSimulation();
	}
	public void AddPoint(float t, Vector3 value) {
		Plugin.PixelpartCurve3AddPoint(nativeCurve, t, value.x, value.y, value.z);
		UpdateSimulation();
	}
	public void SetPoint(int index, Vector3 value) {
		Plugin.PixelpartCurve3SetPoint(nativeCurve, index, value.x, value.y, value.z);
		UpdateSimulation();
	}
	public void MovePoint(int index, Vector3 delta) {
		Plugin.PixelpartCurve3MovePoint(nativeCurve, index, delta.x, delta.y, delta.z);
		UpdateSimulation();
	}
	public void ShiftPoint(int index, float delta) {
		Plugin.PixelpartCurve3ShiftPoint(nativeCurve, index, delta);
		UpdateSimulation();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartCurve3RemovePoint(nativeCurve, index);
		UpdateSimulation();
	}
	public void Clear() {
		Plugin.PixelpartCurve3Clear(nativeCurve);
		UpdateSimulation();
	}

	public void Move(Vector3 delta) {
		Plugin.PixelpartCurve3Move(nativeCurve, delta.x, delta.y, delta.z);
		UpdateSimulation();
	}
	public void Shift(float delta) {
		Plugin.PixelpartCurve3Shift(nativeCurve, delta);
		UpdateSimulation();
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartCurve3EnableAdaptiveCache(nativeCurve);
		UpdateSimulation();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartCurve3EnableFixedCache(nativeCurve, size);
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