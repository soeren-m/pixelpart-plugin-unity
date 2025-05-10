#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
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
#include <cstring>
#include <string>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartFindNode(PixelpartEffectRuntime* effectRuntime, const char* buffer) {
	if(!effectRuntime) {
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

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartFindNodeByIndex(PixelpartEffectRuntime* effectRuntime, UnityInt nodeIndex) {
	if(!effectRuntime || nodeIndex < 0 || nodeIndex >= effectRuntime->effectAsset.effect().sceneGraph().count()) {
		return pixelpart::id_t::nullValue;
	}

	return effectRuntime->effectAsset.effect().sceneGraph().atIndex(static_cast<uint32_t>(nodeIndex)).id().value();
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartNodeExists(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	return effectRuntime && effectRuntime->effectAsset.effect().sceneGraph().contains(nodeId);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartNodeGetType(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	enum class PixelpartNodeType : UnityInt {
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
		return static_cast<UnityInt>(PixelpartNodeType::unknown);
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		if(dynamic_cast<const pixelpart::GroupNode*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::groupNode);
		}
		else if(dynamic_cast<const pixelpart::ParticleEmitter*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::particleEmitter);
		}
		else if(dynamic_cast<const pixelpart::AttractionField*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::attractionField);
		}
		else if(dynamic_cast<const pixelpart::AccelerationField*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::accelerationField);
		}
		else if(dynamic_cast<const pixelpart::VectorField*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::vectorField);
		}
		else if(dynamic_cast<const pixelpart::NoiseField*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::noiseField);
		}
		else if(dynamic_cast<const pixelpart::DragField*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::dragField);
		}
		else if(dynamic_cast<const pixelpart::LineCollider*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::lineCollider);
		}
		else if(dynamic_cast<const pixelpart::PlaneCollider*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::planeCollider);
		}
		else if(dynamic_cast<const pixelpart::DirectionalLightSource*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::directionalLightSource);
		}
		else if(dynamic_cast<const pixelpart::PointLightSource*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::pointLightSource);
		}
		else if(dynamic_cast<const pixelpart::SpotLightSource*>(&node)) {
			return static_cast<UnityInt>(PixelpartNodeType::spotLightSource);
		}

		return static_cast<UnityInt>(PixelpartNodeType::unknown);
	}
	catch(...) {
		return static_cast<UnityInt>(PixelpartNodeType::unknown);
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartNodeGetName(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId, char* buffer, UnityInt length) {
	if(!effectRuntime || length < 2) {
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

		return static_cast<UnityInt>(size);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartNodeGetParentId(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return pixelpart::id_t::nullValue;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.parentId().value();
	}
	catch(...) {
		return pixelpart::id_t::nullValue;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetStart(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId, UnityFloat value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.start(internal::fromUnity(value));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetDuration(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId, UnityFloat value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.duration(internal::fromUnity(value));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNodeSetRepeat(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId, UnityBool value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		node.repeat(value);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartNodeGetStart(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return internal::toUnity(node.start());
	}
	catch(...) {
		return 0.0f;
	}
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartNodeGetDuration(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return internal::toUnity(node.duration());
	}
	catch(...) {
		return 0.0f;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartNodeIsRepeating(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.repeat();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartNodeIsActive(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return node.active(effectRuntime->effectEngine->runtimeContext());
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartNodeGetLocalTime(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return 0.0f;
	}

	try {
		const pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return internal::toUnity(node.life(effectRuntime->effectEngine->runtimeContext()));
	}
	catch(...) {
		return 0.0f;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetPosition(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.position();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetOrientation(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.orientation();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartNodeGetSize(PixelpartEffectRuntime* effectRuntime, UnityUInt nodeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::Node& node =
			effectRuntime->effectAsset.effect().sceneGraph().at(pixelpart::id_t(nodeId));

		return &node.size();
	}
	catch(...) {
		return nullptr;
	}
}
}