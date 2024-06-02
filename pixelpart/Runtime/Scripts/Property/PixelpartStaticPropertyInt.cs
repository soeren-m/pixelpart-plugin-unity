using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyInt {
	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyInt(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	public int Get() {
		return Plugin.PixelpartStaticPropertyIntGet(internalProperty);
	}

	public void SetValue(int value) {
		Plugin.PixelpartStaticPropertyIntSetValue(internalProperty, value);
	}
	public int GetValue() {
		return Plugin.PixelpartStaticPropertyIntGetValue(internalProperty);
	}
}
}