using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyFloat(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public float Get() {
		return Plugin.PixelpartStaticPropertyFloatGet(internalProperty);
	}

	public void SetValue(float value) {
		Plugin.PixelpartStaticPropertyFloatSetValue(internalProperty, value);
	}
	public float GetValue() {
		return Plugin.PixelpartStaticPropertyFloatGetValue(internalProperty);
	}
}
}