#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/LightSource.h"
#include "pixelpart-runtime/effect/SpotLightSource.h"
#include <string>
#include <exception>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartLightSourceGetAttenuation(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t lightSourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.attenuation();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartLightSourceGetColor(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t lightSourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.color();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartLightSourceGetIntensity(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t lightSourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.intensity();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartSpotLightSourceGetSpotAngle(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t lightSourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::SpotLightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::SpotLightSource>(lightSourceId);

		return &lightSource.spotAngle();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartSpotLightSourceGetSpotAngleAttenuation(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t lightSourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::SpotLightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::SpotLightSource>(lightSourceId);

		return &lightSource.spotAngleAttenuation();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}
}