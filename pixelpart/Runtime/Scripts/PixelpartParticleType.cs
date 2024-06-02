using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
public class PixelpartParticleType {
	public uint Id {
		get {
			return particleTypeId;
		}
	}
	private uint particleTypeId = 0;

	public uint ParentId {
		get {
			return Plugin.PixelpartParticleTypeGetParentId(internalEffect, particleTypeId);
		}
	}

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartParticleTypeGetName(internalEffect, particleTypeId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Position {
		get {
			return position;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 position;

	public PixelpartAnimatedPropertyFloat NumParticles {
		get {
			return numParticles;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat numParticles;

	public PixelpartAnimatedPropertyFloat Lifespan {
		get {
			return lifespan;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat lifespan;

	public PixelpartStaticPropertyFloat LifespanVariance {
		get {
			return lifespanVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat lifespanVariance;

	public bool PositionRelative {
		get {
			return Plugin.PixelpartParticleTypeGetPositionRelative(internalEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetPositionRelative(internalEffect, particleTypeId, value);
		}
	}

	public PixelpartStaticPropertyFloat MotionPathForce {
		get {
			return motionPathForce;
		}
	}
	private readonly PixelpartStaticPropertyFloat motionPathForce;

	public PixelpartAnimatedPropertyFloat InitialVelocity {
		get {
			return initialVelocity;
		}
	}
	private PixelpartAnimatedPropertyFloat initialVelocity;

	public PixelpartAnimatedPropertyFloat InheritedVelocity {
		get {
			return inheritedVelocity;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat inheritedVelocity;

	public PixelpartStaticPropertyFloat VelocityVariance {
		get {
			return velocityVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat velocityVariance;

	public PixelpartAnimatedPropertyFloat Acceleration {
		get {
			return acceleration;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat acceleration;

	public PixelpartAnimatedPropertyFloat RadialAcceleration {
		get {
			return radialAcceleration;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat radialAcceleration;

	public RotationModeType RotationMode {
		get {
			return (RotationModeType)Plugin.PixelpartParticleTypeGetRotationMode(internalEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetRotationMode(internalEffect, particleTypeId, (int)value);
		}
	}

	public AlignmentModeType AlignmentMode {
		get {
			return (AlignmentModeType)Plugin.PixelpartParticleTypeGetAlignmentMode(internalEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetAlignmentMode(internalEffect, particleTypeId, (int)value);
		}
	}

	public PixelpartAnimatedPropertyFloat3 InitialRotation {
		get {
			return initialRotation;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 initialRotation;

	public PixelpartAnimatedPropertyFloat3 Rotation {
		get {
			return rotation;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 rotation;

	public PixelpartAnimatedPropertyFloat3 RotationBySpeed {
		get {
			return rotationBySpeed;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 rotationBySpeed;

	public PixelpartStaticPropertyFloat3 RotationVariance {
		get {
			return rotationVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat3 rotationVariance;

	public PixelpartStaticPropertyFloat3 AngularVelocityVariance {
		get {
			return angularVelocityVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat3 angularVelocityVariance;

	public PixelpartStaticPropertyFloat3 Pivot {
		get {
			return pivot;
		}
	}
	private readonly PixelpartStaticPropertyFloat3 pivot;

	public PixelpartAnimatedPropertyFloat PhysicalSize {
		get {
			return physicalSize;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat physicalSize;

	public PixelpartAnimatedPropertyFloat Weight {
		get {
			return weight;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat weight;

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

	public bool Visible {
		get {
			return Plugin.PixelpartParticleTypeIsVisible(internalEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetVisible(internalEffect, particleTypeId, value);
		}
	}

	public int Layer {
		get {
			return Plugin.PixelpartParticleTypeGetLayer(internalEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetLayer(internalEffect, particleTypeId, value);
		}
	}

	public PixelpartAnimatedPropertyFloat InitialSize {
		get {
			return initialSize;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat initialSize;

	public PixelpartAnimatedPropertyFloat3 Size {
		get {
			return size;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 size;

	public PixelpartStaticPropertyFloat SizeVariance {
		get {
			return sizeVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat sizeVariance;

	public PixelpartAnimatedPropertyFloat3 Stretch {
		get {
			return stretch;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 stretch;

	public PixelpartAnimatedPropertyFloat4 Color {
		get {
			return color;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat4 color;

	public PixelpartStaticPropertyFloat4 ColorVariance {
		get {
			return colorVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat4 colorVariance;

	public PixelpartAnimatedPropertyFloat InitialOpacity {
		get {
			return initialOpacity;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat initialOpacity;

	public PixelpartAnimatedPropertyFloat Opacity {
		get {
			return opacity;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat opacity;

	public PixelpartStaticPropertyFloat OpacityVariance {
		get {
			return opacityVariance;
		}
	}
	private readonly PixelpartStaticPropertyFloat opacityVariance;

	private readonly IntPtr internalEffect = IntPtr.Zero;

	public PixelpartParticleType(IntPtr nativeParticleTypePtr, uint nativeParticleTypeId) {
		internalEffect = nativeParticleTypePtr;
		particleTypeId = nativeParticleTypeId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetPosition(internalEffect, particleTypeId));
		numParticles = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetNumParticles(internalEffect, particleTypeId));
		lifespan = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetLifespan(internalEffect, particleTypeId));
		lifespanVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetLifespanVariance(internalEffect, particleTypeId));
		motionPathForce = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetMotionPathForce(internalEffect, particleTypeId));
		initialVelocity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialVelocity(internalEffect, particleTypeId));
		inheritedVelocity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInheritedVelocity(internalEffect, particleTypeId));
		velocityVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetVelocityVariance(internalEffect, particleTypeId));
		acceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetAcceleration(internalEffect, particleTypeId));
		radialAcceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetRadialAcceleration(internalEffect, particleTypeId));
		initialRotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetInitialRotation(internalEffect, particleTypeId));
		rotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetRotation(internalEffect, particleTypeId));
		rotationBySpeed = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetRotationBySpeed(internalEffect, particleTypeId));
		rotationVariance = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetRotationVariance(internalEffect, particleTypeId));
		angularVelocityVariance = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetAngularVelocityVariance(internalEffect, particleTypeId));
		pivot = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetPivot(internalEffect, particleTypeId));
		physicalSize = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetPhysicalSize(internalEffect, particleTypeId));
		weight = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetWeight(internalEffect, particleTypeId));
		bounce = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetBounce(internalEffect, particleTypeId));
		friction = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetFriction(internalEffect, particleTypeId));
		initialSize = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialSize(internalEffect, particleTypeId));
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetSize(internalEffect, particleTypeId));
		sizeVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetSizeVariance(internalEffect, particleTypeId));
		stretch = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetStretch(internalEffect, particleTypeId));
		color = new PixelpartAnimatedPropertyFloat4(Plugin.PixelpartParticleTypeGetColor(internalEffect, particleTypeId));
		colorVariance = new PixelpartStaticPropertyFloat4(Plugin.PixelpartParticleTypeGetColorVariance(internalEffect, particleTypeId));
		initialOpacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialOpacity(internalEffect, particleTypeId));
		opacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetOpacity(internalEffect, particleTypeId));
		opacityVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetOpacityVariance(internalEffect, particleTypeId));
	}
}
}