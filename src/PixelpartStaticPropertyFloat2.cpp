#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartStaticPropertyFloat2Value(pixelpart::StaticProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	return internal::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat2SetBaseValue(pixelpart::StaticProperty<pixelpart::float2_t>* property, UnityVector2 value) {
	if(!property) {
		return;
	}

	property->baseValue(internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetBaseValue(pixelpart::StaticProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	return internal::toUnity(property->baseValue());
}
}