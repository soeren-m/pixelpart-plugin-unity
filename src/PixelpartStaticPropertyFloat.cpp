#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartStaticPropertyFloatValue(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	if(!property) {
		return 0.0f;
	}

	return internal::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloatSetBaseValue(pixelpart::StaticProperty<pixelpart::float_t>* property, UnityFloat value) {
	if(!property) {
		return;
	}

	property->baseValue(internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartStaticPropertyFloatGetBaseValue(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	if(!property) {
		return 0.0f;
	}

	return internal::toUnity(property->baseValue());
}
}