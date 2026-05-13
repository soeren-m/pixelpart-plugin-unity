using System;

namespace Pixelpart
{
    /// <summary>
    /// Force field that generates random turbulent motion.
    /// </summary>
    /// <remarks>
    /// Noise fields are perfect for creating organic and unpredictable patterns.
    /// </remarks>
    public class PixelpartNoiseField : PixelpartForceField
    {
        /// <summary>
        /// How many layers of noise are blended together to produce the final velocity vector.
        /// </summary>
        public PixelpartStaticPropertyInt NoiseOctaves { get; }

        /// <summary>
        /// Base frequency of the noise generation.
        /// </summary>
        /// <remarks>
        /// Higher values produce more turbulent noise fields.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat NoiseFrequency { get; }

        /// <summary>
        /// How much the amplitude changes after each noise octave.
        /// </summary>
        public PixelpartAnimatedPropertyFloat NoisePersistence { get; }

        /// <summary>
        /// How much the frequency changes after each noise octave.
        /// </summary>
        public PixelpartAnimatedPropertyFloat NoiseLacunarity { get; }

        /// <summary>
        /// Whether the noise field is animated.
        /// </summary>
        public bool NoiseAnimated
        {
            get => PixelpartPlugin.PixelpartNoiseFieldIsNoiseAnimated(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartNoiseFieldSetNoiseAnimated(effectRuntime, Id, value);
        }

        /// <summary>
        /// How fast the noise field changes.
        /// </summary>
        public PixelpartStaticPropertyFloat NoiseAnimationTimeScale { get; }

        /// <summary>
        /// Time offset of the noise animation.
        /// </summary>
        public PixelpartStaticPropertyFloat NoiseAnimationTimeBase { get; }

        /// <summary>
        /// Construct <see cref="PixelpartNoiseField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartNoiseField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            NoiseOctaves = new PixelpartStaticPropertyInt(
                PixelpartPlugin.PixelpartNoiseFieldGetNoiseOctaves(effectRuntimePtr, id));
            NoiseFrequency = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartNoiseFieldGetNoiseFrequency(effectRuntimePtr, id));
            NoisePersistence = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartNoiseFieldGetNoisePersistence(effectRuntimePtr, id));
            NoiseLacunarity = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartNoiseFieldGetNoiseLacunarity(effectRuntimePtr, id));
            NoiseAnimationTimeScale = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartNoiseFieldGetNoiseAnimationTimeScale(effectRuntimePtr, id));
            NoiseAnimationTimeBase = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartNoiseFieldGetNoiseAnimationTimeBase(effectRuntimePtr, id));
        }
    }
}
