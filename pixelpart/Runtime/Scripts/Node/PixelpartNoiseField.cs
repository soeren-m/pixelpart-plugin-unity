using System;

namespace Pixelpart
{
    public class PixelpartNoiseField : PixelpartForceField
    {
        public PixelpartStaticPropertyInt NoiseOctaves { get; }

        public PixelpartAnimatedPropertyFloat NoiseFrequency { get; }

        public PixelpartAnimatedPropertyFloat NoisePersistence { get; }

        public PixelpartAnimatedPropertyFloat NoiseLacunarity { get; }

        public bool NoiseAnimated
        {
            get => Plugin.PixelpartNoiseFieldIsNoiseAnimated(effectRuntime, Id);
            set => Plugin.PixelpartNoiseFieldSetNoiseAnimated(effectRuntime, Id, value);
        }

        public PixelpartStaticPropertyFloat NoiseAnimationTimeScale { get; }

        public PixelpartStaticPropertyFloat NoiseAnimationTimeBase { get; }

        public PixelpartNoiseField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            NoiseOctaves = new PixelpartStaticPropertyInt(
                Plugin.PixelpartNoiseFieldGetNoiseOctaves(effectRuntimePtr, id));
            NoiseFrequency = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartNoiseFieldGetNoiseFrequency(effectRuntimePtr, id));
            NoisePersistence = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartNoiseFieldGetNoisePersistence(effectRuntimePtr, id));
            NoiseLacunarity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartNoiseFieldGetNoiseLacunarity(effectRuntimePtr, id));
            NoiseAnimationTimeScale = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartNoiseFieldGetNoiseAnimationTimeScale(effectRuntimePtr, id));
            NoiseAnimationTimeBase = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartNoiseFieldGetNoiseAnimationTimeBase(effectRuntimePtr, id));
        }
    }
}