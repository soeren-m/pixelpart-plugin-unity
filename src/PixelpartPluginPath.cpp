#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartPathGetX(pixelpart::Curve<pixelpart::vec2d>* path, float position) {
	if(!path) {
		return 0.0f;
	}

	return static_cast<float>(path->get(position).x);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartPathGetY(pixelpart::Curve<pixelpart::vec2d>* path, float position) {
	if(!path) {
		return 0.0f;
	}

	return static_cast<float>(path->get(position).y);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartPathGetPointX(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index) {
	if(!path || index < 0 || static_cast<std::size_t>(index) >= path->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(path->getPoints()[static_cast<std::size_t>(index)].value.x);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartPathGetPointY(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index) {
	if(!path || index < 0 || static_cast<std::size_t>(index) >= path->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(path->getPoints()[static_cast<std::size_t>(index)].value.y);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathSet(pixelpart::Curve<pixelpart::vec2d>* path, float x, float y) {
	if(!path) {
		return;
	}

	path->setPoints({ pixelpart::Curve<pixelpart::vec2d>::Point{ 0.5, pixelpart::vec2d(x, y) } });
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathAddPoint(pixelpart::Curve<pixelpart::vec2d>* path, float position, float x, float y) {
	if(!path) {
		return;
	}

	path->addPoint(position, pixelpart::vec2d(x, y));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathSetPoint(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index, float x, float y) {
	if(!path) {
		return;
	}

	path->setPoint(static_cast<std::size_t>(index), pixelpart::vec2d(x, y));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathMovePoint(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index, float deltaX, float deltaY) {
	if(!path) {
		return;
	}

	path->movePoint(static_cast<std::size_t>(index), pixelpart::vec2d(deltaX, deltaY));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathShiftPoint(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index, float delta) {
	if(!path) {
		return;
	}

	path->shiftPoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathRemovePoint(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t index) {
	if(!path) {
		return;
	}

	path->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathClear(pixelpart::Curve<pixelpart::vec2d>* path) {
	if(!path) {
		return;
	}

	path->clear();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartPathGetNumPoints(pixelpart::Curve<pixelpart::vec2d>* path) {
	if(!path) {
		return 0U;
	}

	return path->getNumPoints();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathMove(pixelpart::Curve<pixelpart::vec2d>* path, float deltaX, float deltaY) {
	if(!path) {
		return;
	}

	path->move(pixelpart::vec2d(deltaX, deltaY));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathShift(pixelpart::Curve<pixelpart::vec2d>* path, float delta) {
	if(!path) {
		return;
	}

	path->shift(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathSetInterpolation(pixelpart::Curve<pixelpart::vec2d>* path, int32_t method) {
	if(!path) {
		return;
	}

	path->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartPathGetInterpolation(pixelpart::Curve<pixelpart::vec2d>* path) {
	if(!path) {
		return 0;
	}

	return static_cast<int32_t>(path->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathEnableAdaptiveCache(pixelpart::Curve<pixelpart::vec2d>* path) {
	if(!path) {
		return;
	}

	path->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPathEnableFixedCache(pixelpart::Curve<pixelpart::vec2d>* path, uint32_t size) {
	if(!path) {
		return;
	}

	path->enableFixedCache(static_cast<std::size_t>(size));
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartPathGetCacheSize(pixelpart::Curve<pixelpart::vec2d>* path) {
	if(!path) {
		return 0;
	}

	return static_cast<uint32_t>(path->getCacheSize());
}
}