#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitter(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.containsName(name)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleEmitters.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitterByIndex(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.particleEmitters.getCount()) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleEmitters.getByIndex(emitterIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasParticleEmitter(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	return nativeEffect && nativeEffect->project.effect.particleEmitters.contains(emitterId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetName(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId) || length < 2) {
		return 0;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	if(emitter.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(emitter.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, emitter.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetParentId(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleEmitters.get(emitterId).parentId;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.lifetimeStart = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.lifetimeDuration = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.particleEmitters.get(emitterId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.particleEmitters.get(emitterId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return false;
	}

	return nativeEffect->project.effect.particleEmitters.get(emitterId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterIsActive(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return nativeEffect->particleEngine.getTime() >= emitter.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= emitter.lifetimeStart + emitter.lifetimeDuration || emitter.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - emitter.lifetimeStart, emitter.lifetimeDuration) / emitter.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetPosition(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.position;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t type) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.shape = static_cast<pixelpart::ParticleEmitter::Shape>(type);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Shape::point);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.shape);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetPath(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.path;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSize(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.size;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.orientation;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDistribution(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.distribution = static_cast<pixelpart::ParticleEmitter::Distribution>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetEmissionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.emissionMode = static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDirectionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.directionMode = static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.distribution);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetEmissionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.emissionMode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDirectionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.directionMode);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.spread;
}
}