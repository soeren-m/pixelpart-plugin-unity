using System;

namespace Pixelpart {
public class PixelpartPointLightSource : PixelpartLightSource {
	public PixelpartAnimatedPropertyFloat SpotAngle { get; }

	public PixelpartAnimatedPropertyFloat SpotAngleAttenuation { get; }

	public PixelpartPointLightSource(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		SpotAngle = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartSpotLightSourceGetSpotAngle(effectRuntimePtr, id));
		SpotAngleAttenuation = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartSpotLightSourceGetSpotAngleAttenuation(effectRuntimePtr, id));
	}
}
}