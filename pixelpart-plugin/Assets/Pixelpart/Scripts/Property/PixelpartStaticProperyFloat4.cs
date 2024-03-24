using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat4 {
	private readonly IntPtr nativeProperty;

	private readonly IntPtr nativeEffect;

	public PixelpartStaticPropertyFloat4(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector4 Get() {
		return new Vector4(
			Plugin.PixelpartStaticPropertyFloat4GetX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetY(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetZ(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetW(nativeProperty));
	}

	public void SetValue(Vector4 value) {
		Plugin.PixelpartStaticPropertyFloat4SetValue(nativeProperty, value);
		RefreshSolver();
	}
	public Vector4 GetValue() {
		return new Vector4(
			Plugin.PixelpartStaticPropertyFloat4GetValueX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueY(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueZ(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat4GetValueW(nativeProperty));
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshParticleSolver(nativeEffect);
	}
}
}