using System;

namespace Pixelpart
{
    /// <summary>
    /// Force field that accelerates particles towards or away from its center.
    /// </summary>
    /// <remarks>
    /// Attraction fields are spherical force fields that pull particles towards their center.
    /// You can also make them repel particles by setting a negative strength value.
    /// </remarks>
    public class PixelpartAttractionField : PixelpartForceField
    {
        /// <summary>
        /// Construct <see cref="PixelpartAttractionField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartAttractionField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }
    }
}
