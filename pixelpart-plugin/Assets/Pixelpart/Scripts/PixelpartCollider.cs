using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartCollider {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint colliderId = 0;

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

	public PixelpartCollider(IntPtr nativePtr, uint nativeId) {
		nativeEffect = nativePtr;
		colliderId = nativeId;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartColliderGetName(nativeEffect, colliderId, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}
	public uint GetId() {
		return colliderId;
	}

	public bool IsActive() {
		return Plugin.PixelpartColliderIsActive(nativeEffect, colliderId);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartColliderGetLocalTime(nativeEffect, colliderId);
	}

	public void AddPoint(Vector2 point) {
		Plugin.PixelpartColliderAddPoint(nativeEffect, colliderId, point.x, point.y);
	}
	public void SetPoint(int index, Vector2 point) {
		Plugin.PixelpartColliderSetPoint(nativeEffect, colliderId, index, point.x, point.y);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartColliderRemovePoint(nativeEffect, colliderId, index);
	}
	public Vector2 GetPoint(int index) {
		return new Vector2(
			Plugin.PixelpartColliderGetPointX(nativeEffect, colliderId, index),
			Plugin.PixelpartColliderGetPointY(nativeEffect, colliderId, index));
	}
	public uint GetNumPoints() {
		return Plugin.PixelpartColliderGetNumPoints(nativeEffect, colliderId);
	}

	public PixelpartCurve GetBounce() {
		return new PixelpartCurve(Plugin.PixelpartColliderGetBounce(nativeEffect, colliderId), nativeEffect, PixelpartCurve.ObjectType.Collider);
	}
	public PixelpartCurve GetFriction() {
		return new PixelpartCurve(Plugin.PixelpartColliderGetFriction(nativeEffect, colliderId), nativeEffect, PixelpartCurve.ObjectType.Collider);
	}
}
}