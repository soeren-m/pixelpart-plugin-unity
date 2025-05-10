using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartAccelerationField : PixelpartForceField {
	public PixelpartAnimatedPropertyFloat3 AccelerationDirection { get; }

	public PixelpartAnimatedPropertyFloat AccelerationDirectionVariance { get; }

	public PixelpartAnimatedPropertyFloat AccelerationStrengthVariance { get; }

	public Vector3Int AccelerationGridSize {
		get => new Vector3Int(
			Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeX(effectRuntime, Id),
			Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeY(effectRuntime, Id),
			Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeZ(effectRuntime, Id));
		set => Plugin.PixelpartAccelerationFieldSetAccelerationGridSize(effectRuntime, Id,
			value.x, value.y, value.z);
	}

	public PixelpartAccelerationField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		AccelerationDirection = new PixelpartAnimatedPropertyFloat3(
			Plugin.PixelpartAccelerationFieldGetAccelerationDirection(effectRuntimePtr, id));
		AccelerationDirectionVariance = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartAccelerationFieldGetAccelerationDirectionVariance(effectRuntimePtr, id));
		AccelerationStrengthVariance = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartAccelerationFieldGetAccelerationStrengthVariance(effectRuntimePtr, id));
	}
}
}