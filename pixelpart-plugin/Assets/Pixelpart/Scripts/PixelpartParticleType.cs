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

	public PixelpartCurve3 Position {
		get {
			return position;
		}
	}
	private PixelpartCurve3 position;

	public PixelpartCurve NumParticles {
		get {
			return numParticles;
		}
	}
	private PixelpartCurve numParticles;

	public PixelpartCurve Lifespan {
		get {
			return lifespan;
		}
	}
	private PixelpartCurve lifespan;

	public float LifespanVariance {
		get {
			return Plugin.PixelpartParticleTypeGetLifespanVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetLifespanVariance(nativeEffect, particleTypeId, value);
		}
	}

	public bool PositionRelative {
		get {
			return Plugin.PixelpartParticleTypeGetPositionRelative(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetPositionRelative(nativeEffect, particleTypeId, value);
		}
	}

	public float MotionPathForce {
		get {
			return Plugin.PixelpartParticleTypeGetMotionPathForce(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetMotionPathForce(nativeEffect, particleTypeId, value);
		}
	}

	public PixelpartCurve InitialVelocity {
		get {
			return initialVelocity;
		}
	}
	private PixelpartCurve initialVelocity;

	public float VelocityVariance {
		get {
			return Plugin.PixelpartParticleTypeGetVelocityVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetVelocityVariance(nativeEffect, particleTypeId, value);
		}
	}

	public PixelpartCurve Acceleration {
		get {
			return acceleration;
		}
	}
	private PixelpartCurve acceleration;

	public PixelpartCurve RadialAcceleration {
		get {
			return radialAcceleration;
		}
	}
	private PixelpartCurve radialAcceleration;

	public PixelpartCurve Damping {
		get {
			return damping;
		}
	}
	private PixelpartCurve damping;

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

	public PixelpartCurve3 InitialRotation {
		get {
			return initialRotation;
		}
	}
	private PixelpartCurve3 initialRotation;

	public PixelpartCurve3 Rotation {
		get {
			return rotation;
		}
	}
	private PixelpartCurve3 rotation;

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

	public PixelpartCurve Weight {
		get {
			return weight;
		}
	}
	private PixelpartCurve weight;

	public PixelpartCurve Bounce {
		get {
			return bounce;
		}
	}
	private PixelpartCurve bounce;

	public PixelpartCurve Friction {
		get {
			return friction;
		}
	}
	private PixelpartCurve friction;

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

	public PixelpartCurve InitialSize {
		get {
			return initialSize;
		}
	}
	private PixelpartCurve initialSize;

	public PixelpartCurve3 Size {
		get {
			return size;
		}
	}
	private PixelpartCurve3 size;

	public float SizeVariance {
		get {
			return Plugin.PixelpartParticleTypeGetSizeVariance(nativeEffect, particleTypeId);
		}
		set {
			Plugin.PixelpartParticleTypeSetSizeVariance(nativeEffect, particleTypeId, value);
		}
	}

	public PixelpartGradient Color {
		get {
			return color;
		}
	}
	private PixelpartGradient color;

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

	public PixelpartCurve InitialOpacity {
		get {
			return initialOpacity;
		}
	}
	private PixelpartCurve initialOpacity;

	public PixelpartCurve Opacity {
		get {
			return opacity;
		}
	}
	private PixelpartCurve opacity;

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

		position = new PixelpartCurve3(Plugin.PixelpartParticleTypeGetPosition(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve3.ObjectType.ParticleType);
		numParticles = new PixelpartCurve(Plugin.PixelpartParticleTypeGetNumParticles(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		lifespan = new PixelpartCurve(Plugin.PixelpartParticleTypeGetLifespan(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		initialVelocity = new PixelpartCurve(Plugin.PixelpartParticleTypeGetInitialVelocity(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		acceleration = new PixelpartCurve(Plugin.PixelpartParticleTypeGetAcceleration(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		radialAcceleration = new PixelpartCurve(Plugin.PixelpartParticleTypeGetRadialAcceleration(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		damping = new PixelpartCurve(Plugin.PixelpartParticleTypeGetDamping(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		initialRotation = new PixelpartCurve3(Plugin.PixelpartParticleTypeGetInitialRotation(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve3.ObjectType.ParticleType);
		rotation = new PixelpartCurve3(Plugin.PixelpartParticleTypeGetRotation(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve3.ObjectType.ParticleType);
		weight = new PixelpartCurve(Plugin.PixelpartParticleTypeGetWeight(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		bounce = new PixelpartCurve(Plugin.PixelpartParticleTypeGetBounce(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		friction = new PixelpartCurve(Plugin.PixelpartParticleTypeGetFriction(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		initialSize = new PixelpartCurve(Plugin.PixelpartParticleTypeGetInitialSize(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		size = new PixelpartCurve3(Plugin.PixelpartParticleTypeGetSize(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve3.ObjectType.ParticleType);
		color = new PixelpartGradient(Plugin.PixelpartParticleTypeGetColor(nativeEffect, particleTypeId), nativeEffect, PixelpartGradient.ObjectType.ParticleType);
		initialOpacity = new PixelpartCurve(Plugin.PixelpartParticleTypeGetInitialOpacity(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
		opacity = new PixelpartCurve(Plugin.PixelpartParticleTypeGetOpacity(nativeEffect, particleTypeId), nativeEffect, PixelpartCurve.ObjectType.ParticleType);
	}

	public void SpawnParticles(int count) {
		Plugin.PixelpartParticleTypeSpawnParticles(nativeEffect, particleTypeId, count);
	}
}
}