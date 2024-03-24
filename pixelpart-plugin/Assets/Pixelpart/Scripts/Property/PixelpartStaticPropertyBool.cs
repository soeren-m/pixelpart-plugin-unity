using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyBool {
	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartStaticPropertyBool(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public bool Get() {
		return Plugin.PixelpartStaticPropertyBoolGet(nativeProperty);
	}

	public void SetValue(bool value) {
		Plugin.PixelpartStaticPropertyBoolSetValue(nativeProperty, value);
		RefreshSolver();
	}
	public bool GetValue() {
		return Plugin.PixelpartStaticPropertyBoolGetValue(nativeProperty);
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}