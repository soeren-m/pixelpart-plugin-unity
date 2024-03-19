#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindCollider(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.colliders.containsName(name)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.colliders.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindColliderByIndex(PixelpartNativeEffect* nativeEffect, uint32_t colliderIndex) {
	if(!nativeEffect || colliderIndex >= nativeEffect->project.effect.colliders.getCount()) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.colliders.getByIndex(colliderIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasCollider(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	return nativeEffect && nativeEffect->project.effect.colliders.contains(colliderId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartColliderGetName(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId) || length < 2) {
		return 0;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(collider.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(collider.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, collider.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	collider.lifetimeStart = util::fromUnity(value);
	nativeEffect->particleEngine.refreshCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	collider.lifetimeDuration = util::fromUnity(value);
	nativeEffect->particleEngine.refreshCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	collider.repeat = value;
	nativeEffect->particleEngine.refreshCollisionSolver();
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	return util::toUnity(nativeEffect->project.effect.colliders.get(colliderId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	return util::toUnity(nativeEffect->project.effect.colliders.get(colliderId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return false;
	}

	return nativeEffect->project.effect.colliders.get(colliderId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderIsActive(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return false;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return nativeEffect->particleEngine.getTime() >= collider.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= collider.lifetimeStart + collider.lifetimeDuration || collider.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return util::toUnity(std::fmod(
		nativeEffect->particleEngine.getTime() - collider.lifetimeStart, collider.lifetimeDuration) / collider.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderAddPoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, Vector3 point) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	collider.points.push_back(util::fromUnity(point));
	nativeEffect->particleEngine.refreshCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetPoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, int32_t index, Vector3 point) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points[index] = util::fromUnity(point);
		nativeEffect->particleEngine.refreshCollisionSolver();
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderRemovePoint(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points.erase(collider.points.begin() + index);
		nativeEffect->particleEngine.refreshCollisionSolver();
	}
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointX(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointY(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointZ(PixelpartNativeEffect* nativeEffect, uint32_t colliderId, int32_t index) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].z);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartColliderGetNumPoints(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return 0;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return static_cast<int32_t>(collider.points.size());
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return &collider.width;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return &collider.orientation;
}

// TODO: kill on contact

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetBounce(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return &collider.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetFriction(PixelpartNativeEffect* nativeEffect, uint32_t colliderId) {
	if(!nativeEffect || !nativeEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = nativeEffect->project.effect.colliders.get(colliderId);

	return &collider.friction;
}
}