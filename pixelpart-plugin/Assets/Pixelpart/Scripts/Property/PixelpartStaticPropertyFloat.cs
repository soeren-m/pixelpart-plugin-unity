using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat {
	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartStaticPropertyFloat(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public float Get() {
		return Plugin.PixelpartStaticPropertyFloatGet(nativeProperty);
	}

	public void SetValue(float value) {
		Plugin.PixelpartStaticPropertyFloatSetValue(nativeProperty, value);
		RefreshSolver();
	}
	public float GetValue() {
		return Plugin.PixelpartStaticPropertyFloatGetValue(nativeProperty);
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}