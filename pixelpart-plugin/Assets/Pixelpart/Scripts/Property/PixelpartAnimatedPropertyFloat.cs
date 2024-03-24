using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloatGetInterpolation(nativeProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloatSetInterpolation(nativeProperty, (int)value);
			RefreshSolver();
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloatContainsPoints(nativeProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloatGetNumPoints(nativeProperty);
		}
	}

	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartAnimatedPropertyFloat(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public float Get(float position) {
		return Plugin.PixelpartAnimatedPropertyFloatGet(nativeProperty, position);
	}

	public void AddPoint(float position, float value) {
		Plugin.PixelpartAnimatedPropertyFloatAddPoint(nativeProperty, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloatRemovePoint(nativeProperty, index);
		RefreshSolver();
	}
	public void SetPoint(int index, float value) {
		Plugin.PixelpartAnimatedPropertyFloatSetPoint(nativeProperty, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloatSetPointPosition(nativeProperty, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloatClear(nativeProperty);
		RefreshSolver();
	}

	public float GetPoint(int index) {
		return Plugin.PixelpartAnimatedPropertyFloatGetPoint(nativeProperty, index);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloatEnableAdaptiveCache(nativeProperty);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloatEnableFixedCache(nativeProperty, size);
		RefreshSolver();
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}