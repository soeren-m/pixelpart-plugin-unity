using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat4 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat4GetInterpolation(nativeCurve);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat4SetInterpolation(nativeCurve, (int)value);
			RefreshSolver();
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat4ContainsPoints(nativeProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat4GetNumPoints(nativeCurve);
		}
	}

	private IntPtr nativeCurve = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartAnimatedPropertyFloat4(IntPtr nativeCurvePtr, IntPtr nativeEffectPtr) {
		nativeCurve = nativeCurvePtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector4 Get(float position) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetX(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetY(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetZ(nativeCurve, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetW(nativeCurve, position));
	}

	public void AddPoint(float position, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4AddPoint(nativeCurve, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat4RemovePoint(nativeCurve, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPoint(nativeCurve, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPointPosition(nativeCurve, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat4Clear(nativeCurve);
		RefreshSolver();
	}

	public Vector4 GetPoint(int index) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetPointX(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointY(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointZ(nativeCurve, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointW(nativeCurve, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(nativeCurve);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat4EnableFixedCache(nativeCurve, size);
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