using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyInt {
	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartStaticPropertyInt(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public int Get() {
		return Plugin.PixelpartStaticPropertyIntGet(nativeProperty);
	}

	public void SetValue(int value) {
		Plugin.PixelpartStaticPropertyIntSetValue(nativeProperty, value);
		RefreshSolver();
	}
	public int GetValue() {
		return Plugin.PixelpartStaticPropertyIntGetValue(nativeProperty);
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}