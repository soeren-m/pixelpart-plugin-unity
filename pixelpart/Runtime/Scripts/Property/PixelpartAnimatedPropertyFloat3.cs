using System;
using UnityEngine;

namespace Pixelpart
{
    public class PixelpartAnimatedPropertyFloat3
    {
        public InterpolationType KeyframeInterpolation
        {
            get => (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat3GetKeyframeInterpolation(internalProperty);
            set => Plugin.PixelpartAnimatedPropertyFloat3SetKeyframeInterpolation(internalProperty, (int)value);
        }

        public int KeyframeCount => Plugin.PixelpartAnimatedPropertyFloat3KeyframeCount(internalProperty);

        [Obsolete("deprecated, use KeyframeInterpolation")]
        public InterpolationType Interpolation => KeyframeInterpolation;
        [Obsolete("deprecated, use KeyframeCount")]
        public int NumPoints => KeyframeCount;

        [Obsolete("deprecated, use KeyframeCount")]
        public bool ContainsPoints => KeyframeCount > 0;

        private readonly IntPtr internalProperty;

        public PixelpartAnimatedPropertyFloat3(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        public Vector3 At(float position) =>
            Plugin.PixelpartAnimatedPropertyFloat3At(internalProperty, position);

        public void AddKeyframe(float position, Vector3 value) =>
            Plugin.PixelpartAnimatedPropertyFloat3AddKeyframe(internalProperty, position, value);

        public void RemoveKeyframe(int index) =>
            Plugin.PixelpartAnimatedPropertyFloat3RemoveKeyframe(internalProperty, index);

        public void SetKeyframeValue(int index, Vector3 value) =>
            Plugin.PixelpartAnimatedPropertyFloat3SetKeyframeValue(internalProperty, index, value);

        public void SetKeyframePosition(int index, float position) =>
            Plugin.PixelpartAnimatedPropertyFloat3SetKeyframePosition(internalProperty, index, position);

        public void ClearKeyframes() =>
            Plugin.PixelpartAnimatedPropertyFloat3ClearKeyframes(internalProperty);

        public Vector3 GetKeyframeValue(int index) =>
            Plugin.PixelpartAnimatedPropertyFloat3KeyframeValue(internalProperty, index);

        public int GetKeyframeIndex(float position, float epsilon) =>
            Plugin.PixelpartAnimatedPropertyFloat3KeyframeIndex(internalProperty, position, epsilon);

        public void EnableAdaptiveCache() =>
            Plugin.PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(internalProperty);

        public void EnableFixedCache(int size) =>
            Plugin.PixelpartAnimatedPropertyFloat3EnableFixedCache(internalProperty, size);

        [Obsolete("deprecated, use At")]
        public Vector3 Get(float position) => At(position);
        [Obsolete("deprecated, use AddKeyframe")]
        public void AddPoint(float position, Vector3 value) => AddKeyframe(position, value);
        [Obsolete("deprecated, use RemoveKeyframe")]
        public void RemovePoint(int index) => RemoveKeyframe(index);
        [Obsolete("deprecated, use SetKeyframeValue")]
        public void SetPoint(int index, Vector3 value) => SetKeyframeValue(index, value);
        [Obsolete("deprecated, use SetKeyframePosition")]
        public void SetPointPosition(int index, float position) => SetKeyframePosition(index, position);
        [Obsolete("deprecated, use ClearKeyframes")]
        public void Clear() => ClearKeyframes();
        [Obsolete("deprecated, use GetKeyframeValue")]
        public Vector3 GetPoint(int index) => GetKeyframeValue(index);
        [Obsolete("deprecated, use GetKeyframeIndex")]
        public int GetPointIndex(float position, float epsilon) => GetKeyframeIndex(position, epsilon);
    }
}