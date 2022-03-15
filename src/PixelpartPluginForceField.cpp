#include "PixelpartPlugin.h"
#include <random>

extern "C" {
static std::mt19937 rng;

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindForceField(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	if(!nativeEffect) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.findForceFieldByName(std::string(buffer));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartForceFieldGetName(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex) || length < 2) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	if(forceField.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(forceField.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, forceField.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.lifetimeStart = value;
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.lifetimeDuration = value;
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.repeat = value;
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getForceField(forceFieldIndex).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getForceField(forceFieldIndex).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return false;
	}

	return nativeEffect->project.effect.getForceField(forceFieldIndex).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartForceFieldIsActive(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return false;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return nativeEffect->particleEngine.getTime() >= forceField.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= forceField.lifetimeStart + forceField.lifetimeDuration || forceField.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - forceField.lifetimeStart, forceField.lifetimeDuration) / forceField.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, int32_t type) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.type = static_cast<pixelpart::ForceField::Type>(type);
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API PixelpartForceFieldGetType(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return static_cast<int>(forceField.type);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetDirectionVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.directionVariance = value;
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetStrengthVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.strengthVariance = value;
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetDirectionVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return static_cast<float>(forceField.directionVariance);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartForceFieldGetStrengthVariance(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 0.0f;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return static_cast<float>(forceField.strengthVariance);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetGridSize(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex, int width, int height) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	forceField.gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
	forceField.gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
	forceField.randomize(rng);
	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API PixelpartForceFieldGetGridWidth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return static_cast<int>(forceField.gridSize[0]);
}

UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API PixelpartForceFieldGetGridHeight(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return 1;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return static_cast<int>(forceField.gridSize[1]);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartForceFieldGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return &forceField.motionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return &forceField.width;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetHeight(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return &forceField.height;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);

	return &forceField.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetDirection(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return &forceField.direction;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(PixelpartNativeEffect* nativeEffect, uint32_t forceFieldIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasForceField(forceFieldIndex)) {
		return nullptr;
	}

	pixelpart::ForceField& forceField = nativeEffect->project.effect.getForceField(forceFieldIndex);
	
	return &forceField.strength;
}
}