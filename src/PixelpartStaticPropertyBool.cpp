#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartStaticPropertyBoolValue(pixelpart::StaticProperty<bool>* property) {
	if(!property) {
		return false;
	}

	return property->value();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyBoolSetBaseValue(pixelpart::StaticProperty<bool>* property, UnityBool value) {
	if(!property) {
		return;
	}

	property->baseValue(value);
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartStaticPropertyBoolGetBaseValue(pixelpart::StaticProperty<bool>* property) {
	if(!property) {
		return false;
	}

	return property->baseValue();
}
}