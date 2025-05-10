using System;

namespace Pixelpart {
public class PixelpartForceField : PixelpartNode {
	public bool Infinite {
		get => Plugin.PixelpartForceFieldIsInfinite(effectRuntime, Id);
		set => Plugin.PixelpartForceFieldSetInfinite(effectRuntime, Id, value);
	}

	public PixelpartAnimatedPropertyFloat Strength { get; }

	public PixelpartForceField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		Strength = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartForceFieldGetStrength(effectRuntimePtr, id));
	}
}
}