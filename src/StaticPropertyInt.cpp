#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartStaticPropertyIntValue(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0;
	}

	return pixelpart_unity::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyIntSetBaseValue(pixelpart::StaticProperty<pixelpart::int_t>* property, pixelpart_unity::int_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->baseValue(pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartStaticPropertyIntGetBaseValue(pixelpart::StaticProperty<pixelpart::int_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0;
	}

	return pixelpart_unity::toUnity(property->baseValue());
}
}