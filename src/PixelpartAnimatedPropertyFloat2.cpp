#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstdint>
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2At(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityFloat position) {
	if(!property) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	return internal::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2AddKeyframe(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityFloat position, UnityVector2 value) {
	if(!property) {
		return;
	}

	property->addKeyframe(position, internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2RemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt index) {
	if(!property || index < 0) {
		return;
	}

	property->removeKeyframe(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2SetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt index, UnityVector2 value) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframeValue(static_cast<std::size_t>(index), internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2SetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt index, UnityFloat position) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframePosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2ClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2KeyframeCount(pixelpart::AnimatedProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2KeyframeValue(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	return internal::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2KeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityFloat position, UnityFloat epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2SetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt method) {
	if(!property) {
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2GetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return static_cast<UnityInt>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<UnityInt>(property->keyframeInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float2_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat2EnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float2_t>* property, UnityInt size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}