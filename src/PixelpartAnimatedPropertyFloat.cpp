#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatAt(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityFloat position) {
	if(!property) {
		return 0.0f;
	}

	return internal::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatAddKeyframe(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityFloat position, UnityFloat value) {
	if(!property) {
		return;
	}

	property->addKeyframe(position, internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatRemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt index) {
	if(!property || index < 0) {
		return;
	}

	property->removeKeyframe(static_cast<std::size_t>(index));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt index, UnityFloat value) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframeValue(static_cast<std::size_t>(index), internal::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt index, UnityFloat position) {
	if(!property || index < 0) {
		return;
	}

	property->setKeyframePosition(static_cast<std::size_t>(index), position);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatKeyframeCount(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt index) {
	if(!property || index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		return 0.0f;
	}

	return internal::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatKeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityFloat position, UnityFloat epsilon) {
	if(!property) {
		return 0;
	}

	return static_cast<UnityInt>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatSetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt method) {
	if(!property) {
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatGetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return static_cast<UnityInt>(pixelpart::CurveInterpolation::none);
	}

	return static_cast<UnityInt>(property->keyframeInterpolation());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatEnableAdaptiveCache(pixelpart::AnimatedProperty<pixelpart::float_t>* property) {
	if(!property) {
		return;
	}

	property->enableAdaptiveCache();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloatEnableFixedCache(pixelpart::AnimatedProperty<pixelpart::float_t>* property, UnityInt size) {
	if(!property) {
		return;
	}

	property->enableFixedCache(static_cast<std::size_t>(std::max(size, 1)));
}
}