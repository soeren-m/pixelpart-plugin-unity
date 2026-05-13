using System;

namespace Pixelpart
{
    /// <summary>
    /// <c>float</c> property animated with keyframes and affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that change over time
    /// and are affected by effect inputs. Such properties are represented by one of the <c>PixelpartAnimatedProperty</c> classes,
    /// depending on the type of the property.
    /// </remarks>
    public class PixelpartAnimatedPropertyFloat
    {
        /// <summary>
        /// Interpolation applied to the animation curve.
        /// </summary>
        public PixelpartInterpolationType KeyframeInterpolation
        {
            get => (PixelpartInterpolationType)PixelpartPlugin.PixelpartAnimatedPropertyFloatGetKeyframeInterpolation(internalProperty);
            set => PixelpartPlugin.PixelpartAnimatedPropertyFloatSetKeyframeInterpolation(internalProperty, (int)value);
        }

        /// <summary>
        /// Number of keyframes.
        /// </summary>
        public int KeyframeCount => PixelpartPlugin.PixelpartAnimatedPropertyFloatKeyframeCount(internalProperty);

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartAnimatedPropertyFloat"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartAnimatedPropertyFloat(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return the (interpolated) value of the animation property at time <paramref name="position"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <returns>Value of the property</returns>
        public float At(float position) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatAt(internalProperty, position);

        /// <summary>
        /// Add a keyframe at time <paramref name="position"/> with value <paramref name="value"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="value">Value of the property at the given time</param>
        public void AddKeyframe(float position, float value) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatAddKeyframe(internalProperty, position, value);

        /// <summary>
        /// Remove the keyframe with the given index from the animation.
        /// </summary>
        /// <param name="index">Index to remove</param>
        public void RemoveKeyframe(int index) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatRemoveKeyframe(internalProperty, index);

        /// <summary>
        /// Change the value of the keyframe with the given index.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="value">New value</param>
        public void SetKeyframeValue(int index, float value) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatSetKeyframeValue(internalProperty, index, value);

        /// <summary>
        /// Move the time of the keyframe with the given index to <paramref name="position"/>.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="position">New time between 0 and 1</param>
        public void SetKeyframePosition(int index, float position) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatSetKeyframePosition(internalProperty, index, position);

        /// <summary>
        /// Remove all keyframes from the animation.
        /// </summary>
        public void ClearKeyframes() =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatClearKeyframes(internalProperty);

        /// <summary>
        /// Return the value of the keyframe with the given index.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <returns>Keyframe value</returns>
        public float GetKeyframeValue(int index) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatKeyframeValue(internalProperty, index);

        /// <summary>
        /// Return the index of the keyframe closest to time <paramref name="position"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="epsilon">Maximum differerence in time to consider to keyframes at the same time</param>
        /// <returns>Keyframe index</returns>
        public int GetKeyframeIndex(float position, float epsilon) =>
            PixelpartPlugin.PixelpartAnimatedPropertyFloatKeyframeIndex(internalProperty, position, epsilon);
    }
}
