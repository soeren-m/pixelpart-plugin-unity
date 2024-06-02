#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetX(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, float position) {
	return property ? util::toUnity(property->get(position).x) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetY(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, float position) {
	return property ? util::toUnity(property->get(position).y) : 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetZ(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, float position) {
	return property ? util::toUnity(property->get(position).z) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3AddPoint(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, float position, Vector3 value) {
	if(!property) {
		return;
	}

	property->addPoint(position, util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3RemovePoint(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index) {
	if(!property || index < 0) {
		return;
	}

	property->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetPoint(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index, Vector3 value) {
	if(!property || index < 0) {
		return;
	}

	property->setPoint(static_cast<std::size_t>(index), util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetPointPosition(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index, float position) {
	if(!property || index < 0) {
		return;
	}

	property->setPointPosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3Clear(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property) {
	if(!property) {
		return;
	}

	property->clear();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3ContainsPoints(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property) {
	if(!property) {
		return 0;
	}

	return property->containsPoints();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetNumPoints(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getNumPoints());
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetPointX(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->getNumPoints()) {
		return 0.0f;
	}

	return util::toUnity(property->getPoint(static_cast<std::size_t>(index)).value.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetPointY(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->getNumPoints()) {
		return 0.0f;
	}

	return util::toUnity(property->getPoint(static_cast<std::size_t>(index)).value.y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetPointZ(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->getNumPoints()) {
		return 0.0f;
	}

	return util::toUnity(property->getPoint(static_cast<std::size_t>(index)).value.z);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetPointIndex(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, float position, float epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetInterpolation(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t method) {
	if(!property) {
		return;
	}

	property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetInterpolation(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property) {
	if(!property) {
		return static_cast<int32_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<int32_t>(property->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3EnableFixedCache(pixelpart::AnimatedProperty<pixelpart::vec3_t>* property, int32_t size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}