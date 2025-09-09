#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartStaticPropertyFloatValue(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0.0f;
	}

	return pixelpart_unity::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloatSetBaseValue(pixelpart::StaticProperty<pixelpart::float_t>* property, pixelpart_unity::float_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->baseValue(pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartStaticPropertyFloatGetBaseValue(pixelpart::StaticProperty<pixelpart::float_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0.0f;
	}

	return pixelpart_unity::toUnity(property->baseValue());
}
}
