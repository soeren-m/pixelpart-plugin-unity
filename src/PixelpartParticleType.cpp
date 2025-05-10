#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ParticleType.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartFindParticleType(PixelpartEffectRuntime* effectRuntime, const char* buffer) {
	if(!effectRuntime) {
		return pixelpart::id_t::nullValue;
	}

	std::string name(buffer);
	for(const auto& particleType : effectRuntime->effectAsset.effect().particleTypes()) {
		if(particleType.name() == name) {
			return particleType.id().value();
		}
	}

	return pixelpart::id_t::nullValue;
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartFindParticleTypeByIndex(PixelpartEffectRuntime* effectRuntime, UnityInt particleTypeIndex) {
	if(!effectRuntime || particleTypeIndex < 0 || particleTypeIndex >= effectRuntime->effectAsset.effect().particleTypes().count()) {
		return pixelpart::id_t::nullValue;
	}

	return effectRuntime->effectAsset.effect().particleTypes().atIndex(static_cast<uint32_t>(particleTypeIndex)).id().value();
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartParticleTypeExists(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	return effectRuntime && effectRuntime->effectAsset.effect().particleTypes().contains(particleTypeId);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetIndex(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return -1;
	}

	uint32_t particleTypeIndex = effectRuntime->effectAsset.effect().particleTypes().indexOf(particleTypeId);
	if(particleTypeIndex == pixelpart::id_t::nullValue) {
		return -1;
	}

	return static_cast<UnityInt>(particleTypeIndex);
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartParticleTypeGetParentId(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return pixelpart::id_t::nullValue;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.parentId().value();
	}
	catch(...) {
		return pixelpart::id_t::nullValue;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetName(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, char* buffer, UnityInt length) {
	if(!effectRuntime || length < 2) {
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

		return static_cast<UnityInt>(size);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetPositionRelative(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityBool relative) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.positionRelative(relative);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartParticleTypeIsPositionRelative(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.positionRelative();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationMode(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.rotationMode(static_cast<pixelpart::RotationMode>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetRotationMode(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::RotationMode::angle);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.rotationMode());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::RotationMode::angle);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAlignmentMode(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.alignmentMode(static_cast<pixelpart::AlignmentMode>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetAlignmentMode(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::AlignmentMode::none);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.alignmentMode());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::AlignmentMode::none);
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPivot(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.pivot();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPathForce(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.motionPathForce();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetVisible(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityBool visible) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.visible(visible);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartParticleTypeIsVisible(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.visible();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetLayer(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityInt layer) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		particleType.layer(static_cast<uint32_t>(std::max(layer, static_cast<UnityInt>(0))));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetLayer(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.layer());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialId(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, char* buffer, UnityInt length) {
	if(!effectRuntime || length < 2) {
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

		return static_cast<UnityInt>(size);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartParticleTypeIsMaterialBuiltIn(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return true;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.materialInstance().builtInMaterial();
	}
	catch(...) {
		return true;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterCount(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.materialInstance().materialParameters().size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterIds(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt* parameterIds) {
	if(!effectRuntime) {
		return;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::size_t parameterIndex = 0;
		for(const auto& parameterEntry : particleType.materialInstance().materialParameters()) {
			parameterIds[parameterIndex++] = static_cast<UnityUInt>(parameterEntry.first.value());
		}
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterType(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return -1;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.materialInstance().materialParameters().at(parameterId).type());
	}
	catch(...) {
		return -1;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueBool(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return particleType.materialInstance().materialParameters().at(parameterId).valueBool();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueInt(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return internal::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueInt());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return 0.0f;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return internal::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat());
	}
	catch(...) {
		return 0.0f;
	}
}

UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat2(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return internal::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat2());
	}
	catch(...) {
		return UnityVector2{ 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat3(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return internal::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat3());
	}
	catch(...) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat4(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId) {
	if(!effectRuntime) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return internal::toUnity(particleType.materialInstance().materialParameters().at(parameterId).valueFloat4());
	}
	catch(...) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueResourceId(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, UnityUInt parameterId, char* buffer, UnityInt length) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::string resourceId = particleType.materialInstance().materialParameters().at(parameterId).valueResourceId();

		std::size_t size = std::min(resourceId.size(), static_cast<std::size_t>(length) - 1);
		std::strncpy(buffer, resourceId.c_str(), size);
		buffer[size] = '\0';

		return static_cast<UnityInt>(size);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetRenderer(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleRendererType::sprite);
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return static_cast<UnityInt>(particleType.renderer());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleRendererType::sprite);
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleTypeGetMeshRendererMeshResourceId(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId, char* buffer, UnityInt length) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		std::size_t size = std::min(particleType.meshRendererSettings().meshResourceId.size(), static_cast<std::size_t>(length) - 1);
		std::memcpy(buffer, particleType.meshRendererSettings().meshResourceId.c_str(), size);
		buffer[size] = '\0';

		return static_cast<UnityInt>(size);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetCount(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.count();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespan(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.lifespan();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialSize(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialSize();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialRotation(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialRotation();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialVelocity(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialVelocity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInheritedVelocity(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.inheritedVelocity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialOpacity(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.initialOpacity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPath(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.motionPath();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSize(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.size();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetStretch(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.stretch();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPhysicalSize(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.physicalSize();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotation(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotation();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationBySpeed(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotationBySpeed();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAcceleration(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.acceleration();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRadialAcceleration(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.radialAcceleration();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetWeight(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.weight();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetBounce(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.bounce();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetFriction(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.friction();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColor(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.color();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacity(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.opacity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespanVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.lifespanVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSizeVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.sizeVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.rotationVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.angularVelocityVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetVelocityVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.velocityVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColorVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.colorVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacityVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleType& particleType =
			effectRuntime->effectAsset.effect().particleTypes().at(pixelpart::id_t(particleTypeId));

		return &particleType.opacityVariance();
	}
	catch(...) {
		return nullptr;
	}
}
}