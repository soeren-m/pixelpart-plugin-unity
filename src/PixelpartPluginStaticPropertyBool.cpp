#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartStaticPropertyBoolGet(pixelpart::StaticProperty<bool>* property) {
	return property ? property->get() : false;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyBoolSetValue(pixelpart::StaticProperty<bool>* property, bool value) {
	if(!property) {
		return;
	}

	property->setValue(value);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartStaticPropertyBoolGetValue(pixelpart::StaticProperty<bool>* property) {
	return property ? property->getValue() : false;
}
}