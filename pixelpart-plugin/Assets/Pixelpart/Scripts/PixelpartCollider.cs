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
	private uint colliderId = 0;

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

	public PixelpartCurve Bounce {
		get {
			return bounce;
		}
	}
	private PixelpartCurve bounce;

	public PixelpartCurve Friction {
		get {
			return friction;
		}
	}
	private PixelpartCurve friction;

	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartCollider(IntPtr nativeEffectPtr, uint nativeId) {
		nativeEffect = nativeEffectPtr;
		colliderId = nativeId;

		bounce = new PixelpartCurve(Plugin.PixelpartColliderGetBounce(nativeEffect, colliderId), nativeEffect, PixelpartCurve.ObjectType.Collider);
		friction = new PixelpartCurve(Plugin.PixelpartColliderGetFriction(nativeEffect, colliderId), nativeEffect, PixelpartCurve.ObjectType.Collider);
	}

	public void AddPoint(Vector3 point) {
		Plugin.PixelpartColliderAddPoint(nativeEffect, colliderId, point.x, point.y, point.z);
	}
	public void SetPoint(int index, Vector3 point) {
		Plugin.PixelpartColliderSetPoint(nativeEffect, colliderId, index, point.x, point.y, point.z);
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