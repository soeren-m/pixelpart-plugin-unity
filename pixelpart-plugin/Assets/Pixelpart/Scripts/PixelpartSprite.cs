using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartSprite {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint spriteId = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartSpriteGetLifetimeStart(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetLifetimeStart(nativeEffect, spriteId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartSpriteGetLifetimeDuration(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetLifetimeDuration(nativeEffect, spriteId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartSpriteGetRepeat(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetRepeat(nativeEffect, spriteId, value);
		}
	}

	public bool AlignWithPath {
		get {
			return Plugin.PixelpartSpriteGetAlignWithPath(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetAlignWithPath(nativeEffect, spriteId, value);
		}
	}
	public Vector2 Pivot {
		get {
			return new Vector2(
				Plugin.PixelpartSpriteGetPivotX(nativeEffect, spriteId),
				Plugin.PixelpartSpriteGetPivotY(nativeEffect, spriteId));
		}
		set {
			Plugin.PixelpartSpriteSetPivot(nativeEffect, spriteId, value.x, value.y);
		}
	}

	public uint Layer {
		get {
			return Plugin.PixelpartSpriteGetLayer(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetLayer(nativeEffect, spriteId, value);
		}
	}
	public bool Visible {
		get {
			return Plugin.PixelpartSpriteIsVisible(nativeEffect, spriteId);
		}
		set {
			Plugin.PixelpartSpriteSetVisible(nativeEffect, spriteId, value);
		}
	}

	public PixelpartSprite(IntPtr nativePtr, uint nativeId) {
		nativeEffect = nativePtr;
		spriteId = nativeId;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartSpriteGetName(nativeEffect, spriteId, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}
	public uint GetId() {
		return spriteId;
	}

	public bool IsActive() {
		return Plugin.PixelpartSpriteIsActive(nativeEffect, spriteId);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartSpriteGetLocalTime(nativeEffect, spriteId);
	}

	public PixelpartCurve GetWidth() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetWidth(nativeEffect, spriteId), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartCurve GetHeight() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetHeight(nativeEffect, spriteId), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartCurve GetOrientation() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetOrientation(nativeEffect, spriteId), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
	public PixelpartPath GetMotionPath() {
		return new PixelpartPath(Plugin.PixelpartSpriteGetMotionPath(nativeEffect, spriteId), nativeEffect, PixelpartPath.ObjectType.Sprite);
	}
	public PixelpartGradient GetColor() {
		return new PixelpartGradient(Plugin.PixelpartSpriteGetColor(nativeEffect, spriteId), nativeEffect, PixelpartGradient.ObjectType.Sprite);
	}
	public PixelpartCurve GetOpacity() {
		return new PixelpartCurve(Plugin.PixelpartSpriteGetOpacity(nativeEffect, spriteId), nativeEffect, PixelpartCurve.ObjectType.Sprite);
	}
}
}