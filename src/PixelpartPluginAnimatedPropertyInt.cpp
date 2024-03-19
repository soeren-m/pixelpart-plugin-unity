#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyIntGet(pixelpart::AnimatedProperty<pixelpart::int_t>* property, float position) {
	return property ? util::toUnity(property->get(position)) : 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntAddPoint(pixelpart::AnimatedProperty<pixelpart::int_t>* property, float position, int32_t value) {
	if(!property) {
		return;
	}

	property->addPoint(position, util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntRemovePoint(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t index) {
	if(!property || index < 0) {
		return;
	}

	property->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntSetPoint(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t index, int32_t value) {
	if(!property || index < 0) {
		return;
	}

	property->setPoint(static_cast<std::size_t>(index), util::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntSetPointPosition(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t index, float position) {
	if(!property || index < 0) {
		return;
	}

	property->setPointPosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntClear(pixelpart::AnimatedProperty<pixelpart::int_t>* property) {
	if(!property) {
		return;
	}

	property->clear();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartAnimatedPropertyIntContainsPoints(pixelpart::AnimatedProperty<pixelpart::int_t>* property) {
	if(!property) {
		return 0;
	}

	return property->containsPoints();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyIntGetNumPoints(pixelpart::AnimatedProperty<pixelpart::int_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getNumPoints());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyIntGetPoint(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->getNumPoints()) {
		return 0.0f;
	}

	return util::toUnity(property->getPoint(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyIntGetPointIndex(pixelpart::AnimatedProperty<pixelpart::int_t>* property, float position, float epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<int32_t>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntSetInterpolation(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t method) {
	if(!property) {
		return;
	}

	property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartAnimatedPropertyIntGetInterpolation(pixelpart::AnimatedProperty<pixelpart::int_t>* property) {
	if(!property) {
		return static_cast<int32_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<int32_t>(property->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntEnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::int_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyIntEnableFixedCache(pixelpart::AnimatedProperty<pixelpart::int_t>* property, int32_t size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}