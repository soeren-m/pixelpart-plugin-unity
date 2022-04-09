using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartForceField {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint forceFieldIndex = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeStart(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeStart(nativeEffect, forceFieldIndex, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeDuration(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeDuration(nativeEffect, forceFieldIndex, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartForceFieldGetRepeat(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetRepeat(nativeEffect, forceFieldIndex, value);
		}
	}

	public ForceType Type {
		get {
			return (ForceType)Plugin.PixelpartForceFieldGetType(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetType(nativeEffect, forceFieldIndex, (int)value);
		}
	}

	public float DirectionVariance {
		get {
			return Plugin.PixelpartForceFieldGetDirectionVariance(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetDirectionVariance(nativeEffect, forceFieldIndex, value);
		}
	}
	public float StrengthVariance {
		get {
			return Plugin.PixelpartForceFieldGetStrengthVariance(nativeEffect, forceFieldIndex);
		}
		set {
			Plugin.PixelpartForceFieldSetStrengthVariance(nativeEffect, forceFieldIndex, value);
		}
	}

	public Vector2Int GridSize {
		get {
			return new Vector2Int(
				Plugin.PixelpartForceFieldGetGridWidth(nativeEffect, forceFieldIndex),
				Plugin.PixelpartForceFieldGetGridHeight(nativeEffect, forceFieldIndex));
		}
		set {
			Plugin.PixelpartForceFieldSetGridSize(nativeEffect, forceFieldIndex, value.x, value.y);
		}
	}

	public PixelpartForceField(IntPtr nativePtr, uint nativeIndex) {
		nativeEffect = nativePtr;
		forceFieldIndex = nativeIndex;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartForceFieldGetName(nativeEffect, forceFieldIndex, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}

	public bool IsActive() {
		return Plugin.PixelpartForceFieldIsActive(nativeEffect, forceFieldIndex);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartForceFieldGetLocalTime(nativeEffect, forceFieldIndex);
	}

	public PixelpartCurve GetWidth() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetWidth(nativeEffect, forceFieldIndex), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetHeight() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetHeight(nativeEffect, forceFieldIndex), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetOrientation() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetOrientation(nativeEffect, forceFieldIndex), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartPath GetMotionPath() {
		return new PixelpartPath(Plugin.PixelpartForceFieldGetMotionPath(nativeEffect, forceFieldIndex), nativeEffect, PixelpartPath.ObjectType.ForceField);
	}
	public PixelpartCurve GetDirection() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetDirection(nativeEffect, forceFieldIndex), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
	public PixelpartCurve GetStrength() {
		return new PixelpartCurve(Plugin.PixelpartForceFieldGetStrength(nativeEffect, forceFieldIndex), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
}
}