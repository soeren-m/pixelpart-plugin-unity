using System;

namespace Pixelpart
{
    /// <summary>
    /// Light source with light coming from a specific direction.
    /// </summary>
    /// <remarks>
    /// Direction lights mimic light sources that are very far away and consequently contain light rays of almost parallel direction.
    /// Such light sources are useful to simulate sunlight, for example.
    /// The direction is determined by the node's orientation property.
    /// </remarks>
    public class PixelpartDirectionalLightSource : PixelpartLightSource
    {
        /// <summary>
        /// Construct <see cref="PixelpartDirectionalLightSource"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartDirectionalLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }
    }
}
