#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/LightSource.h"
#include "pixelpart-runtime/effect/SpotLightSource.h"
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartLightSourceGetAttenuation(PixelpartEffectRuntime* effectRuntime, UnityUInt lightSourceId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.attenuation();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float4_t>* UNITY_INTERFACE_API PixelpartLightSourceGetColor(PixelpartEffectRuntime* effectRuntime, UnityUInt lightSourceId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.color();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartLightSourceGetIntensity(PixelpartEffectRuntime* effectRuntime, UnityUInt lightSourceId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::LightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::LightSource>(lightSourceId);

		return &lightSource.intensity();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartSpotLightSourceGetSpotAngle(PixelpartEffectRuntime* effectRuntime, UnityUInt lightSourceId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::SpotLightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::SpotLightSource>(lightSourceId);

		return &lightSource.spotAngle();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartSpotLightSourceGetSpotAngleAttenuation(PixelpartEffectRuntime* effectRuntime, UnityUInt lightSourceId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::SpotLightSource& lightSource =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::SpotLightSource>(lightSourceId);

		return &lightSource.spotAngleAttenuation();
	}
	catch(...) {
		return nullptr;
	}
}
}