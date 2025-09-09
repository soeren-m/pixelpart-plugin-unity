#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ParticleType.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <exception>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartFindParticleType(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* buffer) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}
	else if(!buffer) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	std::string name(buffer);
	for(const auto& particleType : effectRuntime->effectAsset.effect().particleTypes()) {
		if(particleType.name() == name) {
			return particleType.id().value();
		}
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartFindParticleTypeByIndex(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t particleTypeIndex) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}
	else if(particleTypeIndex < 0 || particleTypeIndex >= effectRuntime->effectAsset.effect().particleTypes().count()) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart::id_t::nullValue;
	}

	return effectRuntime->effectAsset.effect().particleTypes().atIndex(static_cast<std::uint32_t>(particleTypeIndex)).id().value();
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartParticleTypeExists(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	return effectRuntime->effectAsset.effect().particleTypes().contains(particleTypeId);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetIndex(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return -1;
	}

	std::uint32_t particleTypeIndex = effectRuntime->effectAsset.effect().particleTypes().indexOf(particleTypeId);
	if(particleTypeIndex == pixelpart::id_t::nullValue) {
		return -1;
	}

	return static_cast<pixelpart_unity::int_t>(particleTypeIndex);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartParticleTypeGetParentId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart::id_t::nullValue;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.parentId().value();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetName(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::char_t* buffer, pixelpart_unity::int_t length) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!buffer || length < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(particleTypeId);
		if(particleType.name().empty()) {
			return 0;
		}

		std::size_t size = std::min(particleType.name().size(), static_cast<std::size_t>(length) - 1);
		std::memcpy(buffer, particleType.name().c_str(), size);
		buffer[size] = '\0';

		return static_cast<pixelpart_unity::int_t>(size);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetPositionRelative(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::bool_t relative) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.positionRelative(relative);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartParticleTypeIsPositionRelative(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.positionRelative();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.rotationMode(static_cast<pixelpart::RotationMode>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetRotationMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::RotationMode::angle);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.rotationMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::RotationMode::angle);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAlignmentMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.alignmentMode(static_cast<pixelpart::AlignmentMode>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetAlignmentMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::AlignmentMode::none);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.alignmentMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::AlignmentMode::none);
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPivot(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.pivot();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPathForce(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.motionPathForce();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetVisible(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::bool_t visible) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.visible(visible);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartParticleTypeIsVisible(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.visible();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetLayer(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t layer) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.layer(static_cast<std::uint32_t>(std::max(layer, static_cast<pixelpart_unity::int_t>(0))));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetLayer(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.layer());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::char_t* buffer, pixelpart_unity::int_t length) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!buffer ||length < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType = effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));
		if(particleType.materialInstance().materialId().empty()) {
			return 0;
		}

		std::size_t size = std::min(particleType.materialInstance().materialId().size(), static_cast<std::size_t>(length) - 1);
		std::memcpy(buffer, particleType.materialInstance().materialId().c_str(), size);
		buffer[size] = '\0';

		return static_cast<pixelpart_unity::int_t>(size);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartParticleTypeIsMaterialBuiltIn(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return true;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.materialInstance().builtInMaterial();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return true;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.materialInstance().materialParameters().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterIds(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t* parameterIds) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::size_t parameterIndex = 0;
		for(const auto& parameterEntry : particleType.materialInstance().materialParameters()) {
			parameterIds[parameterIndex++] = static_cast<pixelpart_unity::uint_t>(parameterEntry.first.value());
		}
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterType(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return -1;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.materialInstance().materialParameters().at(parameterId).type());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return -1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueBool(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.materialInstance().materialParameters().at(parameterId).valueBool();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueInt(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return pixelpart_unity::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueInt());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return pixelpart_unity::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0.0f;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector2_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat2(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return pixelpart_unity::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat2());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat3(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return pixelpart_unity::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat3());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector4_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat4(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return pixelpart_unity::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat4());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueResourceId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::uint_t parameterId, pixelpart_unity::char_t* buffer, pixelpart_unity::int_t length) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!buffer || length < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::string resourceId = particleType.materialInstance().materialParameters().at(parameterId).valueResourceId();

		std::size_t size = std::min(resourceId.size(), static_cast<std::size_t>(length) - 1);
		std::strncpy(buffer, resourceId.c_str(), size);
		buffer[size] = '\0';

		return static_cast<pixelpart_unity::int_t>(size);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetRenderer(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleRendererType::sprite);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<pixelpart_unity::int_t>(particleType.renderer());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleRendererType::sprite);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleTypeGetMeshRendererMeshResourceId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::char_t* buffer, pixelpart_unity::int_t length) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!buffer || length < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::size_t size = std::min(particleType.meshRendererSettings().meshResourceId.size(), static_cast<std::size_t>(length) - 1);
		std::memcpy(buffer, particleType.meshRendererSettings().meshResourceId.c_str(), size);
		buffer[size] = '\0';

		return static_cast<pixelpart_unity::int_t>(size);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.count();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespan(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.lifespan();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialSize(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialSize();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialRotation(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialRotation();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialVelocity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialVelocity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInheritedVelocity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.inheritedVelocity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialOpacity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialOpacity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPath(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.motionPath();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSize(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.size();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetStretch(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.stretch();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPhysicalSize(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.physicalSize();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotation(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotation();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationBySpeed(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotationBySpeed();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAcceleration(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.acceleration();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRadialAcceleration(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.radialAcceleration();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetWeight(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.weight();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetBounce(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.bounce();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetFriction(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.friction();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColor(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.color();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.opacity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespanVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.lifespanVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSizeVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.sizeVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotationVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.angularVelocityVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetVelocityVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.velocityVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColorVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.colorVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacityVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.opacityVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}
}
