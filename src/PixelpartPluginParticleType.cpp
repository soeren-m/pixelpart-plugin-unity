#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleType(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.containsName(name)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleTypes.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindParticleTypeByIndex(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex) {
	if(!nativeEffect || particleTypeIndex >= nativeEffect->project.effect.particleTypes.getCount()) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleTypes.getByIndex(particleTypeIndex).id;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartHasParticleType(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	return nativeEffect && nativeEffect->project.effect.particleTypes.contains(particleTypeId);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetName(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId) || length < 2) {
		return 0;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	if(particleType.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(particleType.name.size(), static_cast<std::size_t>(length) - 1u);
	std::strncpy(buffer, particleType.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleTypeGetParentId(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleTypes.get(particleTypeId).parentId;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPosition(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.position;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.numParticles;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespan(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.lifespan;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespanVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.lifespanVariance;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetPositionRelative(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, bool relative) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.positionRelative = relative;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeGetPositionRelative(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return false;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return particleType.positionRelative;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPathForce(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.motionPathForce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialVelocity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialVelocity;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInheritedVelocity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.inheritedVelocity;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.velocityVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.acceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRadialAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.radialAcceleration;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.rotationMode = static_cast<pixelpart::RotationMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::RotationMode::angle);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.rotationMode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAlignmentMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.alignmentMode = static_cast<pixelpart::AlignmentMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetAlignmentMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::AlignmentMode::none);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.alignmentMode);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialRotation(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialRotation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotation(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotation;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationBySpeed(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotationBySpeed;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotationVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.angularVelocityVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetPivot(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.pivot;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetWeight(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.weight;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetBounce(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetFriction(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.friction;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetVisible(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, bool visible) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.visible = visible;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartParticleTypeIsVisible(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return false;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return particleType.visible;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetLayer(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t layer) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.layer = static_cast<uint32_t>(std::max(layer, 0));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetLayer(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.layer);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialSize(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialSize;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSize(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.size;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetSizeVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.sizeVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec3_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetStretch(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.stretch;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::vec4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColor(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.color;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::vec4_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetColorVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.colorVariance;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialOpacity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialOpacity;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.opacity;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.opacityVariance;
}

// TODO: material instance!!!!!
PixelpartParticleTypeGetMaterialId
PixelpartParticleTypeGetNumMaterialParameters
PixelpartParticleTypeGetMaterialParameterValue

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRenderer(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::ParticleRendererType::sprite);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.renderer);
}
}