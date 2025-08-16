#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartStaticPropertyBoolValue(pixelpart::StaticProperty<bool>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return false;
	}

	return property->value();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyBoolSetBaseValue(pixelpart::StaticProperty<bool>* property, pixelpart_unity::bool_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->baseValue(value);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartStaticPropertyBoolGetBaseValue(pixelpart::StaticProperty<bool>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return false;
	}

	return property->baseValue();
}
}