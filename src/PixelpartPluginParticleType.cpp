#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleType(PixelpartPluginEffect* internalEffect, const char* buffer) {
	std::string name(buffer);
	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsName(name)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleTypes.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleTypeByIndex(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex) {
	if(!internalEffect || particleTypeIndex >= internalEffect->project.effect.particleTypes.getCount()) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleTypes.getByIndex(particleTypeIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasParticleType(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	return internalEffect && internalEffect->project.effect.particleTypes.contains(particleTypeId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetName(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId) || length < 2) {
		return 0;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(particleType.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, particleType.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleTypeGetParentId(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return pixelpart::nullId;
	}

	return internalEffect->project.effect.particleTypes.get(particleTypeId).parentId;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPosition(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.position;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetNumParticles(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.numParticles;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespan(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.lifespan;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespanVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.lifespanVariance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetPositionRelative(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, bool relative) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.positionRelative = relative;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeGetPositionRelative(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return false;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return particleType.positionRelative;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPathForce(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.motionPathForce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialVelocity(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialVelocity;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInheritedVelocity(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.inheritedVelocity;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetVelocityVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.velocityVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAcceleration(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.acceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRadialAcceleration(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.radialAcceleration;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationMode(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.rotationMode = static_cast<pixelpart::RotationMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRotationMode(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::RotationMode::angle);
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.rotationMode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAlignmentMode(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, int32_t mode) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.alignmentMode = static_cast<pixelpart::AlignmentMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetAlignmentMode(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::AlignmentMode::none);
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.alignmentMode);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialRotation(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialRotation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotation(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationBySpeed(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotationBySpeed;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotationVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.angularVelocityVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPivot(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.pivot;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPhysicalSize(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.physicalSize;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetWeight(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.weight;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetBounce(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetFriction(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.friction;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetVisible(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, bool visible) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.visible = visible;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeIsVisible(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return false;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return particleType.visible;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetLayer(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, int32_t layer) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.layer = static_cast<uint32_t>(std::max(layer, 0));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetLayer(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.layer);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialSize(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialSize;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSize(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.size;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSizeVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.sizeVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetStretch(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.stretch;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColor(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.color;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColorVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.colorVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialOpacity(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialOpacity;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacity(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.opacity;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacityVariance(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.opacityVariance;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialId(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId) || length < 2) {
		return 0;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialId.empty()) {
		return 0;
	}

	std::size_t size = std::min(particleType.materialInstance.materialId.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, particleType.materialInstance.materialId.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeIsMaterialBuiltIn(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return true;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return particleType.materialInstance.isBuiltInMaterial;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetNumMaterialParameters(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.materialInstance.materialParameters.size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterIds(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t* parameterIds) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	std::size_t parameterIndex = 0u;
	for(const auto& parameterEntry : particleType.materialInstance.materialParameters) {
		parameterIds[parameterIndex++] = static_cast<uint32_t>(parameterEntry.first);
	}
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterType(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return -1;
	}

	pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return -1;
	}

	return particleType.materialInstance.materialParameters.at(parameterId).type;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueBool(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return false;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return false;
	}

	return particleType.materialInstance.materialParameters.at(parameterId).getBool();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueInt(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return 0;
	}

	return util::toUnity(particleType.materialInstance.materialParameters.at(parameterId).getInt());
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return 0.0f;
	}

	return util::toUnity(particleType.materialInstance.materialParameters.at(parameterId).getFloat());
}

UNITY_INTERFACE_EXPORT Vector2 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat2(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return Vector2{ 0.0f, 0.0f };
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return Vector2{ 0.0f, 0.0f };
	}

	return util::toUnity(particleType.materialInstance.materialParameters.at(parameterId).getFloat2());
}

UNITY_INTERFACE_EXPORT Vector3 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat3(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	return util::toUnity(particleType.materialInstance.materialParameters.at(parameterId).getFloat3());
}

UNITY_INTERFACE_EXPORT Vector4 UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueFloat4(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return util::toUnity(particleType.materialInstance.materialParameters.at(parameterId).getFloat4());
}


UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetMaterialParameterValueResourceId(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, uint32_t parameterId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.materialInstance.materialParameters.count(parameterId) == 0u) {
		return 0;
	}

	std::string resourceId = particleType.materialInstance.materialParameters.at(parameterId).getResourceId();

	std::size_t size = std::min(resourceId.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, resourceId.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRenderer(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::ParticleRendererType::sprite);
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.renderer);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetMeshRendererMeshResourceId(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, char* buffer, int32_t length) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.get(particleTypeId);

	std::size_t size = std::min(particleType.meshRendererSettings.meshResourceId.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, particleType.meshRendererSettings.meshResourceId.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}
}