using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
public class PixelpartParticleEmitter {
	public enum ShapeType : int {
		Point = 0,
		Line = 1,
		Ellipse = 2,
		Rectangle = 3,
		Path = 4,
		Ellipsoid = 5,
		Cuboid = 6,
		Cylinder = 7
	}
	public enum DistributionType : int {
		Uniform = 0,
		Center = 1,
		Hole = 2,
		Boundary = 3,
		GridRandom = 4,
		GridOrdered = 5
	}
	public enum GridOrderType : int {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5
	}
	public enum EmissionModeType : int {
		Continuous = 0,
		BurstStart = 1,
		BurstEnd = 2
	}
	public enum DirectionModeType : int {
		Fixed = 0,
		Outwards = 1,
		Inwards = 2,
		Inherit = 3,
		InheritInverse = 4
	}

	public uint Id {
		get {
			return particleEmitterId;
		}
	}
	private readonly uint particleEmitterId;

	public uint ParentId {
		get {
			return Plugin.PixelpartParticleEmitterGetParentId(internalEffect, particleEmitterId);
		}
	}

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartParticleEmitterGetName(internalEffect, particleEmitterId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeStart(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeStart(internalEffect, particleEmitterId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeDuration(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeDuration(internalEffect, particleEmitterId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartParticleEmitterGetRepeat(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetRepeat(internalEffect, particleEmitterId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartParticleEmitterIsActive(internalEffect, particleEmitterId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartParticleEmitterGetLocalTime(internalEffect, particleEmitterId);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Position {
		get {
			return position;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 position;

	public ShapeType Shape {
		get {
			return (ShapeType)Plugin.PixelpartParticleEmitterGetShape(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetShape(internalEffect, particleEmitterId, (int)value);
		}
	}

	public int NumShapePoints {
		get {
			return Plugin.PixelpartParticleEmitterGetNumShapePoints(internalEffect, particleEmitterId);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Size {
		get {
			return size;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 size;

	public PixelpartAnimatedPropertyFloat3 Orientation {
		get {
			return orientation;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 orientation;

	public DistributionType Distribution {
		get {
			return (DistributionType)Plugin.PixelpartParticleEmitterGetDistribution(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDistribution(internalEffect, particleEmitterId, (int)value);
		}
	}

	public GridOrderType GridOrder {
		get {
			return (GridOrderType)Plugin.PixelpartParticleEmitterGetGridOrder(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetGridOrder(internalEffect, particleEmitterId, (int)value);
		}
	}

	public Vector3Int GridSize {
		get {
			return new Vector3Int(
				Plugin.PixelpartParticleEmitterGetGridWidth(internalEffect, particleEmitterId),
				Plugin.PixelpartParticleEmitterGetGridHeight(internalEffect, particleEmitterId),
				Plugin.PixelpartParticleEmitterGetGridDepth(internalEffect, particleEmitterId));
		}
		set {
			Plugin.PixelpartParticleEmitterSetGridSize(internalEffect, particleEmitterId, value.x, value.y, value.z);
		}
	}

	public EmissionModeType EmissionMode {
		get {
			return (EmissionModeType)Plugin.PixelpartParticleEmitterGetEmissionMode(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetEmissionMode(internalEffect, particleEmitterId, (int)value);
		}
	}

	public DirectionModeType DirectionMode {
		get {
			return (DirectionModeType)Plugin.PixelpartParticleEmitterGetDirectionMode(internalEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDirectionMode(internalEffect, particleEmitterId, (int)value);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Direction {
		get {
			return direction;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 direction;

	public PixelpartAnimatedPropertyFloat Spread {
		get {
			return spread;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat spread;

	private readonly IntPtr internalEffect;

	public PixelpartParticleEmitter(IntPtr internalEffectPtr, uint nativeId) {
		internalEffect = internalEffectPtr;
		particleEmitterId = nativeId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetPosition(internalEffect, particleEmitterId));
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetSize(internalEffect, particleEmitterId));
		orientation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetOrientation(internalEffect, particleEmitterId));
		direction = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetDirection(internalEffect, particleEmitterId));
		spread = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleEmitterGetSpread(internalEffect, particleEmitterId));
	}

	public void AddShapePoint(Vector3 point) {
		Plugin.PixelpartParticleEmitterAddShapePoint(internalEffect, particleEmitterId, point);
	}
	public void RemoveShapePoint(int index) {
		Plugin.PixelpartParticleEmitterRemoveShapePoint(internalEffect, particleEmitterId, index);
	}
	public void SetShapePoint(int index, Vector3 point) {
		Plugin.PixelpartParticleEmitterSetShapePoint(internalEffect, particleEmitterId, index, point);
	}
	public Vector3 GetShapePoint(int index) {
		return Plugin.PixelpartParticleEmitterGetShapePoint(internalEffect, particleEmitterId, index);
	}
}
}