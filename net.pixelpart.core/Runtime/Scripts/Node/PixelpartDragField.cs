using System;

namespace Pixelpart
{
    /// <summary>
    /// Force field that simulates drag.
    /// </summary>
    /// <remarks>
    /// Drag fields slow down particles mimicking the effects of drag.
    /// </remarks>
    public class PixelpartDragField : PixelpartForceField
    {
        /// <summary>
        /// How strongly the drag force is influenced by the particle velocity.
        /// </summary>
        public PixelpartStaticPropertyFloat VelocityInfluence { get; }

        /// <summary>
        /// How strongly the drag force is influenced by the particle size.
        /// </summary>
        public PixelpartStaticPropertyFloat SizeInfluence { get; }

        /// <summary>
        /// Construct <see cref="PixelpartDragField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartDragField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            VelocityInfluence = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartDragFieldGetVelocityInfluence(effectRuntimePtr, id));
            SizeInfluence = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartDragFieldGetSizeInfluence(effectRuntimePtr, id));
        }
    }
}
