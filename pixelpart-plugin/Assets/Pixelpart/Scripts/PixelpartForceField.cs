using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartForceField {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint forceFieldId = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeStart(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeStart(nativeEffect, forceFieldId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeDuration(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeDuration(nativeEffect, forceFieldId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartForceFieldGetRepeat(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetRepeat(nativeEffect, forceFieldId, value);
		}
	}

	public ForceType Type {
		get {
			return (ForceType)Plugin.PixelpartForceFieldGetType(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetType(nativeEffect, forceFieldId, (int)value);
		}
	}

	public float DirectionVariance {
		get {
			return Plugin.PixelpartForceFieldGetDirectionVariance(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetDirectionVariance(nativeEffect, forceFieldId, value);
		}
	}
	public float StrengthVariance {
		get {
			return Plugin.PixelpartForceFieldGetStrengthVariance(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetStrengthVariance(nativeEffect, forceFieldId, value);
		}
	}

	public Vector2Int GridSize {
		get {
			return new Vector2Int(
				Plugin.PixelpartForceFieldGetGridWidth(nativeEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetGridHeight(nativeEffect, forceFieldId));
		}
		set {
			Plugin.PixelpartForceFieldSetGridSize(nativeEffect, forceFieldId, value.x, value.y);
		}
	}

	public PixelpartForceField(IntPtr nativePtr, uint nativeId) {
		nativeEffect = nativePtr;
		forceFieldId = nativeId;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartForceFieldGetName(nativeEffect, forceFieldId, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}
	public uint GetId() {
		return forceFieldId;
	}

	public bool IsActive() {
		return Plugin.PixelpartForceFieldIsActive(nativeEffect, forceFieldId);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartForceFieldGetLocalTime(nativeEffect, forceFieldId);
	}

	public PixelpartCurve GetWidth() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetWidth(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetHeight() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetHeight(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetOrientation() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetOrientation(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartPath GetMotionPath() {
		return new PixelpartPath(Plugin.PixelpartForceFieldGetMotionPath(nativeEffect, forceFieldId), nativeEffect, PixelpartPath.ObjectType.ForceField);
	}
	public PixelpartCurve GetDirection() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetDirection(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetStrength() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetStrength(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
}
}