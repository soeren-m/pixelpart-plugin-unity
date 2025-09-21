using System;

namespace Pixelpart
{
    /// <summary>
    /// Node that emits light.
    /// </summary>
    /// <remarks>
    /// Light sources illuminate the effect and can be used to model phenomena like sunlight, lamps and torches.
    /// Particles with a material that has lighting enabled (for example built-in materials with the suffix Lit)
    /// are shaded by light from light sources in the scene.
    ///
    /// The Unity plugin uses the light sources defined in the scene to apply lighting to effects. To use
    /// light sources defined in the Pixelpart effect file, you need to sync them manually.
    /// </remarks>
    public class PixelpartLightSource : PixelpartNode
    {
        /// <summary>
        /// How quickly the light looses intensity on the way to its outer radius.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Attenuation { get; }

        /// <summary>
        /// Color of the light emitted by the light source.
        /// </summary>
        public PixelpartAnimatedPropertyFloat4 Color { get; }

        /// <summary>
        /// How strongly the light shines.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Intensity { get; }

        /// <summary>
        /// Construct <see cref="PixelpartLightSource"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            Attenuation = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartLightSourceGetAttenuation(effectRuntimePtr, id));
            Color = new PixelpartAnimatedPropertyFloat4(
                Plugin.PixelpartLightSourceGetColor(effectRuntimePtr, id));
            Intensity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartLightSourceGetIntensity(effectRuntimePtr, id));
        }
    }
}
