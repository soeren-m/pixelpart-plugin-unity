namespace Pixelpart
{
    /// <summary>
    /// Mode that determines whether the material is shaded by lights in the scene.
    /// </summary>
    public enum PixelpartLightingMode : int
    {
        /// <summary>
        /// The material is not shaded based on lighting.
        /// </summary>
        Unlit = 0,

        /// <summary>
        /// The material is shaded based on lighting.
        /// </summary>
        Lit = 1
    }
}
