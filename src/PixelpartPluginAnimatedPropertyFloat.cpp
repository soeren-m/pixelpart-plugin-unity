#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGet(pixelpart::AnimatedProperty<pixelpart::float_t>* property, float position) {
	return property ? util::toUnity(property->get(position)) : 0.0f;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatAddPoint(pixelpart::AnimatedProperty<pixelpart::float_t>* property, float position, float value) {
	if(!property) {
		return;
	}

	property->addPoint(position, util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatRemovePoint(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t index) {
	if(!property || index < 0) {
		return;
	}

	property->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetPoint(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t index, float value) {
	if(!property || index < 0) {
		return;
	}

	property->setPoint(static_cast<std::size_t>(index), util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetPointPosition(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t index, float position) {
	if(!property || index < 0) {
		return;
	}

	property->setPointPosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatClear(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return;
	}

	property->clear();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatContainsPoints(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return 0;
	}

	return property->containsPoints();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGetNumPoints(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getNumPoints());
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGetPoint(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->getNumPoints()) {
		return 0.0f;
	}

	return util::toUnity(property->getPoint(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGetPointIndex(pixelpart::AnimatedProperty<pixelpart::float_t>* property, float position, float epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetInterpolation(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t method) {
	if(!property) {
		return;
	}

	property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGetInterpolation(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return static_cast<int32_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<int32_t>(property->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatEnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatEnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float_t>* property, int32_t size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}