#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstdint>
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3At(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityFloat position) {
	if(!property) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3AddKeyframe(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityFloat position, UnityVector3 value) {
	if(!property) {
		return;
	}

	property->addKeyframe(position, internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3RemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt index) {
	if(!property || index < 0) {
		return;
	}

	property->removeKeyframe(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt index, UnityVector3 value) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframeValue(static_cast<std::size_t>(index), internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt index, UnityFloat position) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframePosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3ClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeCount(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeValue(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	return internal::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityFloat position, UnityFloat epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt method) {
	if(!property) {
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return static_cast<UnityInt>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<UnityInt>(property->keyframeInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3EnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, UnityInt size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}