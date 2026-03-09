namespace Pixelpart
{
    /// <summary>
    /// Mode that determines how particles are blended together and are composed with other objects in the effect.
    /// </summary>
    public enum PixelpartBlendMode : int
    {
        /// <summary>
        /// Particles are not blended together and occlude each other.
        /// </summary>
        Off = 0,

        /// <summary>
        /// Particles are blended together based on their opacity.
        /// </summary>
        Alpha = 1,

        /// <summary>
        /// Particle colors are added together.
        /// </summary>
        Additive = 2,

        /// <summary>
        /// Particle colors are subtracted from each other.
        /// </summary>
        Subtractive = 3
    }
}
