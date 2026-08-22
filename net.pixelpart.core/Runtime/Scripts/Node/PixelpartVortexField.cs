using System;

namespace Pixelpart
{
    /// <summary>
    /// Force field that moves particles in a vortex around its center.
    /// </summary>
    public class PixelpartVortexField : PixelpartForceField
    {
        /// <summary>
        /// How strongly particles are accelerated outwards.
        /// </summary>
        public PixelpartAnimatedPropertyFloat TangentialStrength { get; }

        /// <summary>
        /// How strongly the vortex field attracts particles.
        /// </summary>
        public PixelpartAnimatedPropertyFloat RadialStrength { get; }

        /// <summary>
        /// Construct <see cref="PixelpartVortexField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartVortexField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            TangentialStrength = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartVortexFieldGetTangentialStrength(effectRuntimePtr, id));
            RadialStrength = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartVortexFieldGetRadialStrength(effectRuntimePtr, id));
        }
    }
}
