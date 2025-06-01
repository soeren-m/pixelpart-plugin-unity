using System;

namespace Pixelpart
{
    public class PixelpartAnimatedPropertyFloat
    {
        public InterpolationType KeyframeInterpolation
        {
            get => (InterpolationType)Plugin.PixelpartAnimatedPropertyFloatGetKeyframeInterpolation(internalProperty);
            set => Plugin.PixelpartAnimatedPropertyFloatSetKeyframeInterpolation(internalProperty, (int)value);
        }

        public int KeyframeCount => Plugin.PixelpartAnimatedPropertyFloatKeyframeCount(internalProperty);

        [Obsolete("deprecated, use KeyframeInterpolation")]
        public InterpolationType Interpolation => KeyframeInterpolation;
        [Obsolete("deprecated, use KeyframeCount")]
        public int NumPoints => KeyframeCount;

        [Obsolete("deprecated, use KeyframeCount")]
        public bool ContainsPoints => KeyframeCount > 0;

        private readonly IntPtr internalProperty;

        public PixelpartAnimatedPropertyFloat(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        public float At(float position) =>
            Plugin.PixelpartAnimatedPropertyFloatAt(internalProperty, position);

        public void AddKeyframe(float position, float value) =>
            Plugin.PixelpartAnimatedPropertyFloatAddKeyframe(internalProperty, position, value);

        public void RemoveKeyframe(int index) =>
            Plugin.PixelpartAnimatedPropertyFloatRemoveKeyframe(internalProperty, index);

        public void SetKeyframeValue(int index, float value) =>
            Plugin.PixelpartAnimatedPropertyFloatSetKeyframeValue(internalProperty, index, value);

        public void SetKeyframePosition(int index, float position) =>
            Plugin.PixelpartAnimatedPropertyFloatSetKeyframePosition(internalProperty, index, position);

        public void ClearKeyframes() =>
            Plugin.PixelpartAnimatedPropertyFloatClearKeyframes(internalProperty);

        public float GetKeyframeValue(int index) =>
            Plugin.PixelpartAnimatedPropertyFloatKeyframeValue(internalProperty, index);

        public int GetKeyframeIndex(float position, float epsilon) =>
            Plugin.PixelpartAnimatedPropertyFloatKeyframeIndex(internalProperty, position, epsilon);

        public void EnableAdaptiveCache() =>
            Plugin.PixelpartAnimatedPropertyFloatEnableAdaptiveCache(internalProperty);

        public void EnableFixedCache(int size) =>
            Plugin.PixelpartAnimatedPropertyFloatEnableFixedCache(internalProperty, size);

        [Obsolete("deprecated, use At")]
        public float Get(float position) => At(position);
        [Obsolete("deprecated, use AddKeyframe")]
        public void AddPoint(float position, float value) => AddKeyframe(position, value);
        [Obsolete("deprecated, use RemoveKeyframe")]
        public void RemovePoint(int index) => RemoveKeyframe(index);
        [Obsolete("deprecated, use SetKeyframeValue")]
        public void SetPoint(int index, float value) => SetKeyframeValue(index, value);
        [Obsolete("deprecated, use SetKeyframePosition")]
        public void SetPointPosition(int index, float position) => SetKeyframePosition(index, position);
        [Obsolete("deprecated, use ClearKeyframes")]
        public void Clear() => ClearKeyframes();
        [Obsolete("deprecated, use GetKeyframeValue")]
        public float GetPoint(int index) => GetKeyframeValue(index);
        [Obsolete("deprecated, use GetKeyframeIndex")]
        public int GetPointIndex(float position, float epsilon) => GetKeyframeIndex(position, epsilon);
    }
}