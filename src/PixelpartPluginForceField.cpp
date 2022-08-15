#include "PixelpartPlugin.h"
#include <random>

extern "C" {
static std::mt19937 rng;

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceField(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.containsWithName(name)) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.forceFields.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceFieldByIndex(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || forceFieldIndex >= nativeEffect->project.effect.forceFields.getCount()) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.forceFields.getByIndex(forceFieldIndex).id;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetName(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId) || length < 2) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	if(forceField.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(forceField.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, forceField.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeStart = value;
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.lifetimeDuration = value;
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.repeat = value;
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.forceFields.get(forceFieldId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.forceFields.get(forceFieldId).lifetimeDuration);
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

	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - forceField.lifetimeStart, forceField.lifetimeDuration) / forceField.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, int32_t type) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.type = static_cast<pixelpart::ForceField::Type>(type);
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API PixelpartForceFieldGetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int>(forceField.type);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetDirectionVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.directionVariance = value;
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetStrengthVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.strengthVariance = value;
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetDirectionVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<float>(forceField.directionVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetStrengthVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<float>(forceField.strengthVariance);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetGridSize(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId, int32_t width, int32_t height) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);
	forceField.gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
	forceField.gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
	forceField.randomize(rng);
	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetGridWidth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.gridSize[0]);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetGridHeight(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return static_cast<int32_t>(forceField.gridSize[1]);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.width;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetHeight(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.height;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartForceFieldGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.motionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetDirection(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldId) {
	if(!nativeEffect || !nativeEffect->project.effect.forceFields.contains(forceFieldId)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.forceFields.get(forceFieldId);

	return &forceField.strength;
}
}