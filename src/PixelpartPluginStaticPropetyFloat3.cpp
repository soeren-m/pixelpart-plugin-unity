#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetX(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->get().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetY(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->get().y) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetZ(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->get().z) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartStaticPropertyFloat3SetValue(pixelpart::StaticProperty<pixelpart::vec3_t>* property, Vector3 value) {
	if(!property) {
		return;
	}

	property->setValue(util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetValueX(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->getValue().x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetValueY(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->getValue().y) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartStaticPropertyFloat3GetValueZ(pixelpart::StaticProperty<pixelpart::vec3_t>* property) {
	return property ? util::toUnity(property->getValue().z) : 0.0f;
}
}