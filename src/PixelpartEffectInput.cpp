#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/VariantValue.h"
#include "pixelpart-runtime/effect/EffectInput.h"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetEffectInputCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityInt>(effectRuntime->effectAsset.effect().inputs().size());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetEffectInputs(PixelpartEffectRuntime* effectRuntime, UnityUInt* ids, UnityInt* types, char* names, UnityInt namesBufferSize) {
	if(!effectRuntime || !ids || !types || !names || namesBufferSize < 2) {
		return 0;
	}

	const pixelpart::EffectInputCollection& inputs = effectRuntime->effectAsset.effect().inputs();

	std::vector<pixelpart::id_t> inputIds;
	inputIds.reserve(inputs.size());
	for(const auto& entry : inputs) {
		inputIds.push_back(entry.first);
	}

	std::sort(inputIds.begin(), inputIds.end());

	std::string namesString;
	for(std::size_t index = 0; index < inputIds.size(); index++) {
		const pixelpart::EffectInput& input = inputs.at(inputIds[index]);

		ids[index] = inputIds[index].value();
		types[index] = static_cast<UnityInt>(input.value().type());
		namesString += input.name();
		namesString += "|";
	}

	if(!namesString.empty()) {
		namesString.pop_back();
	}

	UnityInt lengthNames = std::min(static_cast<UnityInt>(namesString.size()), namesBufferSize - 1);
	std::memcpy(names, namesString.c_str(), lengthNames);
	names[lengthNames] = '\0';

	return lengthNames;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputBool(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityBool value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Bool(value));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputInt(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityInt value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Int(internal::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityFloat value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float(internal::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat2(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityVector2 value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float2(internal::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat3(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityVector3 value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float3(internal::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat4(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId, UnityVector4 value) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float4(internal::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartGetEffectInputBool(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return false;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return input.value().toBool();
	}
	catch(...) {
		return false;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetEffectInputInt(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return internal::toUnity(input.value().toInt());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartGetEffectInputFloat(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return 0.0f;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return internal::toUnity(input.value().toFloat());
	}
	catch(...) {
		return 0.0f;
	}
}

UNITY_INTERFACE_EXPORT UnityVector2 UNITY_INTERFACE_API PixelpartGetEffectInputFloat2(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return UnityVector2{ 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return internal::toUnity(input.value().toFloat2());
	}
	catch(...) {
		return UnityVector2{ 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartGetEffectInputFloat3(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return internal::toUnity(input.value().toFloat3());
	}
	catch(...) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT UnityVector4 UNITY_INTERFACE_API PixelpartGetEffectInputFloat4(PixelpartEffectRuntime* effectRuntime, UnityUInt inputId) {
	if(!effectRuntime) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return internal::toUnity(input.value().toFloat4());
	}
	catch(...) {
		return UnityVector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}
}
}