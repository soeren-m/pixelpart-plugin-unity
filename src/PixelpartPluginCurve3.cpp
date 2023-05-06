#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetX(pixelpart::Curve<pixelpart::vec3d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).x)
		: 0.0f;
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetY(pixelpart::Curve<pixelpart::vec3d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).y)
		: 0.0f;
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetZ(pixelpart::Curve<pixelpart::vec3d>* curve, float position) {
	return curve != nullptr
		? static_cast<float>(curve->get(position).z)
		: 0.0f;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetPointX(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.x);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetPointY(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.y);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartCurve3GetPointZ(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index) {
	if(!curve || index < 0 || static_cast<std::size_t>(index) >= curve->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(curve->getPoints()[static_cast<std::size_t>(index)].value.z);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3Set(pixelpart::Curve<pixelpart::vec3d>* curve, float x, float y, float z) {
	if(!curve) {
		return;
	}

	curve->setPoints({ pixelpart::Curve<pixelpart::vec3d>::Point{ 0.5, pixelpart::vec3d(x, y, z) } });
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3AddPoint(pixelpart::Curve<pixelpart::vec3d>* curve, float position, float x, float y, float z) {
	if(!curve) {
		return;
	}

	curve->addPoint(position, pixelpart::vec3d(x, y, z));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3SetPoint(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index, float x, float y, float z) {
	if(!curve || index < 0) {
		return;
	}

	curve->setPoint(static_cast<std::size_t>(index), pixelpart::vec3d(x, y, z));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3MovePoint(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index, float deltaX, float deltaY, float deltaZ) {
	if(!curve || index < 0) {
		return;
	}

	curve->movePoint(static_cast<std::size_t>(index), pixelpart::vec3d(deltaX, deltaY, deltaZ));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3ShiftPoint(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index, float delta) {
	if(!curve || index < 0) {
		return;
	}

	curve->shiftPoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3RemovePoint(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t index) {
	if(!curve || index < 0) {
		return;
	}

	curve->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3Clear(pixelpart::Curve<pixelpart::vec3d>* curve) {
	if(!curve) {
		return;
	}

	curve->clear();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve3GetNumPoints(pixelpart::Curve<pixelpart::vec3d>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<int32_t>(curve->getNumPoints());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3Move(pixelpart::Curve<pixelpart::vec3d>* curve, float deltaX, float deltaY, float deltaZ) {
	if(!curve) {
		return;
	}

	curve->move(pixelpart::vec3d(deltaX, deltaY, deltaZ));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3Shift(pixelpart::Curve<pixelpart::vec3d>* curve, float delta) {
	if(!curve) {
		return;
	}

	curve->shift(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3SetInterpolation(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t method) {
	if(!curve) {
		return;
	}

	curve->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve3GetInterpolation(pixelpart::Curve<pixelpart::vec3d>* curve) {
	if(!curve) {
		return static_cast<int32_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<int32_t>(curve->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3EnableAdaptiveCache(pixelpart::Curve<pixelpart::vec3d>* curve) {
	if(!curve) {
		return;
	}

	curve->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartCurve3EnableFixedCache(pixelpart::Curve<pixelpart::vec3d>* curve, int32_t size) {
	if(!curve) {
		return;
	}

	curve->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartCurve3GetCacheSize(pixelpart::Curve<pixelpart::vec3d>* curve) {
	if(!curve) {
		return 0;
	}

	return static_cast<int32_t>(curve->getCacheSize());
}
}