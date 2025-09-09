using System;

namespace Pixelpart
{
    /// <summary>
    /// Node that acts as a parent node for other nodes.
    /// </summary>
    /// <remarks>
    /// Group nodes are nodes with no function other than containing other nodes
    /// like emitters and force fields.
    /// </remarks>
    public class PixelpartGroupNode : PixelpartNode
    {
        /// <summary>
        /// Construct <see cref="PixelpartGroupNode"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartGroupNode(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }
    }
}
