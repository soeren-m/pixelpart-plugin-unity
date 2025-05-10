#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ForceField.h"
#include "pixelpart-runtime/effect/AccelerationField.h"
#include "pixelpart-runtime/effect/VectorField.h"
#include "pixelpart-runtime/effect/NoiseField.h"
#include "pixelpart-runtime/effect/DragField.h"
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetInfinite(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId, UnityBool mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		forceField.infinite(mode);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartForceFieldIsInfinite(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		return forceField.infinite();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		return &forceField.strength();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationDirection(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationDirection();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationDirectionVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationDirectionVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationStrengthVariance(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationStrengthVariance();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAccelerationFieldSetAccelerationGridSize(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId, UnityInt x, UnityInt y, UnityInt z) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		x = std::max(x, 1);
		y = std::max(y, 1);
		z = std::max(z, 1);

		forceField.accelerationGrid(x, y, z);
		forceField.randomizeAccelerationGrid(effectRuntime->rng);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeX(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeX();
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeY(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeY();
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeZ(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeZ();
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartVectorFieldSetVectorFieldFilter(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId, UnityInt filter) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		forceField.vectorFieldFilter(static_cast<pixelpart::VectorField::Filter>(filter));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartVectorFieldGetVectorFieldFilter(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::VectorField::Filter::none);
	}

	try {
		const pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		return static_cast<UnityInt>(forceField.vectorFieldFilter());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::VectorField::Filter::none);
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartVectorFieldGetTightness(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		return &forceField.tightness();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::int_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseOctaves(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseOctaves();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseFrequency(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseFrequency();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoisePersistence(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noisePersistence();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseLacunarity(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseLacunarity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNoiseFieldSetNoiseAnimated(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId, UnityBool animated) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		forceField.noiseAnimated(animated);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartNoiseFieldIsNoiseAnimated(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return forceField.noiseAnimated();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseAnimationTimeScale(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseAnimationTimeScale();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseAnimationTimeBase(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseAnimationTimeBase();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartDragFieldGetVelocityInfluence(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::DragField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::DragField>(forceFieldId);

		return &forceField.velocityInfluence();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartDragFieldGetSizeInfluence(PixelpartEffectRuntime* effectRuntime, UnityUInt forceFieldId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::DragField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::DragField>(forceFieldId);

		return &forceField.sizeInfluence();
	}
	catch(...) {
		return nullptr;
	}
}
}