#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurveGet(pixelpart::Curve<pixelpart::floatd>* curve, float position) {
	if(!curve) {
		return 0.0f;
	}

	return static_cast<float>(curve->get(position));
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurveGetPoint(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveSet(pixelpart::Curve<pixelpart::floatd>* curve, float value) {
	if(!curve) {
		return;
	}

	curve->setPoints({ pixelpart::Curve<pixelpart::floatd>::Point{ 0.5, value } });
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveAddPoint(pixelpart::Curve<pixelpart::floatd>* curve, float position, float value) {
	if(!curve) {
		return;
	}

	curve->addPoint(position, value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveSetPoint(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t index, float value) {
	if(!curve) {
		return;
	}

	curve->setPoint(static_cast<std::size_t>(index), value);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveMovePoint(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t index, float delta) {
	if(!curve) {
		return;
	}

	curve->movePoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveShiftPoint(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t index, float delta) {
	if(!curve) {
		return;
	}

	curve->shiftPoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveRemovePoint(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t index) {
	if(!curve) {
		return;
	}

	curve->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveClear(pixelpart::Curve<pixelpart::floatd>* curve) {
	if(!curve) {
		return;
	}

	curve->clear();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartCurveGetNumPoints(pixelpart::Curve<pixelpart::floatd>* curve) {
	if(!curve) {
		return 0U;
	}

	return curve->getNumPoints();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveMove(pixelpart::Curve<pixelpart::floatd>* curve, float delta) {
	if(!curve) {
		return;
	}

	curve->move(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveShift(pixelpart::Curve<pixelpart::floatd>* curve, float delta) {
	if(!curve) {
		return;
	}

	curve->shift(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveSetInterpolation(pixelpart::Curve<pixelpart::floatd>* curve, int32_t method) {
	if(!curve) {
		return;
	}

	curve->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurveGetInterpolation(pixelpart::Curve<pixelpart::floatd>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<int32_t>(curve->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveEnableAdaptiveCache(pixelpart::Curve<pixelpart::floatd>* curve) {
	if(!curve) {
		return;
	}

	curve->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurveEnableFixedCache(pixelpart::Curve<pixelpart::floatd>* curve, uint32_t size) {
	if(!curve) {
		return;
	}

	curve->enableFixedCache(static_cast<std::size_t>(size));
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartCurveGetCacheSize(pixelpart::Curve<pixelpart::floatd>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<uint32_t>(curve->getCacheSize());
}
}