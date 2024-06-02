#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetX(pixelpart::StaticProperty<pixelpart::vec2_t>* property) {
	return property ? util::toUnity(property->get().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetY(pixelpart::StaticProperty<pixelpart::vec2_t>* property) {
	return property ? util::toUnity(property->get().y) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat2SetValue(pixelpart::StaticProperty<pixelpart::vec2_t>* property, Vector2 value) {
	if(!property) {
		return;
	}

	property->setValue(util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetValueX(pixelpart::StaticProperty<pixelpart::vec2_t>* property) {
	return property ? util::toUnity(property->getValue().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat2GetValueY(pixelpart::StaticProperty<pixelpart::vec2_t>* property) {
	return property ? util::toUnity(property->getValue().y) : 0.0f;
}
}