using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat4 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat4GetInterpolation(nativeProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat4SetInterpolation(nativeProperty, (int)value);
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
			return Plugin.PixelpartAnimatedPropertyFloat4GetNumPoints(nativeProperty);
		}
	}

	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartAnimatedPropertyFloat4(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector4 Get(float position) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetX(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetY(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetZ(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetW(nativeProperty, position));
	}

	public void AddPoint(float position, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4AddPoint(nativeProperty, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat4RemovePoint(nativeProperty, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPoint(nativeProperty, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPointPosition(nativeProperty, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat4Clear(nativeProperty);
		RefreshSolver();
	}

	public Vector4 GetPoint(int index) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetPointX(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointY(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointZ(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointW(nativeProperty, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(nativeProperty);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat4EnableFixedCache(nativeProperty, size);
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