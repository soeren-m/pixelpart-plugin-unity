using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAnimatedPropertyFloat4 {
	public InterpolationType KeyframeInterpolation {
		get => (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat4GetKeyframeInterpolation(internalProperty);
		set => Plugin.PixelpartAnimatedPropertyFloat4SetKeyframeInterpolation(internalProperty, (int)value);
	}

	public int KeyframeCount => Plugin.PixelpartAnimatedPropertyFloat4KeyframeCount(internalProperty);

	[Obsolete("deprecated, use KeyframeInterpolation")]
	public InterpolationType Interpolation => KeyframeInterpolation;
	[Obsolete("deprecated, use KeyframeCount")]
	public int NumPoints => KeyframeCount;

	[Obsolete("deprecated, use KeyframeCount")]
	public bool ContainsPoints => KeyframeCount > 0;

	private readonly IntPtr internalProperty;

	public PixelpartAnimatedPropertyFloat4(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector4 At(float position) =>
		Plugin.PixelpartAnimatedPropertyFloat4At(internalProperty, position);

	public void AddKeyframe(float position, Vector4 value) =>
		Plugin.PixelpartAnimatedPropertyFloat4AddKeyframe(internalProperty, position, value);

	public void RemoveKeyframe(int index) =>
		Plugin.PixelpartAnimatedPropertyFloat4RemoveKeyframe(internalProperty, index);

	public void SetKeyframeValue(int index, Vector4 value) =>
		Plugin.PixelpartAnimatedPropertyFloat4SetKeyframeValue(internalProperty, index, value);

	public void SetKeyframePosition(int index, float position) =>
		Plugin.PixelpartAnimatedPropertyFloat4SetKeyframePosition(internalProperty, index, position);

	public void ClearKeyframes() =>
		Plugin.PixelpartAnimatedPropertyFloat4ClearKeyframes(internalProperty);

	public Vector4 GetKeyframeValue(int index) =>
		Plugin.PixelpartAnimatedPropertyFloat4KeyframeValue(internalProperty, index);

	public int GetKeyframeIndex(float position, float epsilon) =>
		Plugin.PixelpartAnimatedPropertyFloat4KeyframeIndex(internalProperty, position, epsilon);

	public void EnableAdaptiveCache() =>
		Plugin.PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(internalProperty);

	public void EnableFixedCache(int size) =>
		Plugin.PixelpartAnimatedPropertyFloat4EnableFixedCache(internalProperty, size);

	[Obsolete("deprecated, use At")]
	public Vector4 Get(float position) => At(position);
	[Obsolete("deprecated, use AddKeyframe")]
	public void AddPoint(float position, Vector4 value) => AddKeyframe(position, value);
	[Obsolete("deprecated, use RemoveKeyframe")]
	public void RemovePoint(int index) => RemoveKeyframe(index);
	[Obsolete("deprecated, use SetKeyframeValue")]
	public void SetPoint(int index, Vector4 value) => SetKeyframeValue(index, value);
	[Obsolete("deprecated, use SetKeyframePosition")]
	public void SetPointPosition(int index, float position) => SetKeyframePosition(index, position);
	[Obsolete("deprecated, use ClearKeyframes")]
	public void Clear() => ClearKeyframes();
	[Obsolete("deprecated, use GetKeyframeValue")]
	public Vector4 GetPoint(int index) => GetKeyframeValue(index);
	[Obsolete("deprecated, use GetKeyframeIndex")]
	public int GetPointIndex(float position, float epsilon) => GetKeyframeIndex(position, epsilon);
}
}