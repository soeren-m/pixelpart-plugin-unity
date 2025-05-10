using System;

namespace Pixelpart {
public class PixelpartDragField : PixelpartForceField {
	public PixelpartStaticPropertyFloat VelocityInfluence { get; }

	public PixelpartStaticPropertyFloat SizeInfluence { get; }

	[Obsolete("deprecated, use VelocityInfluence")]
	public PixelpartStaticPropertyFloat DragVelocityInfluence => VelocityInfluence;

	[Obsolete("deprecated, use SizeInfluence")]
	public PixelpartStaticPropertyFloat DragSizeInfluence => SizeInfluence;

	public PixelpartDragField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		VelocityInfluence = new PixelpartStaticPropertyFloat(
			Plugin.PixelpartDragFieldGetVelocityInfluence(effectRuntimePtr, id));
		SizeInfluence = new PixelpartStaticPropertyFloat(
			Plugin.PixelpartDragFieldGetSizeInfluence(effectRuntimePtr, id));
	}
}
}