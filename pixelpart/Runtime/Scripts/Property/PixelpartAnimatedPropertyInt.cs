using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyInt {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyIntGetInterpolation(internalProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyIntSetInterpolation(internalProperty, (int)value);
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyIntContainsPoints(internalProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyIntGetNumPoints(internalProperty);
		}
	}

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyInt(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public int Get(float position) {
		return Plugin.PixelpartAnimatedPropertyIntGet(internalProperty, position);
	}

	public void AddPoint(float position, int value) {
		Plugin.PixelpartAnimatedPropertyIntAddPoint(internalProperty, position, value);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyIntRemovePoint(internalProperty, index);
	}
	public void SetPoint(int index, int value) {
		Plugin.PixelpartAnimatedPropertyIntSetPoint(internalProperty, index, value);
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyIntSetPointPosition(internalProperty, index, position);
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyIntClear(internalProperty);
	}

	public int GetPoint(int index) {
		return Plugin.PixelpartAnimatedPropertyIntGetPoint(internalProperty, index);
	}
	public int GetPointIndex(float position, float epsilon) {
		return Plugin.PixelpartAnimatedPropertyIntGetPointIndex(internalProperty, position, epsilon);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyIntEnableAdaptiveCache(internalProperty);
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyIntEnableFixedCache(internalProperty, size);
	}
}
}