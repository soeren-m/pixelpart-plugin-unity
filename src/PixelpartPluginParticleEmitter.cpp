#include "PixelpartPluginUtil.h"

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

	std::size_t size = std::min(emitter.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, emitter.name.c_str(), size);
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
	emitter.lifetimeStart = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.lifetimeDuration = util::fromUnity(value);
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

	return util::toUnity(nativeEffect->project.effect.particleEmitters.get(emitterId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	return util::toUnity(nativeEffect->project.effect.particleEmitters.get(emitterId).lifetimeDuration);
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

	return util::toUnity(std::fmod(
		nativeEffect->particleEngine.getTime() - emitter.lifetimeStart, emitter.lifetimeDuration) / emitter.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetPosition(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.position;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t shape) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.shape = static_cast<pixelpart::ParticleEmitter::Shape>(shape);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Shape::point);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.shape);
}

// TODO
/*UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetPath(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.path;
}*/

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSize(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.size;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
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

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.distribution);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridOrder(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.gridOrder = static_cast<pixelpart::ParticleEmitter::GridOrder>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridOrder(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridOrder);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridSize(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t width, int32_t height, int32_t depth) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
	emitter.gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
	emitter.gridSize[2] = static_cast<uint32_t>(std::max(depth, 1));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridWidth(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[0]);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridHeight(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[1]);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridDepth(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[2]);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetEmissionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.emissionMode = static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetEmissionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.emissionMode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDirectionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);
	emitter.directionMode = static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDirectionMode(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.directionMode);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(PixelpartNativeEffect* nativeEffect, uint32_t emitterId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.spread;
}
}