using System;

namespace Pixelpart
{
    /// <summary>
    /// Force field that accelerates particles in the direction of a predefined grid of vectors.
    /// </summary>
    /// <remarks>
    /// A particle in the field follows the vector specified in the vector field at that location.
    /// </remarks>
    public class PixelpartVectorField : PixelpartForceField
    {
        /// <summary>
        /// Filtering techniques.
        /// </summary>
        public enum Filter : int
        {
            None = 0,
            Linear = 1
        }

        /// <summary>
        /// How velocity values are interpolated between the cells of the vector field.
        /// </summary>
        public Filter VectorFilter
        {
            get => (Filter)Plugin.PixelpartVectorFieldGetVectorFieldFilter(effectRuntime, Id);
            set => Plugin.PixelpartVectorFieldSetVectorFieldFilter(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// How directly particles follow the vectors in the force field.
        /// </summary>
        /// <remarks>
        /// If set to 0, the field applies forces to the particles over time.
        /// If set to 1, particles follow the vectors in the force field directly.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat Tightness { get; }

        /// <summary>
        /// How directly particles follow the vectors in the force field.
        /// <b>Deprecated</b>, use <see cref="Tightness"/>.
        /// </summary>
        /// <remarks>
        /// If set to 0, the field applies forces to the particles over time.
        /// If set to 1, particles follow the vectors in the force field directly.
        /// </remarks>
        [Obsolete("deprecated, use Tightness")]
        public PixelpartAnimatedPropertyFloat VectorTightness => Tightness;

        /// <summary>
        /// Construct <see cref="PixelpartVectorField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartVectorField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            Tightness = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartVectorFieldGetTightness(effectRuntimePtr, id));
        }
    }
}