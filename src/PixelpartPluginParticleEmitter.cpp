#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitter(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitterWithName(name)) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.getParticleEmitterByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitterByIndex(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.getNumParticleEmitters()) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.getParticleEmitterByIndex(emitterIndex).id;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetName(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId) || length < 2) {
		return 0;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	if(emitter.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(emitter.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, emitter.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetParentId(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.getParticleEmitter(emitterId).parentId;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.lifetimeStart = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.lifetimeDuration = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getParticleEmitter(emitterId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getParticleEmitter(emitterId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return false;
	}

	return nativeEffect->project.effect.getParticleEmitter(emitterId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterIsActive(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return nativeEffect->particleEngine.getTime() >= emitter.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= emitter.lifetimeStart + emitter.lifetimeDuration || emitter.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	
	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - emitter.lifetimeStart, emitter.lifetimeDuration) / emitter.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t type) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.shape = static_cast<pixelpart::ParticleEmitter::Shape>(type);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDistribution(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.distribution = static_cast<pixelpart::ParticleEmitter::Distribution>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetSpawnMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.spawnMode = static_cast<pixelpart::ParticleEmitter::SpawnMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetBurst(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, bool burst) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.burst = burst;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Shape::point);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.shape);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.distribution);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetSpawnMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::SpawnMode::fixed);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.spawnMode);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterGetBurst(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return emitter.burst;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetAlphaThreshold(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float threshold) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.alphaThreshold = threshold;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.blendMode = static_cast<pixelpart::BlendMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetColorMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.colorMode = static_cast<pixelpart::ColorMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLayer(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, uint32_t layer) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.layer = layer;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetVisible(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, bool visible) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.visible = visible;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetAlphaThreshold(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.alphaThreshold);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::BlendMode::normal);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.blendMode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetColorMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ColorMode::multiply);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.colorMode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetRenderer(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::RendererType::sprite);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.renderer);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetLayer(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return emitter.layer;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterIsVisible(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return emitter.visible;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleRotationMode = static_cast<pixelpart::ParticleEmitter::RotationMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::RotationMode::angle);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<int32_t>(emitter.particleRotationMode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleLifespanVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleLifespanVariance = variance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleInitialVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleInitialVelocityVariance = variance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleRotationVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleRotationVariance = variance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleAngularVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleAngularVelocityVariance = variance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleSizeVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleSizeVariance = variance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetParticleOpacityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	emitter.particleOpacityVariance = variance;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleLifespanVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleLifespanVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleInitialVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleInitialVelocityVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleRotationVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleRotationVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleAngularVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleAngularVelocityVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleSizeVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleSizeVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleOpacityVariance(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return static_cast<float>(emitter.particleOpacityVariance);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.width;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetHeight(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.height;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);
	
	return &emitter.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.spread;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.numParticles;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleLifespan(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleLifespan;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.motionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartParticleEmitterParticleGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleMotionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleInitialVelocity(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleInitialVelocity;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleAcceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleRadialAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleRadialAcceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleDamping(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleDamping;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleInitialRotation(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleInitialRotation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleRotation(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleRotation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleWeight(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleWeight;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleBounce(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleBounce;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleFriction(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleFriction;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleInitialSize(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleInitialSize;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleSize(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleSize;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleWidth(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleWidth;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleHeight(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleHeight;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec4d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleColor(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleColor;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleInitialOpacity(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleInitialOpacity;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetParticleOpacity(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.hasParticleEmitter(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitter(emitterId);

	return &emitter.particleOpacity;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSpawnParticles(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, uint32_t count) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.spawnParticles(emitterId, count);
}
}