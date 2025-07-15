using System;

namespace Pixelpart
{
    /// <summary>
    /// Light source used to model light coming from a single point emitting light in all directions.
    /// </summary>
    public class PixelpartPointLightSource : PixelpartLightSource
    {
        /// <summary>
        /// Construct <see cref="PixelpartPointLightSource"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartPointLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }
    }
}