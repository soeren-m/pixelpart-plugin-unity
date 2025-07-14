using System;

namespace Pixelpart
{
    /// <summary>
    /// Collider consisting of a single line (2D) or plane (3D) particles collide with.
    /// </summary>
    public class PixelpartPlaneCollider : PixelpartCollider
    {
        /// <summary>
        /// Construct <see cref="PixelpartPlaneCollider"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartPlaneCollider(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }
    }
}