using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat2 {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyFloat2(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public Vector2 Get() {
		return new Vector2(
			Plugin.PixelpartStaticPropertyFloat2GetX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat2GetY(internalProperty));
	}

	public void SetValue(Vector2 value) {
		Plugin.PixelpartStaticPropertyFloat2SetValue(internalProperty, value);
	}
	public Vector2 GetValue() {
		return new Vector2(
			Plugin.PixelpartStaticPropertyFloat2GetValueX(internalProperty),
			Plugin.PixelpartStaticPropertyFloat2GetValueY(internalProperty));
	}
}
}