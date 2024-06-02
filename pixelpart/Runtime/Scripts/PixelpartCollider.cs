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
			int size = Plugin.PixelpartColliderGetName(internalEffect, colliderId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartColliderGetLifetimeStart(internalEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeStart(internalEffect, colliderId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartColliderGetLifetimeDuration(internalEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeDuration(internalEffect, colliderId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartColliderGetRepeat(internalEffect, colliderId);
		}
		set {
			Plugin.PixelpartColliderSetRepeat(internalEffect, colliderId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartColliderIsActive(internalEffect, colliderId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartColliderGetLocalTime(internalEffect, colliderId);
		}
	}

	public int NumPoints {
		get {
			return Plugin.PixelpartColliderGetNumPoints(internalEffect, colliderId);
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

	public PixelpartStaticPropertyBool KillOnContact {
		get {
			return killOnContact;
		}
	}
	private readonly PixelpartStaticPropertyBool killOnContact;

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

	private readonly IntPtr internalEffect;

	public PixelpartCollider(IntPtr internalEffectPtr, uint nativeId) {
		internalEffect = internalEffectPtr;
		colliderId = nativeId;

		width = new PixelpartStaticPropertyFloat(Plugin.PixelpartColliderGetWidth(internalEffect, colliderId));
		orientation = new PixelpartStaticPropertyFloat(Plugin.PixelpartColliderGetOrientation(internalEffect, colliderId));
		killOnContact = new PixelpartStaticPropertyBool(Plugin.PixelpartColliderGetKillOnContact(internalEffect, colliderId));
		bounce = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartColliderGetBounce(internalEffect, colliderId));
		friction = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartColliderGetFriction(internalEffect, colliderId));
	}

	public void AddPoint(Vector3 point) {
		Plugin.PixelpartColliderAddPoint(internalEffect, colliderId, point);
	}
	public void SetPoint(int index, Vector3 point) {
		Plugin.PixelpartColliderSetPoint(internalEffect, colliderId, index, point);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartColliderRemovePoint(internalEffect, colliderId, index);
	}
	public Vector3 GetPoint(int index) {
		return new Vector3(
			Plugin.PixelpartColliderGetPointX(internalEffect, colliderId, index),
			Plugin.PixelpartColliderGetPointY(internalEffect, colliderId, index),
			Plugin.PixelpartColliderGetPointZ(internalEffect, colliderId, index));
	}
}
}