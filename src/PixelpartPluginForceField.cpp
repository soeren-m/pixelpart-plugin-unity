#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceField(PixelpartPluginEffect* internalEffect, const char* buffer) {
	std::string name(buffer);
	if(!internalEffect || !internalEffect->project.effect.forceFields.containsName(name)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.forceFields.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceFieldByIndex(PixelpartPluginEffect* internalEffect, uint32_t forceFieldIndex) {
	if(!internalEffect || forceFieldIndex >= internalEffect->project.effect.forceFields.getCount()) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.forceFields.getByIndex(forceFieldIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasForceField(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	return internalEffect && internalEffect->project.effect.forceFields.contains(forceFieldId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetName(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId) || length < 2) {
		return 0;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	if(forceField.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(forceField.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, forceField.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, float value) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeStart = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, float value) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeDuration = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetRepeat(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, bool value) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.forceFields.get(forceFieldId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.forceFields.get(forceFieldId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldGetRepeat(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	return internalEffect->project.effect.forceFields.get(forceFieldId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldIsActive(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return internalEffect->particleEngine->getTime() >= forceField.lifetimeStart &&
		(internalEffect->particleEngine->getTime() <= forceField.lifetimeStart + forceField.lifetimeDuration || forceField.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLocalTime(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return util::toUnity(std::fmod(
		internalEffect->particleEngine->getTime() - forceField.lifetimeStart, forceField.lifetimeDuration) / forceField.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetPosition(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.position;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetType(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, int32_t type) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.type = static_cast<pixelpart::ForceField::Type>(type);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetType(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.type);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetSize(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.size;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetOrientation(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.strength;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationDirection(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationDirectionVariance(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.directionVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationStrengthVariance(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.strengthVariance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetAccelerationGridSize(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, int32_t width, int32_t height, int32_t depth) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.accelerationField.gridSize[0] = std::max(width, 1);
	forceField.accelerationField.gridSize[1] = std::max(height, 1);
	forceField.accelerationField.gridSize[2] = std::max(depth, 1);
	forceField.accelerationField.randomizeGrid(internalEffect->rng);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridWidth(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[0];
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridHeight(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[1];
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridDepth(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[2];
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetVectorFilter(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, int32_t filter) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.vectorField.filter = static_cast<pixelpart::ForceField::VectorField::Filter>(filter);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetVectorFilter(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return static_cast<int32_t>(pixelpart::ForceField::VectorField::Filter::none);
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.vectorField.filter);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetVectorTightness(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.vectorField.tightness;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::int_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseOctaves(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.octaves;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseFrequency(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.frequency;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoisePersistence(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.persistence;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseLacunarity(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.lacunarity;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetNoiseAnimated(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId, bool animated) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);
	forceField.noiseField.animated = animated;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimated(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.noiseField.animated;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimationTimeScale(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.animationTimeScale;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimationTimeBase(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.animationTimeBase;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetDragVelocityInfluence(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.dragField.velocityInfluence;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetDragSizeInfluence(PixelpartPluginEffect* internalEffect, uint32_t forceFieldId) {
	if(!internalEffect || !internalEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = internalEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.dragField.sizeInfluence;
}
}