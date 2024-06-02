using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat3 {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyFloat3(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector3 Get() {
		return new Vector3(
			Plugin.PixelpartStaticPropertyFloat3GetX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat3GetY(internalProperty),
			Plugin.PixelpartStaticPropertyFloat3GetZ(internalProperty));
	}

	public void SetValue(Vector3 value) {
		Plugin.PixelpartStaticPropertyFloat3SetValue(internalProperty, value);
	}
	public Vector3 GetValue() {
		return new Vector3(
			Plugin.PixelpartStaticPropertyFloat3GetValueX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat3GetValueY(internalProperty),
			Plugin.PixelpartStaticPropertyFloat3GetValueZ(internalProperty));
	}
}
}