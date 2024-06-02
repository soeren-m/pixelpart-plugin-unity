using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat3 {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat3GetInterpolation(internalProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloat3SetInterpolation(internalProperty, (int)value);
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat3ContainsPoints(internalProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloat3GetNumPoints(internalProperty);
		}
	}

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat3(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector3 Get(float position) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetX(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetY(internalProperty, position),
			Plugin.PixelpartAnimatedPropertyFloat3GetZ(internalProperty, position));
	}

	public void AddPoint(float position, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3AddPoint(internalProperty, position, value);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloat3RemovePoint(internalProperty, index);
	}
	public void SetPoint(int index, Vector3 value) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPoint(internalProperty, index, value);
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloat3SetPointPosition(internalProperty, index, position);
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloat3Clear(internalProperty);
	}

	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartAnimatedPropertyFloat3GetPointX(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointY(internalProperty, index),
			Plugin.PixelpartAnimatedPropertyFloat3GetPointZ(internalProperty, index));
	}
	public int GetPointIndex(float position, float epsilon) {
		return Plugin.PixelpartAnimatedPropertyFloat3GetPointIndex(internalProperty, position, epsilon);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(internalProperty);
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloat3EnableFixedCache(internalProperty, size);
	}
}
}