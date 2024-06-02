using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat4 {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyFloat4(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector4 Get() {
		return new Vector4(
			Plugin.PixelpartStaticPropertyFloat4GetX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetY(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetZ(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetW(internalProperty));
	}

	public void SetValue(Vector4 value) {
		Plugin.PixelpartStaticPropertyFloat4SetValue(internalProperty, value);
	}
	public Vector4 GetValue() {
		return new Vector4(
			Plugin.PixelpartStaticPropertyFloat4GetValueX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueY(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueZ(internalProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueW(internalProperty));
	}
}
}