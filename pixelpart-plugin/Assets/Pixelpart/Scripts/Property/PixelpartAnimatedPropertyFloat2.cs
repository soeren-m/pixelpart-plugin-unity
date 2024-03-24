using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat2 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat2GetInterpolation(nativeProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat2SetInterpolation(nativeProperty, (int)value);
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
			return Plugin.PixelpartAnimatedPropertyFloat2GetNumPoints(nativeProperty);
		}
	}

	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartAnimatedPropertyFloat2(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector2 Get(float position) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetX(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat2GetY(nativeProperty, position));
	}

	public void AddPoint(float position, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2AddPoint(nativeProperty, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat2RemovePoint(nativeProperty, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPoint(nativeProperty, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPointPosition(nativeProperty, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat2Clear(nativeProperty);
		RefreshSolver();
	}

	public Vector2 GetPoint(int index) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetPointX(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat2GetPointY(nativeProperty, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(nativeProperty);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat2EnableFixedCache(nativeProperty, size);
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