using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat2 {
	public InterpolationType KeyframeInterpolation {
		get => (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat2GetKeyframeInterpolation(internalProperty);
		set => Plugin.PixelpartAnimatedPropertyFloat2SetKeyframeInterpolation(internalProperty, (int)value);
	}

	public int KeyframeCount => Plugin.PixelpartAnimatedPropertyFloat2KeyframeCount(internalProperty);

	[Obsolete("deprecated, use KeyframeInterpolation")]
	public InterpolationType Interpolation => KeyframeInterpolation;
	[Obsolete("deprecated, use KeyframeCount")]
	public int NumPoints => KeyframeCount;

	[Obsolete("deprecated, use KeyframeCount")]
	public bool ContainsPoints => KeyframeCount > 0;

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat2(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector2 At(float position) =>
		Plugin.PixelpartAnimatedPropertyFloat2At(internalProperty, position);

	public void AddKeyframe(float position, Vector2 value) =>
		Plugin.PixelpartAnimatedPropertyFloat2AddKeyframe(internalProperty, position, value);

	public void RemoveKeyframe(int index) =>
		Plugin.PixelpartAnimatedPropertyFloat2RemoveKeyframe(internalProperty, index);

	public void SetKeyframeValue(int index, Vector2 value) =>
		Plugin.PixelpartAnimatedPropertyFloat2SetKeyframeValue(internalProperty, index, value);

	public void SetKeyframePosition(int index, float position) =>
		Plugin.PixelpartAnimatedPropertyFloat2SetKeyframePosition(internalProperty, index, position);

	public void ClearKeyframes() =>
		Plugin.PixelpartAnimatedPropertyFloat2ClearKeyframes(internalProperty);

	public Vector2 GetKeyframeValue(int index) =>
		Plugin.PixelpartAnimatedPropertyFloat2KeyframeValue(internalProperty, index);

	public int GetKeyframeIndex(float position, float epsilon) =>
		Plugin.PixelpartAnimatedPropertyFloat2KeyframeIndex(internalProperty, position, epsilon);

	public void EnableAdaptiveCache() =>
		Plugin.PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(internalProperty);

	public void EnableFixedCache(int size) =>
		Plugin.PixelpartAnimatedPropertyFloat2EnableFixedCache(internalProperty, size);

	[Obsolete("deprecated, use At")]
	public Vector2 Get(float position) => At(position);
	[Obsolete("deprecated, use AddKeyframe")]
	public void AddPoint(float position, Vector2 value) => AddKeyframe(position, value);
	[Obsolete("deprecated, use RemoveKeyframe")]
	public void RemovePoint(int index) => RemoveKeyframe(index);
	[Obsolete("deprecated, use SetKeyframeValue")]
	public void SetPoint(int index, Vector2 value) => SetKeyframeValue(index, value);
	[Obsolete("deprecated, use SetKeyframePosition")]
	public void SetPointPosition(int index, float position) => SetKeyframePosition(index, position);
	[Obsolete("deprecated, use ClearKeyframes")]
	public void Clear() => ClearKeyframes();
	[Obsolete("deprecated, use GetKeyframeValue")]
	public Vector2 GetPoint(int index) => GetKeyframeValue(index);
	[Obsolete("deprecated, use GetKeyframeIndex")]
	public int GetPointIndex(float position, float epsilon) => GetKeyframeIndex(position, epsilon);
}
}