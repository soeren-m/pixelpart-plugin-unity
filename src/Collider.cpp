#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/Collider.h"
#include "pixelpart-runtime/effect/LineCollider.h"
#include <cstddef>
#include <string>
#include <exception>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<bool>* UNITY_INTERFACE_API PixelpartColliderGetKillOnContact(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.killOnContact();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetBounce(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.bounce();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartColliderGetFriction(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Collider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::Collider>(colliderId);

		return &collider.friction();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderAddPoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId, pixelpart_unity::vector3_t point) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		collider.points().push_back(pixelpart_unity::fromUnity(point));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderSetPoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId, pixelpart_unity::int_t index, pixelpart_unity::vector3_t point) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		collider.points().at(static_cast<std::size_t>(index)) = pixelpart_unity::fromUnity(point);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartLineColliderRemovePoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId, pixelpart_unity::int_t index) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		collider.points().erase(collider.points().begin() + static_cast<std::size_t>(index));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartLineColliderGetPoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId, pixelpart_unity::int_t index) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		return pixelpart_unity::toUnity(collider.points().at(static_cast<std::size_t>(index)));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartLineColliderGetPointCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t colliderId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		pixelpart::LineCollider& collider =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LineCollider>(colliderId);

		return static_cast<pixelpart_unity::int_t>(collider.points().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}
}