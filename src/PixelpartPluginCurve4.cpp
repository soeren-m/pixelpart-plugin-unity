#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetX(pixelpart::Curve<pixelpart::vec4d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).x)
		: 0.0f;
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetY(pixelpart::Curve<pixelpart::vec4d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).y)
		: 0.0f;
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetZ(pixelpart::Curve<pixelpart::vec4d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).z)
		: 0.0f;
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetW(pixelpart::Curve<pixelpart::vec4d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).w)
		: 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetPointX(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.x);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetPointY(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.y);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetPointZ(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.z);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve4GetPointW(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.w);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4Set(pixelpart::Curve<pixelpart::vec4d>* curve, float x, float y, float z, float w) {
	if(!curve) {
		return;
	}

	curve->setPoints({ pixelpart::Curve<pixelpart::vec4d>::Point{ 0.5, pixelpart::vec4d(x, y, z, w) } });
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4AddPoint(pixelpart::Curve<pixelpart::vec4d>* curve, float position, float x, float y, float z, float w) {
	if(!curve) {
		return;
	}

	curve->addPoint(position, pixelpart::vec4d(x, y, z, w));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4SetPoint(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index, float x, float y, float z, float w) {
	if(!curve || index < 0) {
		return;
	}

	curve->setPoint(static_cast<std::size_t>(index), pixelpart::vec4d(x, y, z, w));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4MovePoint(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index, float deltaX, float deltaY, float deltaZ, float deltaW) {
	if(!curve || index < 0) {
		return;
	}

	curve->movePoint(static_cast<std::size_t>(index), pixelpart::vec4d(deltaX, deltaY, deltaZ, deltaW));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4ShiftPoint(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index, float delta) {
	if(!curve || index < 0) {
		return;
	}

	curve->shiftPoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4RemovePoint(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t index) {
	if(!curve || index < 0) {
		return;
	}

	curve->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4Clear(pixelpart::Curve<pixelpart::vec4d>* curve) {
	if(!curve) {
		return;
	}

	curve->clear();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve4GetNumPoints(pixelpart::Curve<pixelpart::vec4d>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<int32_t>(curve->getNumPoints());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4Move(pixelpart::Curve<pixelpart::vec4d>* curve, float deltaX, float deltaY, float deltaZ, float deltaW) {
	if(!curve) {
		return;
	}

	curve->move(pixelpart::vec4d(deltaX, deltaY, deltaZ, deltaW));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4Shift(pixelpart::Curve<pixelpart::vec4d>* curve, float delta) {
	if(!curve) {
		return;
	}

	curve->shift(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4SetInterpolation(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t method) {
	if(!curve) {
		return;
	}

	curve->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve4GetInterpolation(pixelpart::Curve<pixelpart::vec4d>* curve) {
	if(!curve) {
		return static_cast<int32_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<int32_t>(curve->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4EnableAdaptiveCache(pixelpart::Curve<pixelpart::vec4d>* curve) {
	if(!curve) {
		return;
	}

	curve->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve4EnableFixedCache(pixelpart::Curve<pixelpart::vec4d>* curve, int32_t size) {
	if(!curve) {
		return;
	}

	curve->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve4GetCacheSize(pixelpart::Curve<pixelpart::vec4d>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<int32_t>(curve->getCacheSize());
}
}