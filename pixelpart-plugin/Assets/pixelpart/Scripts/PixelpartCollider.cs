using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartCollider {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint colliderIndex = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartColliderGetLifetimeStart(nativeEffect, colliderIndex);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeStart(nativeEffect, colliderIndex, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartColliderGetLifetimeDuration(nativeEffect, colliderIndex);
		}
		set {
			Plugin.PixelpartColliderSetLifetimeDuration(nativeEffect, colliderIndex, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartColliderGetRepeat(nativeEffect, colliderIndex);
		}
		set {
			Plugin.PixelpartColliderSetRepeat(nativeEffect, colliderIndex, value);
		}
	}

	public PixelpartCollider(IntPtr nativePtr, uint nativeIndex) {
		nativeEffect = nativePtr;
		colliderIndex = nativeIndex;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartColliderGetName(nativeEffect, colliderIndex, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}

	public bool IsActive() {
		return Plugin.PixelpartColliderIsActive(nativeEffect, colliderIndex);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartColliderGetLocalTime(nativeEffect, colliderIndex);
	}

	public void AddPoint(Vector2 point) {
		Plugin.PixelpartColliderAddPoint(nativeEffect, colliderIndex, point.x, point.y);
	}
	public void SetPoint(int index, Vector2 point) {
		Plugin.PixelpartColliderSetPoint(nativeEffect, colliderIndex, index, point.x, point.y);
	}
	public void RemovePoint(int index) {
		Plugin.PixelpartColliderRemovePoint(nativeEffect, colliderIndex, index);
	}
	public Vector2 GetPoint(int index) {
		return new Vector2(
			Plugin.PixelpartColliderGetPointX(nativeEffect, colliderIndex, index),
			Plugin.PixelpartColliderGetPointY(nativeEffect, colliderIndex, index));
	}
	public uint GetNumPoints() {
		return Plugin.PixelpartColliderGetNumPoints(nativeEffect, colliderIndex);
	}

	public PixelpartCurve GetBounce() {
		return new PixelpartCurve(Plugin.PixelpartColliderGetBounce(nativeEffect, colliderIndex), nativeEffect, PixelpartCurve.ObjectType.Collider);
	}
	public PixelpartCurve GetFriction() {
		return new PixelpartCurve(Plugin.PixelpartColliderGetFriction(nativeEffect, colliderIndex), nativeEffect, PixelpartCurve.ObjectType.Collider);
	}
}
}