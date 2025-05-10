using System;

namespace Pixelpart {
public class PixelpartVectorField : PixelpartForceField {
	public enum Filter : int {
		None = 0,
		Linear = 1
	}

	public Filter VectorFilter {
		get => (Filter)Plugin.PixelpartVectorFieldGetVectorFieldFilter(effectRuntime, Id);
		set => Plugin.PixelpartVectorFieldSetVectorFieldFilter(effectRuntime, Id, (int)value);
	}

	public PixelpartAnimatedPropertyFloat Tightness { get; }

	[Obsolete("deprecated, use Tightness")]
	public PixelpartAnimatedPropertyFloat VectorTightness => Tightness;

	public PixelpartVectorField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		Tightness = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartVectorFieldGetTightness(effectRuntimePtr, id));
	}
}
}