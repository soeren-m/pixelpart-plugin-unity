#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"

namespace util {
void setParticleEmitterShapePoints(pixelpart::ParticleEmitter& particleEmitter, const std::vector<pixelpart::vec3_t>& points) {
	std::vector<pixelpart::float_t> distances(points.size(), 0.0);
	pixelpart::float_t length = 0.0;

	for(std::size_t i = 1u; i < points.size(); i++) {
		length += std::max(glm::distance(points[i], points[i - 1u]), 0.000001);
		distances[i] = length;
	}

	pixelpart::Curve<pixelpart::vec3_t> modifiedPath;
	for(std::size_t i = 0u; i < points.size(); i++) {
		modifiedPath.addPoint(distances[i] / length, points[i]);
	}

	particleEmitter.path = modifiedPath;
}

std::vector<pixelpart::vec3_t> getParticleEmitterShapePoints(const pixelpart::ParticleEmitter& particleEmitter) {
	std::vector<pixelpart::vec3_t> points(particleEmitter.path.getNumPoints(), pixelpart::vec3_t(0.0));
	for(std::size_t i = 0u; i < particleEmitter.path.getNumPoints(); i++) {
		points[i] = particleEmitter.path.getPoint(i).value;
	}

	return points;
}
}

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitter(PixelpartPluginEffect* internalEffect, const char* buffer) {
	std::string name(buffer);
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.containsName(name)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleEmitters.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleEmitterByIndex(PixelpartPluginEffect* internalEffect, uint32_t emitterIndex) {
	if(!internalEffect || emitterIndex >= internalEffect->project.effect.particleEmitters.getCount()) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleEmitters.getByIndex(emitterIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasParticleEmitter(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	return internalEffect && internalEffect->project.effect.particleEmitters.contains(emitterId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetName(PixelpartPluginEffect* internalEffect, uint32_t emitterId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId) || length < 2) {
		return 0;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	if(emitter.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(emitter.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, emitter.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetParentId(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleEmitters.get(emitterId).parentId;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t emitterId, float value) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.lifetimeStart = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t emitterId, float value) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.lifetimeDuration = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetRepeat(PixelpartPluginEffect* internalEffect, uint32_t emitterId, bool value) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.particleEmitters.get(emitterId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.particleEmitters.get(emitterId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterGetRepeat(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return false;
	}

	return internalEffect->project.effect.particleEmitters.get(emitterId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleEmitterIsActive(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return internalEffect->particleEngine->getTime() >= emitter.lifetimeStart &&
		(internalEffect->particleEngine->getTime() <= emitter.lifetimeStart + emitter.lifetimeDuration || emitter.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleEmitterGetLocalTime(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0.0f;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return util::toUnity(std::fmod(
		internalEffect->particleEngine->getTime() - emitter.lifetimeStart, emitter.lifetimeDuration) / emitter.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetPosition(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.position;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t shape) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.shape = static_cast<pixelpart::ParticleEmitter::Shape>(shape);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Shape::point);
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.shape);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterAddShapePoint(PixelpartPluginEffect* internalEffect, uint32_t emitterId, Vector3 point) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	std::vector<pixelpart::vec3_t> points = util::getParticleEmitterShapePoints(emitter);
	points.push_back(util::fromUnity(point));

	util::setParticleEmitterShapePoints(emitter, points);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterRemoveShapePoint(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	std::vector<pixelpart::vec3_t> points = util::getParticleEmitterShapePoints(emitter);
	if(index < 0 || index >= static_cast<int>(points.size())) {
		return;
	}

	points.erase(points.begin() + index);

	util::setParticleEmitterShapePoints(emitter, points);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShapePoint(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t index, Vector3 point) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	std::vector<pixelpart::vec3_t> points = util::getParticleEmitterShapePoints(emitter);
	if(index < 0 || index >= static_cast<int>(points.size())) {
		return;
	}

	points[index] = util::fromUnity(point);

	util::setParticleEmitterShapePoints(emitter, points);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetNumShapePoints(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 0;
	}

	const pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.path.getNumPoints());
}

UNITY_INTERFACE_EXPORT Vector3 UNITY_INTERFACE_API PixelpartParticleEmitterGetShapePoint(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	const pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	if(index < 0 || index >= static_cast<int32_t>(emitter.path.getNumPoints())) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	return util::toUnity(emitter.path.getPoint(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSize(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.size;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetOrientation(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.orientation;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDistribution(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.distribution = static_cast<pixelpart::ParticleEmitter::Distribution>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.distribution);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridOrder(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.gridOrder = static_cast<pixelpart::ParticleEmitter::GridOrder>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridOrder(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridOrder);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridSize(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t width, int32_t height, int32_t depth) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
	emitter.gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
	emitter.gridSize[2] = static_cast<uint32_t>(std::max(depth, 1));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridWidth(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[0]);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridHeight(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[1]);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridDepth(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return 1;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.gridSize[2]);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetEmissionMode(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.emissionMode = static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetEmissionMode(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.emissionMode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDirectionMode(PixelpartPluginEffect* internalEffect, uint32_t emitterId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);
	emitter.directionMode = static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDirectionMode(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return static_cast<int32_t>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return static_cast<int32_t>(emitter.directionMode);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(PixelpartPluginEffect* internalEffect, uint32_t emitterId) {
	if(!internalEffect || !internalEffect->project.effect.particleEmitters.contains(emitterId)) {
		return nullptr;
	}

	pixelpart::ParticleEmitter& emitter = internalEffect->project.effect.particleEmitters.get(emitterId);

	return &emitter.spread;
}
}