#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstdint>
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4At(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityFloat position) {
	if(!property) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4AddKeyframe(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityFloat position, UnityVector4 value) {
	if(!property) {
		return;
	}

	property->addKeyframe(position, internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4RemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt index) {
	if(!property || index < 0) {
		return;
	}

	property->removeKeyframe(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt index, UnityVector4 value) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframeValue(static_cast<std::size_t>(index), internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt index, UnityFloat position) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframePosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4ClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeCount(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeValue(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4KeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityFloat position, UnityFloat epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4SetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt method) {
	if(!property) {
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4GetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return static_cast<UnityInt>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<UnityInt>(property->keyframeInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float4_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat4EnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float4_t>* property, UnityInt size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}