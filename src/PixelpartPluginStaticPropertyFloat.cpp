#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloatGet(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	return property ? util::toUnity(property->get()) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloatSetValue(pixelpart::StaticProperty<pixelpart::float_t>* property, float value) {
	if(!property) {
		return;
	}

	property->setValue(util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloatGetValue(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	return property ? util::toUnity(property->getValue()) : 0.0f;
}
}