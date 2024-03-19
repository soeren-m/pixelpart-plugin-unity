#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceField(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.containsName(name)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.forceFields.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceFieldByIndex(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || forceFieldIndex >= nativeEffect->project.effect.forceFields.getCount()) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.forceFields.getByIndex(forceFieldIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasForceField(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	return nativeEffect && nativeEffect->project.effect.forceFields.contains(forceFieldId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetName(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId) || length < 2) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	if(forceField.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(forceField.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, forceField.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeStart = util::fromUnity(value);
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeDuration = util::fromUnity(value);
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.repeat = value;
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return util::toUnity(nativeEffect->project.effect.forceFields.get(forceFieldId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return util::toUnity(nativeEffect->project.effect.forceFields.get(forceFieldId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	return nativeEffect->project.effect.forceFields.get(forceFieldId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldIsActive(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return nativeEffect->particleEngine.getTime() >= forceField.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= forceField.lifetimeStart + forceField.lifetimeDuration || forceField.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return util::toUnity(std::fmod(
		nativeEffect->particleEngine.getTime() - forceField.lifetimeStart, forceField.lifetimeDuration) / forceField.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetPosition(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.position;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, int32_t type) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.type = static_cast<pixelpart::ForceField::Type>(type);
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.type);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetSize(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.size;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.strength;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationDirection(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationDirectionVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.directionVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationStrengthVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.accelerationField.strengthVariance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetAccelerationGridSize(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, int32_t width, int32_t height, int32_t depth) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.accelerationField.gridSize[0] = std::max(width, 1);
	forceField.accelerationField.gridSize[1] = std::max(height, 1);
	forceField.accelerationField.gridSize[2] = std::max(depth, 1);
	forceField.accelerationField.randomizeGrid(nativeEffect->rng);
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridWidth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[0];
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridHeight(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[1];
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetAccelerationGridDepth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.accelerationField.gridSize[2];
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetVectorFilter(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, int32_t filter) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.vectorField.filter = static_cast<pixelpart::ForceField::VectorField::Filter>(filter);
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetVectorFilter(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return static_cast<int32_t>(pixelpart::ForceField::VectorField::Filter::none);
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.vectorField.filter);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetVectorTightness(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.vectorField.tightness;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::int_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseOctaves(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.octaves;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseFrequency(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.frequency;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoisePersistence(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.persistence;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseLacunarity(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.lacunarity;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetNoiseAnimated(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, bool animated) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.noiseField.animated = animated;
	nativeEffect->particleEngine.refreshForceSolver();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimated(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return false;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return forceField.noiseField.animated;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimationTimeScale(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.animationTimeScale;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetNoiseAnimationTimeBase(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.noiseField.animationTimeBase;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetDragVelocityInfluence(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.dragField.velocityInfluence;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetDragSizeInfluence(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.dragField.sizeInfluence;
}
}