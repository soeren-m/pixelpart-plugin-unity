#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartStaticPropertyFloat3Value(pixelpart::StaticProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat3SetBaseValue(pixelpart::StaticProperty<pixelpart::float3_t>* property, UnityVector3 value) {
	if(!property) {
		return;
	}

	property->baseValue(internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetBaseValue(pixelpart::StaticProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->baseValue());
}
}