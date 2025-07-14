using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// <c>Vector2</c> property animated with keyframes and affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that change over time
    /// and are affected by effect inputs. Such properties are represented by one of the <c>PixelpartAnimatedProperty</c> classes,
    /// depending on the type of the property.
    /// </remarks>
    public class PixelpartAnimatedPropertyFloat2
    {
        /// <summary>
        /// Interpolation applied to the animation curve.
        /// </summary>
        public InterpolationType KeyframeInterpolation
        {
            get => (InterpolationType)Plugin.PixelpartAnimatedPropertyFloat2GetKeyframeInterpolation(internalProperty);
            set => Plugin.PixelpartAnimatedPropertyFloat2SetKeyframeInterpolation(internalProperty, (int)value);
        }

        /// <summary>
        /// Number of keyframes.
        /// </summary>
        public int KeyframeCount => Plugin.PixelpartAnimatedPropertyFloat2KeyframeCount(internalProperty);

        /// <summary>
        /// Interpolation applied to the animation curve.
        /// <b>Deprecated</b>, use <see cref="KeyframeInterpolation"/>.
        /// </summary>
        [Obsolete("deprecated, use KeyframeInterpolation")]
        public InterpolationType Interpolation => KeyframeInterpolation;

        /// <summary>
        /// Number of keyframes.
        /// <b>Deprecated</b>, use <see cref="KeyframeCount"/>.
        /// </summary>
        [Obsolete("deprecated, use KeyframeCount")]
        public int NumPoints => KeyframeCount;

        /// <summary>
        /// Whether the animation property contains any keyframes.
        /// <b>Deprecated</b>, use <see cref="KeyframeCount"/>.
        /// </summary>
        [Obsolete("deprecated, use KeyframeCount")]
        public bool ContainsPoints => KeyframeCount > 0;

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartAnimatedPropertyFloat2"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartAnimatedPropertyFloat2(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return the (interpolated) value of the animation property at time <paramref name="position"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <returns>Value of the property</returns>
        public Vector2 At(float position) =>
            Plugin.PixelpartAnimatedPropertyFloat2At(internalProperty, position);

        /// <summary>
        /// Add a keyframe at time <paramref name="position"/> with value <paramref name="value"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="value">Value of the property at the given time</param>
        public void AddKeyframe(float position, Vector2 value) =>
            Plugin.PixelpartAnimatedPropertyFloat2AddKeyframe(internalProperty, position, value);

        /// <summary>
        /// Remove the keyframe with the given index from the animation.
        /// </summary>
        /// <param name="index">Index to remove</param>
        public void RemoveKeyframe(int index) =>
            Plugin.PixelpartAnimatedPropertyFloat2RemoveKeyframe(internalProperty, index);

        /// <summary>
        /// Change the value of the keyframe with the given index.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="value">New value</param>
        public void SetKeyframeValue(int index, Vector2 value) =>
            Plugin.PixelpartAnimatedPropertyFloat2SetKeyframeValue(internalProperty, index, value);

        /// <summary>
        /// Move the time of the keyframe with the given index to <paramref name="position"/>.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="position">New time between 0 and 1</param>
        public void SetKeyframePosition(int index, float position) =>
            Plugin.PixelpartAnimatedPropertyFloat2SetKeyframePosition(internalProperty, index, position);

        /// <summary>
        /// Remove all keyframes from the animation.
        /// </summary>
        public void ClearKeyframes() =>
            Plugin.PixelpartAnimatedPropertyFloat2ClearKeyframes(internalProperty);

        /// <summary>
        /// Return the value of the keyframe with the given index.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <returns>Keyframe value</returns>
        public Vector2 GetKeyframeValue(int index) =>
            Plugin.PixelpartAnimatedPropertyFloat2KeyframeValue(internalProperty, index);

        /// <summary>
        /// Return the index of the keyframe closest to time <paramref name="position"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="epsilon">Maximum differerence in time to consider to keyframes at the same time</param>
        /// <returns>Keyframe index</returns>
        public int GetKeyframeIndex(float position, float epsilon) =>
            Plugin.PixelpartAnimatedPropertyFloat2KeyframeIndex(internalProperty, position, epsilon);

        /// <summary>
        /// Enable an adaptive cache, which resizes itself automatically if the number of keyframes increases or decreases a lot.
        /// </summary>
        public void EnableAdaptiveCache() =>
            Plugin.PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(internalProperty);

        /// <summary>
        /// Enable a fixed cache with the given size, which only stores up to <paramref name="size"/> different value.
        /// </summary>
        /// <param name="size">Cache size</param>
        public void EnableFixedCache(int size) =>
            Plugin.PixelpartAnimatedPropertyFloat2EnableFixedCache(internalProperty, size);

        /// <summary>
        /// Return the (interpolated) value of the animation property at time <paramref name="position"/>.
        /// <b>Deprecated</b>, use <see cref="At"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <returns>Value of the property</returns>
        [Obsolete("deprecated, use At")]
        public Vector2 Get(float position) => At(position);

        /// <summary>
        /// Add a keyframe at time <paramref name="position"/> with value <paramref name="value"/>.
        /// <b>Deprecated</b>, use <see cref="AddKeyframe"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="value">Value of the property at the given time</param>
        [Obsolete("deprecated, use AddKeyframe")]
        public void AddPoint(float position, Vector2 value) => AddKeyframe(position, value);

        /// <summary>
        /// Remove the keyframe with the given index from the animation.
        /// <b>Deprecated</b>, use <see cref="RemoveKeyframe"/>.
        /// </summary>
        /// <param name="index">Index to remove</param>
        [Obsolete("deprecated, use RemoveKeyframe")]
        public void RemovePoint(int index) => RemoveKeyframe(index);

        /// <summary>
        /// Change the value of the keyframe with the given index.
        /// <b>Deprecated</b>, use <see cref="SetKeyframeValue"/>.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="value">New value</param>
        [Obsolete("deprecated, use SetKeyframeValue")]
        public void SetPoint(int index, Vector2 value) => SetKeyframeValue(index, value);

        /// <summary>
        /// Move the time of the keyframe with the given index to <paramref name="position"/>.
        /// <b>Deprecated</b>, use <see cref="SetKeyframePosition"/>.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <param name="position">New time between 0 and 1</param>
        [Obsolete("deprecated, use SetKeyframePosition")]
        public void SetPointPosition(int index, float position) => SetKeyframePosition(index, position);

        /// <summary>
        /// Remove all keyframes from the animation.
        /// <b>Deprecated</b>, use <see cref="ClearKeyframes"/>.
        /// </summary>
        [Obsolete("deprecated, use ClearKeyframes")]
        public void Clear() => ClearKeyframes();

        /// <summary>
        /// Return the value of the keyframe with the given index.
        /// <b>Deprecated</b>, use <see cref="GetKeyframeValue"/>.
        /// </summary>
        /// <param name="index">Keyframe index</param>
        /// <returns>Keyframe value</returns>
        [Obsolete("deprecated, use GetKeyframeValue")]
        public Vector2 GetPoint(int index) => GetKeyframeValue(index);

        /// <summary>
        /// Return the index of the keyframe closest to time <paramref name="position"/>.
        /// <b>Deprecated</b>, use <see cref="GetKeyframeIndex"/>.
        /// </summary>
        /// <param name="position">Time between 0 and 1</param>
        /// <param name="epsilon">Maximum differerence in time to consider to keyframes at the same time</param>
        /// <returns>Keyframe index</returns>
        [Obsolete("deprecated, use GetKeyframeIndex")]
        public int GetPointIndex(float position, float epsilon) => GetKeyframeIndex(position, epsilon);
    }
}