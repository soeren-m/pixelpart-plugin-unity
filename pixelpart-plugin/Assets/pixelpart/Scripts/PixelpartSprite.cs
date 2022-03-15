using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartSprite {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint spriteIndex = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartSpriteGetLifetimeStart(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetLifetimeStart(nativeEffect, spriteIndex, value);
		}
	}

	public float LifetimeDuration {
		get {
			return Plugin.PixelpartSpriteGetLifetimeDuration(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetLifetimeDuration(nativeEffect, spriteIndex, value);
		}
	}

	public bool Repeat {
		get {
			return Plugin.PixelpartSpriteGetRepeat(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetRepeat(nativeEffect, spriteIndex, value);
		}
	}

	public BlendModeType BlendMode {
		get {
			return (BlendModeType)Plugin.PixelpartSpriteGetBlendMode(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetBlendMode(nativeEffect, spriteIndex, (int)value);
		}
	}

	public ColorModeType ColorMode {
		get {
			return (ColorModeType)Plugin.PixelpartSpriteGetColorMode(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetColorMode(nativeEffect, spriteIndex, (int)value);
		}
	}

	public uint Layer {
		get {
			return Plugin.PixelpartSpriteGetLayer(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetLayer(nativeEffect, spriteIndex, value);
		}
	}

	public bool Visible {
		get {
			return Plugin.PixelpartSpriteIsVisible(nativeEffect, spriteIndex);
		}
		set {
			Plugin.PixelpartSpriteSetVisible(nativeEffect, spriteIndex, value);
		}
	}

	public PixelpartSprite(IntPtr nativePtr, uint nativeIndex) {
		nativeEffect = nativePtr;
		spriteIndex = nativeIndex;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartSpriteGetName(nativeEffect, spriteIndex, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}

	public bool IsActive() {
		return Plugin.PixelpartSpriteIsActive(nativeEffect, spriteIndex);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartSpriteGetLocalTime(nativeEffect, spriteIndex);
	}

	public PixelpartCurve GetWidth() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetWidth(nativeEffect, spriteIndex), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartCurve GetHeight() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetHeight(nativeEffect, spriteIndex), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartCurve GetOrientation() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetOrientation(nativeEffect, spriteIndex), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartPath GetMotionPath() {
		return new PixelpartPath(Plugin.PixelpartSpriteGetMotionPath(nativeEffect, spriteIndex), nativeEffect, PixelpartPath.ObjectType.Sprite);
	}
	public PixelpartGradient GetColor() {
		return new PixelpartGradient(Plugin.PixelpartSpriteGetColor(nativeEffect, spriteIndex), nativeEffect, PixelpartGradient.ObjectType.Sprite);
	}
	public PixelpartCurve GetOpacity() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetOpacity(nativeEffect, spriteIndex), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	
}
}