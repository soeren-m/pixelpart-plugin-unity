#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/GroupNode.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include "pixelpart-runtime/effect/AttractionField.h"
#include "pixelpart-runtime/effect/AccelerationField.h"
#include "pixelpart-runtime/effect/VectorField.h"
#include "pixelpart-runtime/effect/NoiseField.h"
#include "pixelpart-runtime/effect/DragField.h"
#include "pixelpart-runtime/effect/LineCollider.h"
#include "pixelpart-runtime/effect/PlaneCollider.h"
#include "pixelpart-runtime/effect/DirectionalLightSource.h"
#include "pixelpart-runtime/effect/PointLightSource.h"
#include "pixelpart-runtime/effect/SpotLightSource.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <exception>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartFindNode(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* buffer) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}

	std::string name(buffer);
	for(const auto& node : effectRuntime->effectAsset.effect().sceneGraph()) {
		if(node->name() == name) {
			return node->id().value();
		}
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartFindNodeByIndex(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t nodeIndex) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}
	else if(nodeIndex < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart::id_t::nullValue;
	}

	try {
		return effectRuntime->effectAsset.effect().sceneGraph().atIndex(static_cast<std::uint32_t>(nodeIndex)).id().value();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartNodeExists(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	return effectRuntime->effectAsset.effect().sceneGraph().contains(nodeId);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartNodeGetType(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	enum class PixelpartNodeType : pixelpart_unity::int_t {
		unknown = -1,
		groupNode = 0,
		particleEmitter = 1,
		attractionField = 2,
		accelerationField = 3,
		vectorField = 4,
		noiseField = 5,
		dragField = 6,
		lineCollider = 7,
		planeCollider = 8,
		directionalLightSource = 9,
		pointLightSource = 10,
		spotLightSource = 11
	};

	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::unknown);
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		if(dynamic_cast<const pixelpart::GroupNode*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::groupNode);
		}
		else if(dynamic_cast<const pixelpart::ParticleEmitter*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::particleEmitter);
		}
		else if(dynamic_cast<const pixelpart::AttractionField*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::attractionField);
		}
		else if(dynamic_cast<const pixelpart::AccelerationField*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::accelerationField);
		}
		else if(dynamic_cast<const pixelpart::VectorField*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::vectorField);
		}
		else if(dynamic_cast<const pixelpart::NoiseField*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::noiseField);
		}
		else if(dynamic_cast<const pixelpart::DragField*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::dragField);
		}
		else if(dynamic_cast<const pixelpart::LineCollider*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::lineCollider);
		}
		else if(dynamic_cast<const pixelpart::PlaneCollider*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::planeCollider);
		}
		else if(dynamic_cast<const pixelpart::DirectionalLightSource*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::directionalLightSource);
		}
		else if(dynamic_cast<const pixelpart::PointLightSource*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::pointLightSource);
		}
		else if(dynamic_cast<const pixelpart::SpotLightSource*>(&node)) {
			return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::spotLightSource);
		}
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(PixelpartNodeType::unknown);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartNodeGetName(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId, char* buffer, pixelpart_unity::int_t length) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(length < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));
		if(node.name().empty()) {
			return 0;
		}

		std::size_t size = std::min(node.name().size(), static_cast<std::size_t>(length) - 1);
		std::memcpy(buffer, node.name().c_str(), size);
		buffer[size] = '\0';

		return static_cast<pixelpart_unity::int_t>(size);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartNodeGetParentId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.parentId().value();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetStart(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId, pixelpart_unity::float_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.start(pixelpart_unity::fromUnity(value));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetDuration(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId, pixelpart_unity::float_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.duration(pixelpart_unity::fromUnity(value));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetRepeat(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId, pixelpart_unity::bool_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.repeat(value);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartNodeGetStart(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return pixelpart_unity::toUnity(node.start());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0.0f;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartNodeGetDuration(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return pixelpart_unity::toUnity(node.duration());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0.0f;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartNodeIsRepeating(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.repeat();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartNodeIsActive(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.active(effectRuntime->effectEngine->runtimeContext());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartNodeGetLocalTime(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return pixelpart_unity::toUnity(node.life(effectRuntime->effectEngine->runtimeContext()));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0.0f;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetPosition(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.position();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetRotation(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.rotation();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetScale(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t nodeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.scale();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}
}
