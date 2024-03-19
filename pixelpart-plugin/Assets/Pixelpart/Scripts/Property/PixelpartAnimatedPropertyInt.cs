using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyInt {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyIntGetInterpolation(nativeProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyIntSetInterpolation(nativeProperty, (int)value);
			RefreshSolver();
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyIntContainsPoints(nativeProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyIntGetNumPoints(nativeProperty);
		}
	}

	private IntPtr nativeProperty = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartAnimatedPropertyInt(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public int Get(float position) {
		return Plugin.PixelpartAnimatedPropertyIntGet(nativeProperty, position);
	}

	public void AddPoint(float position, int value) {
		Plugin.PixelpartAnimatedPropertyIntAddPoint(nativeProperty, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyIntRemovePoint(nativeProperty, index);
		RefreshSolver();
	}
	public void SetPoint(int index, int value) {
		Plugin.PixelpartAnimatedPropertyIntSetPoint(nativeProperty, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyIntSetPointPosition(nativeProperty, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyIntClear(nativeProperty);
		RefreshSolver();
	}

	public int GetPoint(int index) {
		return Plugin.PixelpartAnimatedPropertyIntGetPoint(nativeProperty, index);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyIntEnableAdaptiveCache(nativeProperty);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyIntEnableFixedCache(nativeProperty, size);
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