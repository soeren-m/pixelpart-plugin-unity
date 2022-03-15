#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetR(pixelpart::Curve<pixelpart::vec4d>* gradient, float position) {
	if(!gradient) {
		return 0.0f;
	}

	return static_cast<float>(gradient->get(position).r);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetG(pixelpart::Curve<pixelpart::vec4d>* gradient, float position) {
	if(!gradient) {
		return 0.0f;
	}

	return static_cast<float>(gradient->get(position).g);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetB(pixelpart::Curve<pixelpart::vec4d>* gradient, float position) {
	if(!gradient) {
		return 0.0f;
	}

	return static_cast<float>(gradient->get(position).b);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetA(pixelpart::Curve<pixelpart::vec4d>* gradient, float position) {
	if(!gradient) {
		return 0.0f;
	}

	return static_cast<float>(gradient->get(position).a);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetPointR(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index) {
	if(!gradient || index < 0 || static_cast<std::size_t>(index) >= gradient->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(gradient->getPoints()[static_cast<std::size_t>(index)].value.r);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetPointG(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index) {
	if(!gradient || index < 0 || static_cast<std::size_t>(index) >= gradient->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(gradient->getPoints()[static_cast<std::size_t>(index)].value.g);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetPointB(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index) {
	if(!gradient || index < 0 || static_cast<std::size_t>(index) >= gradient->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(gradient->getPoints()[static_cast<std::size_t>(index)].value.b);
}
UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGradientGetPointA(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index) {
	if(!gradient || index < 0 || static_cast<std::size_t>(index) >= gradient->getNumPoints()) {
		return 0.0f;
	}

	return static_cast<float>(gradient->getPoints()[static_cast<std::size_t>(index)].value.a);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientSet(pixelpart::Curve<pixelpart::vec4d>* gradient, float r, float g, float b, float a) {
	if(!gradient) {
		return;
	}

	gradient->setPoints({ pixelpart::Curve<pixelpart::vec4d>::Point{ 0.5, pixelpart::vec4d(r, g, b, a) } });
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientAddPoint(pixelpart::Curve<pixelpart::vec4d>* gradient, float position, float r, float g, float b, float a) {
	if(!gradient) {
		return;
	}

	gradient->addPoint(position, pixelpart::vec4d(r, g, b, a));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientSetPoint(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index, float r, float g, float b, float a) {
	if(!gradient) {
		return;
	}

	gradient->setPoint(static_cast<std::size_t>(index), pixelpart::vec4d(r, g, b, a));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientMovePoint(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index, float deltaR, float deltaG, float deltaB, float deltaA) {
	if(!gradient) {
		return;
	}

	gradient->movePoint(static_cast<std::size_t>(index), pixelpart::vec4d(deltaR, deltaG, deltaB, deltaA));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientShiftPoint(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index, float delta) {
	if(!gradient) {
		return;
	}

	gradient->shiftPoint(static_cast<std::size_t>(index), delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientRemovePoint(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t index) {
	if(!gradient) {
		return;
	}

	gradient->removePoint(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientClear(pixelpart::Curve<pixelpart::vec4d>* gradient) {
	if(!gradient) {
		return;
	}

	gradient->clear();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGradientGetNumPoints(pixelpart::Curve<pixelpart::vec4d>* gradient) {
	if(!gradient) {
		return 0U;
	}

	return gradient->getNumPoints();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientMove(pixelpart::Curve<pixelpart::vec4d>* gradient, float deltaR, float deltaG, float deltaB, float deltaA) {
	if(!gradient) {
		return;
	}

	gradient->move(pixelpart::vec4d(deltaR, deltaG, deltaB, deltaA));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientShift(pixelpart::Curve<pixelpart::vec4d>* gradient, float delta) {
	if(!gradient) {
		return;
	}

	gradient->shift(delta);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientSetInterpolation(pixelpart::Curve<pixelpart::vec4d>* gradient, int32_t method) {
	if(!gradient) {
		return;
	}

	gradient->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGradientGetInterpolation(pixelpart::Curve<pixelpart::vec4d>* gradient) {
	if(!gradient) {
		return 0;
	}

	return static_cast<int32_t>(gradient->getInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientEnableAdaptiveCache(pixelpart::Curve<pixelpart::vec4d>* gradient) {
	if(!gradient) {
		return;
	}

	gradient->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGradientEnableFixedCache(pixelpart::Curve<pixelpart::vec4d>* gradient, uint32_t size) {
	if(!gradient) {
		return;
	}

	gradient->enableFixedCache(static_cast<std::size_t>(size));
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGradientGetCacheSize(pixelpart::Curve<pixelpart::vec4d>* gradient) {
	if(!gradient) {
		return 0;
	}

	return static_cast<uint32_t>(gradient->getCacheSize());
}
}