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
			return Plugin.PixelpartParticleEmitterGetParentId(nativeEffect, particleEmitterId);
		}
	}

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartParticleEmitterGetName(nativeEffect, particleEmitterId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeStart(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeStart(nativeEffect, particleEmitterId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeDuration(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeDuration(nativeEffect, particleEmitterId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartParticleEmitterGetRepeat(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetRepeat(nativeEffect, particleEmitterId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartParticleEmitterIsActive(nativeEffect, particleEmitterId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartParticleEmitterGetLocalTime(nativeEffect, particleEmitterId);
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
			return (ShapeType)Plugin.PixelpartParticleEmitterGetShape(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetShape(nativeEffect, particleEmitterId, (int)value);
		}
	}

	// TODO
	/*public PixelpartAnimatedPropertyFloat3 Path {
		get {
			return path;
		}
	}
	private PixelpartAnimatedPropertyFloat3 path;*/

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
			return (DistributionType)Plugin.PixelpartParticleEmitterGetDistribution(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDistribution(nativeEffect, particleEmitterId, (int)value);
		}
	}

	public GridOrderType GridOrder {
		get {
			return (GridOrderType)Plugin.PixelpartParticleEmitterGetGridOrder(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetGridOrder(nativeEffect, particleEmitterId, (int)value);
		}
	}

	public Vector3Int GridSize {
		get {
			return new Vector3Int(
				Plugin.PixelpartParticleEmitterGetGridWidth(nativeEffect, particleEmitterId),
				Plugin.PixelpartParticleEmitterGetGridHeight(nativeEffect, particleEmitterId),
				Plugin.PixelpartParticleEmitterGetGridDepth(nativeEffect, particleEmitterId));
		}
		set {
			Plugin.PixelpartParticleEmitterSetGridSize(nativeEffect, particleEmitterId, value.x, value.y, value.z);
		}
	}

	public EmissionModeType EmissionMode {
		get {
			return (EmissionModeType)Plugin.PixelpartParticleEmitterGetEmissionMode(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetEmissionMode(nativeEffect, particleEmitterId, (int)value);
		}
	}

	public DirectionModeType DirectionMode {
		get {
			return (DirectionModeType)Plugin.PixelpartParticleEmitterGetDirectionMode(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDirectionMode(nativeEffect, particleEmitterId, (int)value);
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

	private readonly IntPtr nativeEffect;

	public PixelpartParticleEmitter(IntPtr nativeEffectPtr, uint nativeId) {
		nativeEffect = nativeEffectPtr;
		particleEmitterId = nativeId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetPosition(nativeEffect, particleEmitterId), nativeEffect);
		// TODO: path = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetPath(nativeEffect, particleEmitterId), nativeEffect);
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetSize(nativeEffect, particleEmitterId), nativeEffect);
		orientation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetOrientation(nativeEffect, particleEmitterId), nativeEffect);
		direction = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleEmitterGetDirection(nativeEffect, particleEmitterId), nativeEffect);
		spread = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleEmitterGetSpread(nativeEffect, particleEmitterId), nativeEffect);
	}
}
}