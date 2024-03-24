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
			return Plugin.PixelpartParticleTypeGetParentId(nativeEffect, particleTypeId);
		}
	}

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartParticleTypeGetName(nativeEffect, particleTypeId, buffer, buffer.Length);

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
			return Plugin.PixelpartParticleTypeGetPositionRelative(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetPositionRelative(nativeEffect, particleTypeId, value);
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
			return (RotationModeType)Plugin.PixelpartParticleTypeGetRotationMode(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetRotationMode(nativeEffect, particleTypeId, (int)value);
		}
	}

	public AlignmentModeType AlignmentMode {
		get {
			return (AlignmentModeType)Plugin.PixelpartParticleTypeGetAlignmentMode(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetAlignmentMode(nativeEffect, particleTypeId, (int)value);
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
			return Plugin.PixelpartParticleTypeIsVisible(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetVisible(nativeEffect, particleTypeId, value);
		}
	}

	public int Layer {
		get {
			return Plugin.PixelpartParticleTypeGetLayer(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetLayer(nativeEffect, particleTypeId, value);
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

	private readonly IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartParticleType(IntPtr nativeParticleTypePtr, uint nativeParticleTypeId) {
		nativeEffect = nativeParticleTypePtr;
		particleTypeId = nativeParticleTypeId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetPosition(nativeEffect, particleTypeId), nativeEffect);
		numParticles = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetNumParticles(nativeEffect, particleTypeId), nativeEffect);
		lifespan = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetLifespan(nativeEffect, particleTypeId), nativeEffect);
		lifespanVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetLifespanVariance(nativeEffect, particleTypeId), nativeEffect);
		motionPathForce = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetMotionPathForce(nativeEffect, particleTypeId), nativeEffect);
		initialVelocity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialVelocity(nativeEffect, particleTypeId), nativeEffect);
		inheritedVelocity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInheritedVelocity(nativeEffect, particleTypeId), nativeEffect);
		velocityVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetVelocityVariance(nativeEffect, particleTypeId), nativeEffect);
		acceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetAcceleration(nativeEffect, particleTypeId), nativeEffect);
		radialAcceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetRadialAcceleration(nativeEffect, particleTypeId), nativeEffect);
		initialRotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetInitialRotation(nativeEffect, particleTypeId), nativeEffect);
		rotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetRotation(nativeEffect, particleTypeId), nativeEffect);
		rotationBySpeed = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetRotationBySpeed(nativeEffect, particleTypeId), nativeEffect);
		rotationVariance = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetRotationVariance(nativeEffect, particleTypeId), nativeEffect);
		angularVelocityVariance = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetAngularVelocityVariance(nativeEffect, particleTypeId), nativeEffect);
		pivot = new PixelpartStaticPropertyFloat3(Plugin.PixelpartParticleTypeGetPivot(nativeEffect, particleTypeId), nativeEffect);
		weight = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetWeight(nativeEffect, particleTypeId), nativeEffect);
		bounce = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetBounce(nativeEffect, particleTypeId), nativeEffect);
		friction = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetFriction(nativeEffect, particleTypeId), nativeEffect);
		initialSize = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialSize(nativeEffect, particleTypeId), nativeEffect);
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetSize(nativeEffect, particleTypeId), nativeEffect);
		sizeVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetSizeVariance(nativeEffect, particleTypeId), nativeEffect);
		stretch = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetStretch(nativeEffect, particleTypeId), nativeEffect);
		color = new PixelpartAnimatedPropertyFloat4(Plugin.PixelpartParticleTypeGetColor(nativeEffect, particleTypeId), nativeEffect);
		colorVariance = new PixelpartStaticPropertyFloat4(Plugin.PixelpartParticleTypeGetColorVariance(nativeEffect, particleTypeId), nativeEffect);
		initialOpacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialOpacity(nativeEffect, particleTypeId), nativeEffect);
		opacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetOpacity(nativeEffect, particleTypeId), nativeEffect);
		opacityVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetOpacityVariance(nativeEffect, particleTypeId), nativeEffect);
	}
}
}