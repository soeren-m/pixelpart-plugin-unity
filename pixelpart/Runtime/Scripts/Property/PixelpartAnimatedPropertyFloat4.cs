using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat4 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat4GetInterpolation(internalProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat4SetInterpolation(internalProperty, (int)value);
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat4ContainsPoints(internalProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat4GetNumPoints(internalProperty);
		}
	}

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat4(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector4 Get(float position) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetX(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetY(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetZ(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat4GetW(internalProperty, position));
	}

	public void AddPoint(float position, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4AddPoint(internalProperty, position, value);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat4RemovePoint(internalProperty, index);
	}
	public void SetPoint(int index, Vector4 value) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPoint(internalProperty, index, value);
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat4SetPointPosition(internalProperty, index, position);
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat4Clear(internalProperty);
	}

	public Vector4 GetPoint(int index) {
		return new Vector4(
			Plugin.PixelpartAnimatedPropertyFloat4GetPointX(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointY(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointZ(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat4GetPointW(internalProperty, index));
	}
	public int GetPointIndex(float position, float epsilon) {
		return Plugin.PixelpartAnimatedPropertyFloat4GetPointIndex(internalProperty, position, epsilon);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(internalProperty);
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat4EnableFixedCache(internalProperty, size);
	}
}
}