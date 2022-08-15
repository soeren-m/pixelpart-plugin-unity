using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
public class PixelpartParticleEmitter {
	private IntPtr nativeEffect = IntPtr.Zero;
	private uint emitterId = 0;

	public float LifetimeStart {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeStart(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeStart(nativeEffect, emitterId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartParticleEmitterGetLifetimeDuration(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLifetimeDuration(nativeEffect, emitterId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartParticleEmitterGetRepeat(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetRepeat(nativeEffect, emitterId, value);
		}
	}

	public EmitterShapeType Shape {
		get {
			return (EmitterShapeType)Plugin.PixelpartParticleEmitterGetShape(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetShape(nativeEffect, emitterId, (int)value);
		}
	}
	public EmitterDistributionType Distribution {
		get {
			return (EmitterDistributionType)Plugin.PixelpartParticleEmitterGetDistribution(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetDistribution(nativeEffect, emitterId, (int)value);
		}
	}
	public EmitterSpawnMode SpawnMode {
		get {
			return (EmitterSpawnMode)Plugin.PixelpartParticleEmitterGetSpawnMode(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetSpawnMode(nativeEffect, emitterId, (int)value);
		}
	}
	public EmitterInstantiationMode InstantiationMode {
		get {
			return (EmitterInstantiationMode)Plugin.PixelpartParticleEmitterGetInstantiationMode(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetInstantiationMode(nativeEffect, emitterId, (int)value);
		}
	}

	public uint Layer {
		get {
			return Plugin.PixelpartParticleEmitterGetLayer(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetLayer(nativeEffect, emitterId, value);
		}
	}
	public bool Visible {
		get {
			return Plugin.PixelpartParticleEmitterIsVisible(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetVisible(nativeEffect, emitterId, value);
		}
	}

	public RotationModeType ParticleRotationMode {
		get {
			return (RotationModeType)Plugin.PixelpartParticleEmitterGetParticleRotationMode(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleRotationMode(nativeEffect, emitterId, (int)value);
		}
	}
	public Vector2 ParticlePivot {
		get {
			return new Vector2(
				Plugin.PixelpartParticleEmitterGetParticlePivotX(nativeEffect, emitterId),
				Plugin.PixelpartParticleEmitterGetParticlePivotY(nativeEffect, emitterId));
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticlePivot(nativeEffect, emitterId, value.x, value.y);
		}
	}

	public float ParticleLifespanVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleLifespanVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleLifespanVariance(nativeEffect, emitterId, value);
		}
	}
	public float ParticleInitialVelocityVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleInitialVelocityVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleInitialVelocityVariance(nativeEffect, emitterId, value);
		}
	}
	public float ParticleRotationVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleRotationVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleRotationVariance(nativeEffect, emitterId, value);
		}
	}
	public float ParticleAngularVelocityVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleAngularVelocityVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleAngularVelocityVariance(nativeEffect, emitterId, value);
		}
	}
	public float ParticleSizeVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleSizeVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleSizeVariance(nativeEffect, emitterId, value);
		}
	}
	public float ParticleOpacityVariance {
		get {
			return Plugin.PixelpartParticleEmitterGetParticleOpacityVariance(nativeEffect, emitterId);
		}
		set {
			Plugin.PixelpartParticleEmitterSetParticleOpacityVariance(nativeEffect, emitterId, value);
		}
	}

	public PixelpartParticleEmitter(IntPtr nativePtr, uint nativeId) {
		nativeEffect = nativePtr;
		emitterId = nativeId;
	}

	public string GetName() {
		byte[] buffer = new byte[256];
		int size = Plugin.PixelpartParticleEmitterGetName(nativeEffect, emitterId, buffer, buffer.Length);

		return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
	}
	public uint GetId() {
		return emitterId;
	}
	public uint GetParentId() {
		return Plugin.PixelpartParticleEmitterGetParentId(nativeEffect, emitterId);
	}

	public bool IsActive() {
		return Plugin.PixelpartParticleEmitterIsActive(nativeEffect, emitterId);
	}
	public float GetLocalTime() {
		return Plugin.PixelpartParticleEmitterGetLocalTime(nativeEffect, emitterId);
	}

	public void SpawnParticles(int count) {
		if(count > 0) {
			Plugin.PixelpartParticleEmitterSpawnParticles(nativeEffect, emitterId, (uint)count);
		}
	}

	public PixelpartPath GetShapePath() {
		return new PixelpartPath(Plugin.PixelpartParticleEmitterGetShapePath(nativeEffect, emitterId), nativeEffect, PixelpartPath.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetWidth() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetWidth(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetHeight() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetHeight(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetOrientation() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetOrientation(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetDirection() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetDirection(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetSpread() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetSpread(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetNumParticles() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetNumParticles(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleLifespan() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleLifespan(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartPath GetMotionPath() {
		return new PixelpartPath(Plugin.PixelpartParticleEmitterGetMotionPath(nativeEffect, emitterId), nativeEffect, PixelpartPath.ObjectType.ParticleEmitter);
	}
	public PixelpartPath GetParticleMotionPath() {
		return new PixelpartPath(Plugin.PixelpartParticleEmitterGetParticleMotionPath(nativeEffect, emitterId), nativeEffect, PixelpartPath.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleInitialVelocity() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleInitialVelocity(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleAcceleration() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleAcceleration(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleRadialAcceleration() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleRadialAcceleration(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleDamping() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleDamping(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleInitialRotation() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleInitialRotation(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleRotation() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleRotation(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleWeight() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleWeight(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleBounce() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleBounce(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleFriction() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleFriction(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleInitialSize() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleInitialSize(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleSize() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleSize(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleWidth() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleWidth(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleHeight() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleHeight(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartGradient GetParticleColor() {
		return new PixelpartGradient(Plugin.PixelpartParticleEmitterGetParticleColor(nativeEffect, emitterId), nativeEffect, PixelpartGradient.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleInitialOpacity() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleInitialOpacity(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
	public PixelpartCurve GetParticleOpacity() {
		return new PixelpartCurve(Plugin.PixelpartParticleEmitterGetParticleOpacity(nativeEffect, emitterId), nativeEffect, PixelpartCurve.ObjectType.ParticleEmitter);
	}
}
}