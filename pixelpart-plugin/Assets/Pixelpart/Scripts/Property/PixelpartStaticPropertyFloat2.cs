using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat2 {
	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartStaticPropertyFloat2(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector2 Get() {
		return new Vector2(
			Plugin.PixelpartStaticPropertyFloat2GetX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat2GetY(nativeProperty));
	}

	public void SetValue(Vector2 value) {
		Plugin.PixelpartStaticPropertyFloat2SetValue(nativeProperty, value);
		RefreshSolver();
	}
	public Vector2 GetValue() {
		return new Vector2(
			Plugin.PixelpartStaticPropertyFloat2GetValueX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat2GetValueY(nativeProperty));
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}