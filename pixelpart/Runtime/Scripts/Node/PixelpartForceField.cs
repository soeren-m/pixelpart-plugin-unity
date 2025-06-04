using System;

namespace Pixelpart
{
    /// <summary>
    /// Node that interacts with particles in a given area by applying forces
    /// and changing the particles' velocity.
    /// </summary>
    /// <remarks>
    /// Force fields affect particles by attracting, repelling or accelerating them in some direction
    /// and can be used to model gravity, wind and other natural effects.
    /// </remarks>
    public class PixelpartForceField : PixelpartNode
    {
        /// <summary>
        /// Whether the force field has an infinite area of effect.
        /// </summary>
        public bool Infinite
        {
            get => Plugin.PixelpartForceFieldIsInfinite(effectRuntime, Id);
            set => Plugin.PixelpartForceFieldSetInfinite(effectRuntime, Id, value);
        }

        /// <summary>
        /// How strongly particles are affected by the force field.
        /// </summary>
        /// <remarks>
        /// Negative values reverse the force direction. Attraction fields then repel particles, for example.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat Strength { get; }

        /// <summary>
        /// Construct <see cref="PixelpartForceField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartForceField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            Strength = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartForceFieldGetStrength(effectRuntimePtr, id));
        }
    }
}