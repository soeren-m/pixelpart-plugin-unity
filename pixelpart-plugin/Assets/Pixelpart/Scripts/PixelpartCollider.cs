using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
public class PixelpartCollider {
	public uint Id {
		get {
			return colliderId;
		}
	}
	private readonly uint colliderId;

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartColliderGetName(nativeEffect, colliderId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartColliderGetLifetimeStart(nativeEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeStart(nativeEffect, colliderId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartColliderGetLifetimeDuration(nativeEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeDuration(nativeEffect, colliderId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartColliderGetRepeat(nativeEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetRepeat(nativeEffect, colliderId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartColliderIsActive(nativeEffect, colliderId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartColliderGetLocalTime(nativeEffect, colliderId);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartColliderGetNumPoints(nativeEffect, colliderId);
		}
	}

	public PixelpartStaticPropertyFloat Width {
		get {
			return width;
		}
	}
	private readonly PixelpartStaticPropertyFloat width;

	public PixelpartStaticPropertyFloat Orientation {
		get {
			return orientation;
		}
	}
	private readonly PixelpartStaticPropertyFloat orientation;

	// TODO: kill on contact

	public PixelpartAnimatedPropertyFloat Bounce {
		get {
			return bounce;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat bounce;

	public PixelpartAnimatedPropertyFloat Friction {
		get {
			return friction;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat friction;

	private readonly IntPtr nativeEffect;

	public PixelpartCollider(IntPtr nativeEffectPtr, uint nativeId) {
		nativeEffect = nativeEffectPtr;
		colliderId = nativeId;

		width = new PixelpartStaticPropertyFloat(Plugin.PixelpartColliderGetWidth(nativeEffect, colliderId), nativeEffect);
		orientation = new PixelpartStaticPropertyFloat(Plugin.PixelpartColliderGetOrientation(nativeEffect, colliderId), nativeEffect);
		bounce = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartColliderGetBounce(nativeEffect, colliderId), nativeEffect);
		friction = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartColliderGetFriction(nativeEffect, colliderId), nativeEffect);
	}

	public void AddPoint(Vector3 point) {
		Plugin.PixelpartColliderAddPoint(nativeEffect, colliderId, point);
	}
	public void SetPoint(int index, Vector3 point) {
		Plugin.PixelpartColliderSetPoint(nativeEffect, colliderId, index, point);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartColliderRemovePoint(nativeEffect, colliderId, index);
	}
	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartColliderGetPointX(nativeEffect, colliderId, index),
			Plugin.PixelpartColliderGetPointY(nativeEffect, colliderId, index),
			Plugin.PixelpartColliderGetPointZ(nativeEffect, colliderId, index));
	}
}
}