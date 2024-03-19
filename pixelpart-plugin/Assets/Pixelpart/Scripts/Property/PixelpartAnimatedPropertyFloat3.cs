using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat3 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat3GetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat3SetInterpolation(nativeCurve, (int)value);
			RefreshSolver();
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat3ContainsPoints(nativeProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat3GetNumPoints(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartAnimatedPropertyFloat3(IntPtr nativeCurvePtr, IntPtr nativeEffectPtr) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector3 Get(float position) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetX(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetY(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetZ(nativeCurve, position));
	}

	public void AddPoint(float position, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3AddPoint(nativeCurve, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat3RemovePoint(nativeCurve, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPoint(nativeCurve, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPointPosition(nativeCurve, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat3Clear(nativeCurve);
		RefreshSolver();
	}

	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetPointX(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointY(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointZ(nativeCurve, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(nativeCurve);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat3EnableFixedCache(nativeCurve, size);
		RefreshSolver();
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshSolver(nativeEffect);
	}
}
}