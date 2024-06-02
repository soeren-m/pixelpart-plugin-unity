#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetX(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->get().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetY(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->get().y) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetZ(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->get().z) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetW(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->get().w) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat4SetValue(pixelpart::StaticProperty<pixelpart::vec4_t>* property, Vector4 value) {
	if(!property) {
		return;
	}

	property->setValue(util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetValueX(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->getValue().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetValueY(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->getValue().y) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetValueZ(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->getValue().z) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat4GetValueW(pixelpart::StaticProperty<pixelpart::vec4_t>* property) {
	return property ? util::toUnity(property->getValue().w) : 0.0f;
}
}