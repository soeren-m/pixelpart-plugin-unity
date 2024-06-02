using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyBool {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyBool(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public bool Get() {
		return Plugin.PixelpartStaticPropertyBoolGet(internalProperty);
	}

	public void SetValue(bool value) {
		Plugin.PixelpartStaticPropertyBoolSetValue(internalProperty, value);
	}
	public bool GetValue() {
		return Plugin.PixelpartStaticPropertyBoolGetValue(internalProperty);
	}
}
}