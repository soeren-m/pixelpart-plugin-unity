using System;

namespace Pixelpart
{
    /// <summary>
    /// Light source similar to a point light but emitted light is restricted to a cone shape.
    /// </summary>
    /// <remarks>
    /// The direction is determined by the node's orientation property.
    /// </remarks>
    public class PixelpartSpotLightSource : PixelpartLightSource
    {
        /// <summary>
        /// Width of the light cone in degrees.
        /// </summary>
        public PixelpartAnimatedPropertyFloat SpotAngle { get; }

        /// <summary>
        /// How much the light looses intensity near the edge of the light cone.
        /// </summary>
        public PixelpartAnimatedPropertyFloat SpotAngleAttenuation { get; }

        /// <summary>
        /// Construct <see cref="PixelpartSpotLightSource"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartSpotLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            SpotAngle = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartSpotLightSourceGetSpotAngle(effectRuntimePtr, id));
            SpotAngleAttenuation = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartSpotLightSourceGetSpotAngleAttenuation(effectRuntimePtr, id));
        }
    }
}
