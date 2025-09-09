#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/StaticProperty.h"

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::vector2_t UNITY_INTERFACE_API PixelpartStaticPropertyFloat2Value(pixelpart::StaticProperty<pixelpart::float2_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->value());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat2SetBaseValue(pixelpart::StaticProperty<pixelpart::float2_t>* property, pixelpart_unity::vector2_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->baseValue(pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector2_t UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetBaseValue(pixelpart::StaticProperty<pixelpart::float2_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->baseValue());
}
}
