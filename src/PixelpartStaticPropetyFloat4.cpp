#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartStaticPropertyFloat4Value(pixelpart::StaticProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat4SetBaseValue(pixelpart::StaticProperty<pixelpart::float4_t>* property, UnityVector4 value) {
	if(!property) {
		return;
	}

	property->baseValue(internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetBaseValue(pixelpart::StaticProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->baseValue());
}
}