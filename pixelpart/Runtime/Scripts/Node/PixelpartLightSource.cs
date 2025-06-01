using System;

namespace Pixelpart
{
    public class PixelpartLightSource : PixelpartNode
    {
        public PixelpartAnimatedPropertyFloat Attenuation { get; }

        public PixelpartAnimatedPropertyFloat4 Color { get; }

        public PixelpartAnimatedPropertyFloat Intensity { get; }

        public PixelpartLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            Attenuation = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartLightSourceGetAttenuation(effectRuntimePtr, id));
            Color = new PixelpartAnimatedPropertyFloat4(
                Plugin.PixelpartLightSourceGetColor(effectRuntimePtr, id));
            Intensity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartLightSourceGetIntensity(effectRuntimePtr, id));
        }
    }
}