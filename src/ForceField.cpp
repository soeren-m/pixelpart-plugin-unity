#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ForceField.h"
#include "pixelpart-runtime/effect/AccelerationField.h"
#include "pixelpart-runtime/effect/VectorField.h"
#include "pixelpart-runtime/effect/NoiseField.h"
#include "pixelpart-runtime/effect/DragField.h"
#include <string>
#include <exception>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartForceFieldSetInfinite(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId, pixelpart_unity::bool_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		forceField.infinite(mode);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartForceFieldIsInfinite(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		return forceField.infinite();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartForceFieldGetStrength(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ForceField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ForceField>(forceFieldId);

		return &forceField.strength();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationDirection(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationDirection();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationDirectionVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationDirectionVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationStrengthVariance(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return &forceField.accelerationStrengthVariance();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAccelerationFieldSetAccelerationGridSize(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId, pixelpart_unity::int_t x, pixelpart_unity::int_t y, pixelpart_unity::int_t z) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		x = std::max(x, 1);
		y = std::max(y, 1);
		z = std::max(z, 1);

		forceField.accelerationGrid(x, y, z);
		forceField.randomizeAccelerationGrid(pixelpart_unity::rng);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeX(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeX();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeY(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeY();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAccelerationFieldGetAccelerationGridSizeZ(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::AccelerationField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::AccelerationField>(forceFieldId);

		return forceField.accelerationGridSizeZ();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartVectorFieldSetVectorFieldFilter(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId, pixelpart_unity::int_t filter) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		forceField.vectorFieldFilter(static_cast<pixelpart::VectorField::Filter>(filter));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartVectorFieldGetVectorFieldFilter(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::VectorField::Filter::none);
	}

	try {
		const pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		return static_cast<pixelpart_unity::int_t>(forceField.vectorFieldFilter());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::VectorField::Filter::none);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartVectorFieldGetTightness(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::VectorField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::VectorField>(forceFieldId);

		return &forceField.tightness();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::int_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseOctaves(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseOctaves();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseFrequency(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseFrequency();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoisePersistence(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noisePersistence();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseLacunarity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseLacunarity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartNoiseFieldSetNoiseAnimated(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId, pixelpart_unity::bool_t animated) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		forceField.noiseAnimated(animated);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartNoiseFieldIsNoiseAnimated(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return forceField.noiseAnimated();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseAnimationTimeScale(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseAnimationTimeScale();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartNoiseFieldGetNoiseAnimationTimeBase(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::NoiseField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::NoiseField>(forceFieldId);

		return &forceField.noiseAnimationTimeBase();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartDragFieldGetVelocityInfluence(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::DragField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::DragField>(forceFieldId);

		return &forceField.velocityInfluence();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::StaticProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartDragFieldGetSizeInfluence(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t forceFieldId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::DragField& forceField =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::DragField>(forceFieldId);

		return &forceField.sizeInfluence();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}
}
