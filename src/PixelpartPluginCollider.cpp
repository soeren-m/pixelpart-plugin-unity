#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindCollider(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.colliders.containsWithName(name)) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.colliders.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindColliderByIndex(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || colliderIndex >= nativeEffect->project.effect.colliders.getCount()) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.colliders.getByIndex(colliderIndex).id;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartColliderGetName(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex) || length < 2) {
		return 0;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	if(collider.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(collider.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, collider.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	collider.lifetimeStart = value;
	nativeEffect->particleEngine.updateCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	collider.lifetimeDuration = value;
	nativeEffect->particleEngine.updateCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	collider.repeat = value;
	nativeEffect->particleEngine.updateCollisionSolver();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.colliders.get(colliderIndex).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.colliders.get(colliderIndex).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return false;
	}

	return nativeEffect->project.effect.colliders.get(colliderIndex).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderIsActive(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return false;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);

	return nativeEffect->particleEngine.getTime() >= collider.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= collider.lifetimeStart + collider.lifetimeDuration || collider.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);

	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - collider.lifetimeStart, collider.lifetimeDuration) / collider.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderAddPoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, float x, float y) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	collider.points.push_back(pixelpart::vec2d(x, y));
	nativeEffect->particleEngine.updateCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetPoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, int32_t index, float x, float y) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points[index] = pixelpart::vec2d(x, y);
		nativeEffect->particleEngine.updateCollisionSolver();
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderRemovePoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points.erase(collider.points.begin() + index);
		nativeEffect->particleEngine.updateCollisionSolver();
	}
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointX(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return static_cast<float>(collider.points[index].x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointY(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return static_cast<float>(collider.points[index].y);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartColliderGetNumPoints(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return 0;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);

	return static_cast<uint32_t>(collider.points.size());
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartColliderGetBounce(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);

	return &collider.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartColliderGetFriction(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderIndex)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderIndex);

	return &collider.friction;
}
}