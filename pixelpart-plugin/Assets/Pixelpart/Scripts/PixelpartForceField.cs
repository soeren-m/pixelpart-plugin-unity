using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
public class PixelpartForceField {
	public enum ForceType : int {
		Point = 0,
		Area = 1
	}

	public uint Id {
		get {
			return forceFieldId;
		}
	}
	private uint forceFieldId = 0;

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartForceFieldGetName(nativeEffect, forceFieldId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

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
	public bool Active {
		get {
			return Plugin.PixelpartForceFieldIsActive(nativeEffect, forceFieldId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartForceFieldGetLocalTime(nativeEffect, forceFieldId);
		}
	}

	public PixelpartCurve3 Position {
		get {
			return position;
		}
	}
	private PixelpartCurve3 position;

	public ForceType Type {
		get {
			return (ForceType)Plugin.PixelpartForceFieldGetType(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetType(nativeEffect, forceFieldId, (int)value);
		}
	}

	public PixelpartCurve3 Size {
		get {
			return size;
		}
	}
	private PixelpartCurve3 size;

	public PixelpartCurve3 Orientation {
		get {
			return orientation;
		}
	}
	private PixelpartCurve3 orientation;

	public PixelpartCurve3 Direction {
		get {
			return direction;
		}
	}
	private PixelpartCurve3 direction;

	public PixelpartCurve Strength {
		get {
			return strength;
		}
	}
	private PixelpartCurve strength;

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

	public Vector3Int GridSize {
		get {
			return new Vector3Int(
				Plugin.PixelpartForceFieldGetGridWidth(nativeEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetGridHeight(nativeEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetGridDepth(nativeEffect, forceFieldId));
		}
		set {
			Plugin.PixelpartForceFieldSetGridSize(nativeEffect, forceFieldId, value.x, value.y, value.z);
		}
	}

	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartForceField(IntPtr nativeEffectPtr, uint nativeForceFieldId) {
		nativeEffect = nativeEffectPtr;
		forceFieldId = nativeForceFieldId;

		position = new PixelpartCurve3(Plugin.PixelpartForceFieldGetPosition(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve3.ObjectType.ForceField);
		size = new PixelpartCurve3(Plugin.PixelpartForceFieldGetSize(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve3.ObjectType.ForceField);
		orientation = new PixelpartCurve3(Plugin.PixelpartForceFieldGetOrientation(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve3.ObjectType.ForceField);
		direction = new PixelpartCurve3(Plugin.PixelpartForceFieldGetDirection(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve3.ObjectType.ForceField);
		strength = new PixelpartCurve(Plugin.PixelpartForceFieldGetStrength(nativeEffect, forceFieldId), nativeEffect, PixelpartCurve.ObjectType.ForceField);
	}
}
}