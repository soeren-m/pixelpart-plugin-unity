using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat {
	public InterpolationType Interpolation {
		get {
			return (InterpolationType)Plugin.PixelpartAnimatedPropertyFloatGetInterpolation(internalProperty);
		}
		set {
			Plugin.PixelpartAnimatedPropertyFloatSetInterpolation(internalProperty, (int)value);
		}
	}

	public bool ContainsPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloatContainsPoints(internalProperty);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartAnimatedPropertyFloatGetNumPoints(internalProperty);
		}
	}

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public float Get(float position) {
		return Plugin.PixelpartAnimatedPropertyFloatGet(internalProperty, position);
	}

	public void AddPoint(float position, float value) {
		Plugin.PixelpartAnimatedPropertyFloatAddPoint(internalProperty, position, value);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartAnimatedPropertyFloatRemovePoint(internalProperty, index);
	}
	public void SetPoint(int index, float value) {
		Plugin.PixelpartAnimatedPropertyFloatSetPoint(internalProperty, index, value);
	}
	public void SetPointPosition(int index, float position) {
		Plugin.PixelpartAnimatedPropertyFloatSetPointPosition(internalProperty, index, position);
	}
	public void Clear() {
		Plugin.PixelpartAnimatedPropertyFloatClear(internalProperty);
	}

	public float GetPoint(int index) {
		return Plugin.PixelpartAnimatedPropertyFloatGetPoint(internalProperty, index);
	}
	public int GetPointIndex(float position, float epsilon) {
		return Plugin.PixelpartAnimatedPropertyFloatGetPointIndex(internalProperty, position, epsilon);
	}

	public void EnableAdaptiveCache() {
		Plugin.PixelpartAnimatedPropertyFloatEnableAdaptiveCache(internalProperty);
	}
	public void EnableFixedCache(int size) {
		Plugin.PixelpartAnimatedPropertyFloatEnableFixedCache(internalProperty, size);
	}
}
}