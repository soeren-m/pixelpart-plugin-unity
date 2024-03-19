using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat2 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat2GetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat2SetInterpolation(nativeCurve, (int)value);
			RefreshSolver();
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat2ContainsPoints(nativeProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat2GetNumPoints(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartAnimatedPropertyFloat2(IntPtr nativeCurvePtr, IntPtr nativeEffectPtr) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector2 Get(float position) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetX(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat2GetY(nativeCurve, position));
	}

	public void AddPoint(float position, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2AddPoint(nativeCurve, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat2RemovePoint(nativeCurve, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPoint(nativeCurve, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPointPosition(nativeCurve, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat2Clear(nativeCurve);
		RefreshSolver();
	}

	public Vector2 GetPoint(int index) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetPointX(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat2GetPointY(nativeCurve, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(nativeCurve);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat2EnableFixedCache(nativeCurve, size);
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