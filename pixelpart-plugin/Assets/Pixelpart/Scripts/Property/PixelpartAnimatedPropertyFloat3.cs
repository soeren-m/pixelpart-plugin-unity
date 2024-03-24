using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat3 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat3GetInterpolation(nativeProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat3SetInterpolation(nativeProperty, (int)value);
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
			return Plugin.PixelpartAnimatedPropertyFloat3GetNumPoints(nativeProperty);
		}
	}

	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartAnimatedPropertyFloat3(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector3 Get(float position) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetX(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetY(nativeProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetZ(nativeProperty, position));
	}

	public void AddPoint(float position, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3AddPoint(nativeProperty, position, value);
		RefreshSolver();
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat3RemovePoint(nativeProperty, index);
		RefreshSolver();
	}
	public void SetPoint(int index, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPoint(nativeProperty, index, value);
		RefreshSolver();
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPointPosition(nativeProperty, index, position);
		RefreshSolver();
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat3Clear(nativeProperty);
		RefreshSolver();
	}

	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetPointX(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointY(nativeProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointZ(nativeProperty, index));
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(nativeProperty);
		RefreshSolver();
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat3EnableFixedCache(nativeProperty, size);
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