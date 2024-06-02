using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat2 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat2GetInterpolation(internalProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat2SetInterpolation(internalProperty, (int)value);
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat2ContainsPoints(internalProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat2GetNumPoints(internalProperty);
		}
	}

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat2(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector2 Get(float position) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetX(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat2GetY(internalProperty, position));
	}

	public void AddPoint(float position, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2AddPoint(internalProperty, position, value);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat2RemovePoint(internalProperty, index);
	}
	public void SetPoint(int index, Vector2 value) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPoint(internalProperty, index, value);
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat2SetPointPosition(internalProperty, index, position);
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat2Clear(internalProperty);
	}

	public Vector2 GetPoint(int index) {
		return new Vector2(
			Plugin.PixelpartAnimatedPropertyFloat2GetPointX(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat2GetPointY(internalProperty, index));
	}
	public int GetPointIndex(float position, float epsilon) {
		return Plugin.PixelpartAnimatedPropertyFloat2GetPointIndex(internalProperty, position, epsilon);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(internalProperty);
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat2EnableFixedCache(internalProperty, size);
	}
}
}