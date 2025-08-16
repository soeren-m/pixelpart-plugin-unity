#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::vector4_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4At(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::float_t position) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4AddKeyframe(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::float_t position, pixelpart_unity::vector4_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->addKeyframe(position, pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4RemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t index) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	property->removeKeyframe(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t index, pixelpart_unity::vector4_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	property->setKeyframeValue(static_cast<std::size_t>(index), pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t index, pixelpart_unity::float_t position) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	property->setKeyframePosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4ClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeCount(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector4_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeValue(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t index) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
	}
	else if(index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::float_t position, pixelpart_unity::float_t epsilon) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t method) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4GetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<pixelpart_unity::int_t>(property->keyframeInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4EnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, pixelpart_unity::int_t size) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}