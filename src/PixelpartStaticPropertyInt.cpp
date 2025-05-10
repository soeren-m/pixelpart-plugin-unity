#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/StaticProperty.h"
#include <cstdint>

extern "C" {
UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartStaticPropertyIntValue(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	if(!property) {
		return 0;
	}

	return internal::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyIntSetBaseValue(pixelpart::StaticProperty<pixelpart::int_t>* property, UnityInt value) {
	if(!property) {
		return;
	}

	property->baseValue(internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartStaticPropertyIntGetBaseValue(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	if(!property) {
		return 0;
	}

	return internal::toUnity(property->baseValue());
}
}