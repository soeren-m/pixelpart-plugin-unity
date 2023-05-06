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

	std::size_t size = std::min(particleType.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, particleType.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartParticleTypeGetParentId(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return pixelpart::nullId;
	}

	return nativeEffect->project.effect.particleTypes.get(particleTypeId).parentId;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleTypeGetPosition(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.position;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.numParticles;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetLifespan(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.lifespan;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetLifespanVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.lifespanVariance = variance;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetLifespanVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.lifespanVariance);
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetMotionPathForce(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float force) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.motionPathForce = force;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetMotionPathForce(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.motionPathForce);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialVelocity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialVelocity;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.velocityVariance = variance;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.velocityVariance);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.acceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetRadialAcceleration(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.radialAcceleration;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetDamping(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.damping;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.rotationMode = static_cast<pixelpart::RotationMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAlignmentMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.alignmentMode = static_cast<pixelpart::AlignmentMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRotationMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::RotationMode::angle);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.rotationMode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetAlignmentMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::AlignmentMode::none);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.alignmentMode);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialRotation(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialRotation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleTypeGetRotation(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.rotation;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetRotationVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float varianceRoll, float varianceYaw, float variancePitch) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.rotationVariance = pixelpart::vec3d(varianceRoll, varianceYaw, variancePitch);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVarianceRoll(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.rotationVariance.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVarianceYaw(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.rotationVariance.y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetRotationVariancePitch(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.rotationVariance.z);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetAngularVelocityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float varianceRoll, float varianceYaw, float variancePitch) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.angularVelocityVariance = pixelpart::vec3d(varianceRoll, varianceYaw, variancePitch);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVarianceRoll(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.angularVelocityVariance.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVarianceYaw(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.angularVelocityVariance.y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetAngularVelocityVariancePitch(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.angularVelocityVariance.z);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetPivot(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float x, float y, float z) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.pivot = pixelpart::vec3d(x, y, z);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetPivotX(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.pivot.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetPivotY(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.pivot.y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetPivotZ(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.pivot.z);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetWeight(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.weight;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetBounce(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.bounce;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetFriction(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.blendMode = static_cast<pixelpart::BlendMode>(mode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::BlendMode::normal);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.blendMode);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialSize(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialSize;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec3d>* UNITY_INTERFACE_API PixelpartParticleTypeGetSize(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.size;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetSizeVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.sizeVariance = variance;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetSizeVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.sizeVariance);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec4d>* UNITY_INTERFACE_API PixelpartParticleTypeGetColor(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.color;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetColorVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float h, float s, float v) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.colorVariance = pixelpart::vec4d(h, s, v, 0.0f);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetColorVarianceHue(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.colorVariance.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetColorVarianceSaturation(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.colorVariance.y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetColorVarianceValue(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.colorVariance.z);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetInitialOpacity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.initialOpacity;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartParticleTypeGetOpacity(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return nullptr;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return &particleType.opacity;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSetOpacityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, float variance) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);
	particleType.opacityVariance = variance;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartParticleTypeGetOpacityVariance(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return 0.0f;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<float>(particleType.opacityVariance);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartParticleTypeGetRenderer(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.contains(particleTypeId)) {
		return static_cast<int32_t>(pixelpart::ParticleType::Renderer::sprite);
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.get(particleTypeId);

	return static_cast<int32_t>(particleType.renderer);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleTypeSpawnParticles(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeId, int32_t count) {
	if(!nativeEffect || count <= 0) {
		return;
	}

	nativeEffect->particleEngine.spawnParticles(particleTypeId, static_cast<uint32_t>(count));
}
}