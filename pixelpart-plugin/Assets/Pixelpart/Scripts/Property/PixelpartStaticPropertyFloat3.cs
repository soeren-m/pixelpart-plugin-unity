using System;
using System.Text;
using UnityEngine;

namespace Pixelpart {
public class PixelpartStaticPropertyFloat3 {
	private IntPtr nativeProperty = IntPtr.Zero;
	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartStaticPropertyFloat3(IntPtr nativePropertyPtr, IntPtr nativeEffectPtr) {
		nativeProperty = nativePropertyPtr;
		nativeEffect = nativeEffectPtr;
	}

	public Vector3 Get() {
		return new Vector3(
			Plugin.PixelpartStaticPropertyFloat3GetX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat3GetY(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat3GetZ(nativeProperty));
	}

	public void SetValue(Vector3 value) {
		Plugin.PixelpartStaticPropertyFloat3SetValue(nativeProperty, value);
		RefreshSolver();
	}
	public Vector3 GetValue() {
		return new Vector3(
			Plugin.PixelpartStaticPropertyFloat3GetValueX(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat3GetValueY(nativeProperty),
			Plugin.PixelpartStaticPropertyFloat3GetValueZ(nativeProperty));
	}

	private void RefreshSolver() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartRefreshSolver(nativeEffect);
	}
}
}