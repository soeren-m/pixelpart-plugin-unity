namespace Pixelpart
{
    /// <summary>
    /// Keyframe interpolation types.
    /// </summary>
    public enum PixelpartInterpolationType : int
    {
        /// <summary>
        /// Values change abruptly between keyframes.
        /// </summary>
        Step = 0,

        /// <summary>
        /// Values are interpolated linearly.
        /// </summary>
        Linear = 1,

        /// <summary>
        /// Values are interpolated with Spline interpolation producing smooth transitions.
        /// </summary>
        Spline = 2,

        /// <summary>
        /// Values are interpolated with Bezier interpolation producing smooth transitions.
        /// </summary>
        Bezier = 3
    }
}
