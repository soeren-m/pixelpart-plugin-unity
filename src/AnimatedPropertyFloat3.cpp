#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include <cstddef>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3At(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::float_t position) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->at(position));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3AddKeyframe(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::float_t position, pixelpart_unity::vector3_t value) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->addKeyframe(position, pixelpart_unity::fromUnity(value));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3RemoveKeyframe(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::int_t index) {
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframeValue(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::int_t index, pixelpart_unity::vector3_t value) {
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframePosition(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::int_t index, pixelpart_unity::float_t position) {
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3ClearKeyframes(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->clearKeyframes();
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeCount(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(property->keyframeCount());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeValue(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::int_t index) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}
	else if(index < 0 || static_cast<std::size_t>(index) >= property->keyframeCount()) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	return pixelpart_unity::toUnity(property->keyframe(static_cast<std::size_t>(index)).value);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3KeyframeIndex(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::float_t position, pixelpart_unity::float_t epsilon) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return -1;
	}

	auto index = property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon));

	return index ? static_cast<pixelpart_unity::int_t>(index.value()) : -1;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3SetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float3_t>* property, pixelpart_unity::int_t method) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return;
	}

	property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartAnimatedPropertyFloat3GetKeyframeInterpolation(pixelpart::AnimatedProperty<pixelpart::float3_t>* property) {
	if(!property) {
		pixelpart_unity::lastError = pixelpart_unity::invalidPropertyError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::CurveInterpolation::step);
	}

	return static_cast<pixelpart_unity::int_t>(property->keyframeInterpolation());
}
}
