#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/Collider.h"
#include "pixelpart-runtime/effect/LineCollider.h"

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<bool>* UNITY_INTERFACE_API PixelpartColliderGetKillOnContact(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.killOnContact();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetBounce(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.bounce();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetFriction(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.friction();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderAddPoint(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId, UnityVector3 point) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		collider.points().push_back(internal::fromUnity(point));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderSetPoint(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId, UnityInt index, UnityVector3 point) {
	if(!effectRuntime || index < 0) {
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		collider.points().at(index) = internal::fromUnity(point);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderRemovePoint(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId, UnityInt index) {
	if(!effectRuntime || index < 0) {
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		if(index < collider.points().size()) {
			return;
		}

		collider.points().erase(collider.points().begin() + index);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartLineColliderGetPoint(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId, UnityInt index) {
	if(!effectRuntime || index < 0) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		return internal::toUnity(collider.points().at(index));
	}
	catch(...) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartLineColliderGetPointCount(PixelpartEffectRuntime* effectRuntime, UnityUInt colliderId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		return static_cast<UnityInt>(collider.points().size());
	}
	catch(...) {
		return 0;
	}
}
}