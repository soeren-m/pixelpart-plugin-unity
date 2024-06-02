#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindCollider(PixelpartPluginEffect* internalEffect, const char* buffer) {
	std::string name(buffer);
	if(!internalEffect || !internalEffect->project.effect.colliders.containsName(name)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.colliders.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindColliderByIndex(PixelpartPluginEffect* internalEffect, uint32_t colliderIndex) {
	if(!internalEffect || colliderIndex >= internalEffect->project.effect.colliders.getCount()) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.colliders.getByIndex(colliderIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasCollider(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	return internalEffect && internalEffect->project.effect.colliders.contains(colliderId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartColliderGetName(PixelpartPluginEffect* internalEffect, uint32_t colliderId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId) || length < 2) {
		return 0;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(collider.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(collider.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, collider.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t colliderId, float value) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	collider.lifetimeStart = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t colliderId, float value) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	collider.lifetimeDuration = util::fromUnity(value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetRepeat(PixelpartPluginEffect* internalEffect, uint32_t colliderId, bool value) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	collider.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeStart(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.colliders.get(colliderId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLifetimeDuration(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	return util::toUnity(internalEffect->project.effect.colliders.get(colliderId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderGetRepeat(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return false;
	}

	return internalEffect->project.effect.colliders.get(colliderId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartColliderIsActive(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return false;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return internalEffect->particleEngine->getTime() >= collider.lifetimeStart &&
		(internalEffect->particleEngine->getTime() <= collider.lifetimeStart + collider.lifetimeDuration || collider.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetLocalTime(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return util::toUnity(std::fmod(
		internalEffect->particleEngine->getTime() - collider.lifetimeStart, collider.lifetimeDuration) / collider.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderAddPoint(PixelpartPluginEffect* internalEffect, uint32_t colliderId, Vector3 point) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	collider.points.push_back(util::fromUnity(point));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderSetPoint(PixelpartPluginEffect* internalEffect, uint32_t colliderId, int32_t index, Vector3 point) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points[index] = util::fromUnity(point);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartColliderRemovePoint(PixelpartPluginEffect* internalEffect, uint32_t colliderId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(index >= 0 && index < static_cast<int32_t>(collider.points.size())) {
		collider.points.erase(collider.points.begin() + index);
	}
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointX(PixelpartPluginEffect* internalEffect, uint32_t colliderId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointY(PixelpartPluginEffect* internalEffect, uint32_t colliderId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartColliderGetPointZ(PixelpartPluginEffect* internalEffect, uint32_t colliderId, int32_t index) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0.0f;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);
	if(index < 0 || index >= static_cast<int32_t>(collider.points.size())) {
		return 0.0f;
	}

	return util::toUnity(collider.points[index].z);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartColliderGetNumPoints(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return 0;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return static_cast<int32_t>(collider.points.size());
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetWidth(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return &collider.width;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetOrientation(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return &collider.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<bool>* UNITY_INTERFACE_API PixelpartColliderGetKillOnContact(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return &collider.killOnContact;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetBounce(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return &collider.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetFriction(PixelpartPluginEffect* internalEffect, uint32_t colliderId) {
	if(!internalEffect || !internalEffect->project.effect.colliders.contains(colliderId)) {
		return nullptr;
	}

	pixelpart::Collider& collider = internalEffect->project.effect.colliders.get(colliderId);

	return &collider.friction;
}
}