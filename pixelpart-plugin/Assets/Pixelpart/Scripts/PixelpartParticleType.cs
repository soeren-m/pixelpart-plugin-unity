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
	private PixelpartAnimatedPropertyFloat3 position;

	public PixelpartAnimatedPropertyFloat NumParticles {
		get {
			return numParticles;
		}
	}
	private PixelpartAnimatedPropertyFloat numParticles;

	public PixelpartAnimatedPropertyFloat Lifespan {
		get {
			return lifespan;
		}
	}
	private PixelpartAnimatedPropertyFloat lifespan;

	public PixelpartStaticPropertyFloat LifespanVariance {
		get {
			return lifespanVariance;
		}
	}
	private PixelpartStaticPropertyFloat lifespanVariance;

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
	private PixelpartStaticPropertyFloat motionPathForce;

	public PixelpartAnimatedPropertyFloat InitialVelocity {
		get {
			return initialVelocity;
		}
	}
	private PixelpartAnimatedPropertyFloat initialVelocity;

	// TODO

	public float VelocityVariance {
		get {
			return Plugin.PixelpartParticleTypeGetVelocityVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetVelocityVariance(nativeEffect, particleTypeId, value);
		}
	}

	public PixelpartAnimatedPropertyFloat Acceleration {
		get {
			return acceleration;
		}
	}
	private PixelpartAnimatedPropertyFloat acceleration;

	public PixelpartAnimatedPropertyFloat RadialAcceleration {
		get {
			return radialAcceleration;
		}
	}
	private PixelpartAnimatedPropertyFloat radialAcceleration;

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
	private PixelpartAnimatedPropertyFloat3 initialRotation;

	public PixelpartAnimatedPropertyFloat3 Rotation {
		get {
			return rotation;
		}
	}
	private PixelpartAnimatedPropertyFloat3 rotation;

	public Vector3 RotationVariance {
		get {
			return new Vector3(
				Plugin.PixelpartParticleTypeGetRotationVarianceRoll(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetRotationVarianceYaw(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetRotationVariancePitch(nativeEffect, particleTypeId));
		}
		set {
			Plugin.PixelpartParticleTypeSetRotationVariance(nativeEffect, particleTypeId, value.x, value.y, value.z);
		}
	}

	public Vector3 AngularVelocityVariance {
		get {
			return new Vector3(
				Plugin.PixelpartParticleTypeGetAngularVelocityVarianceRoll(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetAngularVelocityVarianceYaw(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetAngularVelocityVariancePitch(nativeEffect, particleTypeId));
		}
		set {
			Plugin.PixelpartParticleTypeSetAngularVelocityVariance(nativeEffect, particleTypeId, value.x, value.y, value.z);
		}
	}

	public Vector3 Pivot {
		get {
			return new Vector3(
				Plugin.PixelpartParticleTypeGetPivotX(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetPivotY(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetPivotZ(nativeEffect, particleTypeId));
		}
		set {
			Plugin.PixelpartParticleTypeSetPivot(nativeEffect, particleTypeId, value.x, value.y, value.z);
		}
	}

	public PixelpartAnimatedPropertyFloat Weight {
		get {
			return weight;
		}
	}
	private PixelpartAnimatedPropertyFloat weight;

	public PixelpartAnimatedPropertyFloat Bounce {
		get {
			return bounce;
		}
	}
	private PixelpartAnimatedPropertyFloat bounce;

	public PixelpartAnimatedPropertyFloat Friction {
		get {
			return friction;
		}
	}
	private PixelpartAnimatedPropertyFloat friction;

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

	public BlendModeType BlendMode {
		get {
			return (BlendModeType)Plugin.PixelpartParticleTypeGetBlendMode(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetBlendMode(nativeEffect, particleTypeId, (int)value);
		}
	}

	public PixelpartAnimatedPropertyFloat InitialSize {
		get {
			return initialSize;
		}
	}
	private PixelpartAnimatedPropertyFloat initialSize;

	public PixelpartAnimatedPropertyFloat3 Size {
		get {
			return size;
		}
	}
	private PixelpartAnimatedPropertyFloat3 size;

	public float SizeVariance {
		get {
			return Plugin.PixelpartParticleTypeGetSizeVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetSizeVariance(nativeEffect, particleTypeId, value);
		}
	}

	public PixelpartAnimatedPropertyFloat4 Color {
		get {
			return color;
		}
	}
	private PixelpartAnimatedPropertyFloat4 color;

	public Vector3 ColorVariance {
		get {
			return new Vector3(
				Plugin.PixelpartParticleTypeGetColorVarianceHue(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetColorVarianceSaturation(nativeEffect, particleTypeId),
				Plugin.PixelpartParticleTypeGetColorVarianceValue(nativeEffect, particleTypeId));
		}
		set {
			Plugin.PixelpartParticleTypeSetColorVariance(nativeEffect, particleTypeId, value.x, value.y, value.z);
		}
	}

	public PixelpartAnimatedPropertyFloat InitialOpacity {
		get {
			return initialOpacity;
		}
	}
	private PixelpartAnimatedPropertyFloat initialOpacity;

	public PixelpartAnimatedPropertyFloat Opacity {
		get {
			return opacity;
		}
	}
	private PixelpartAnimatedPropertyFloat opacity;

	public float OpacityVariance {
		get {
			return Plugin.PixelpartParticleTypeGetOpacityVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetOpacityVariance(nativeEffect, particleTypeId, value);
		}
	}

	private IntPtr nativeEffect = IntPtr.Zero;

	public PixelpartParticleType(IntPtr nativeParticleTypePtr, uint nativeParticleTypeId) {
		nativeEffect = nativeParticleTypePtr;
		particleTypeId = nativeParticleTypeId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetPosition(nativeEffect, particleTypeId), nativeEffect);
		numParticles = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetNumParticles(nativeEffect, particleTypeId), nativeEffect);
		lifespan = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetLifespan(nativeEffect, particleTypeId), nativeEffect);
		lifespanVariance = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetLifespanVariance(nativeEffect, particleTypeId), nativeEffect);
		motionPathForce = new PixelpartStaticPropertyFloat(Plugin.PixelpartParticleTypeGetMotionPathForce(nativeEffect, particleTypeId), nativeEffect);
		initialVelocity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialVelocity(nativeEffect, particleTypeId), nativeEffect);
		acceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetAcceleration(nativeEffect, particleTypeId), nativeEffect);
		radialAcceleration = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetRadialAcceleration(nativeEffect, particleTypeId), nativeEffect);
		initialRotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetInitialRotation(nativeEffect, particleTypeId), nativeEffect);
		rotation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetRotation(nativeEffect, particleTypeId), nativeEffect);
		weight = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetWeight(nativeEffect, particleTypeId), nativeEffect);
		bounce = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetBounce(nativeEffect, particleTypeId), nativeEffect);
		friction = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetFriction(nativeEffect, particleTypeId), nativeEffect);
		initialSize = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialSize(nativeEffect, particleTypeId), nativeEffect);
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartParticleTypeGetSize(nativeEffect, particleTypeId), nativeEffect);
		color = new PixelpartAnimatedPropertyFloat4(Plugin.PixelpartParticleTypeGetColor(nativeEffect, particleTypeId), nativeEffect);
		initialOpacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetInitialOpacity(nativeEffect, particleTypeId), nativeEffect);
		opacity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartParticleTypeGetOpacity(nativeEffect, particleTypeId), nativeEffect);
	}

	public void SpawnParticles(int count) {
		Plugin.PixelpartParticleTypeSpawnParticles(nativeEffect, particleTypeId, count);
	}
}
}