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
		Boundary = 3
	}
	public enum EmissionModeType : int {
		Continuous = 0,
		BurstStart = 1,
		BurstEnd = 2
	}
	public enum DirectionModeType : int {
		Fixed = 0,
		Outwards = 1,
		Inwards = 2
	}

	public uint Id {
		get {
			return particleEmitterId;
		}
	}
	private uint particleEmitterId = 0;

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

	public PixelpartCurve3 Position {
		get {
			return position;
		}
	}
	private PixelpartCurve3 position;

	public ShapeType Shape {
		get {
			return (ShapeType)Plugin.PixelpartParticleEmitterGetShape(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetShape(nativeEffect, particleEmitterId, (int)value);
		}
	}

	public PixelpartCurve3 Path {
		get {
			return path;
		}
	}
	private PixelpartCurve3 path;

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

	public DistributionType Distribution {
		get {
			return (DistributionType)Plugin.PixelpartParticleEmitterGetDistribution(nativeEffect, particleEmitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDistribution(nativeEffect, particleEmitterId, (int)value);
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

	public PixelpartCurve3 Direction {
		get {
			return direction;
		}
	}
	private PixelpartCurve3 direction;

	public PixelpartCurve Spread {
		get {
			return spread;
		}
	}
	private PixelpartCurve spread;

	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartParticleEmitter(IntPtr nativeEffectPtr, uint nativeId) {
		nativeEffect = nativeEffectPtr;
		particleEmitterId = nativeId;

		position = new PixelpartCurve3(Plugin.PixelpartParticleEmitterGetPosition(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve3.ObjectType.ParticleEmitter);
		path = new PixelpartCurve3(Plugin.PixelpartParticleEmitterGetPath(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve3.ObjectType.ParticleEmitter);
		size = new PixelpartCurve3(Plugin.PixelpartParticleEmitterGetSize(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve3.ObjectType.ParticleEmitter);
		orientation = new PixelpartCurve3(Plugin.PixelpartParticleEmitterGetOrientation(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve3.ObjectType.ParticleEmitter);
		direction = new PixelpartCurve3(Plugin.PixelpartParticleEmitterGetDirection(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve3.ObjectType.ParticleEmitter);
		spread = new PixelpartCurve(Plugin.PixelpartParticleEmitterGetSpread(nativeEffect, particleEmitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
}
}