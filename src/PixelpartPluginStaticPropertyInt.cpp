#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartStaticPropertyIntGet(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	return property ? util::toUnity(property->get()) : 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyIntSetValue(pixelpart::StaticProperty<pixelpart::int_t>* property, int32_t value) {
	if(!property) {
		return;
	}

	property->setValue(util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartStaticPropertyIntGetValue(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	return property ? util::toUnity(property->getValue()) : 0;
}
}