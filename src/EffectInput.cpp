#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/VariantValue.h"
#include "pixelpart-runtime/effect/EffectInput.h"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectInputCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().inputs().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectInputs(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t* ids, pixelpart_unity::int_t* types, pixelpart_unity::char_t* names, pixelpart_unity::int_t namesBufferSize) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!ids || !types || !names || namesBufferSize < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
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
		types[index] = static_cast<pixelpart_unity::int_t>(input.value().type());
		namesString += input.name();
		namesString += "|";
	}

	if(!namesString.empty()) {
		namesString.pop_back();
	}

	pixelpart_unity::int_t lengthNames = std::min(static_cast<pixelpart_unity::int_t>(namesString.size()), namesBufferSize - 1);
	std::memcpy(names, namesString.c_str(), lengthNames);
	names[lengthNames] = '\0';

	return lengthNames;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputBool(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::bool_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Bool(value));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputInt(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::int_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Int(pixelpart_unity::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::float_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float(pixelpart_unity::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat2(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::vector2_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float2(pixelpart_unity::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat3(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::vector3_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float3(pixelpart_unity::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat4(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId, pixelpart_unity::vector4_t value) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));
		input.value(pixelpart::VariantValue::Float4(pixelpart_unity::fromUnity(value)));

		effectRuntime->effectAsset.effect().applyInputs();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGetEffectInputBool(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return input.value().toBool();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectInputInt(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return pixelpart_unity::toUnity(input.value().toInt());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartGetEffectInputFloat(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return pixelpart_unity::toUnity(input.value().toFloat());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0.0f;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector2_t UNITY_INTERFACE_API PixelpartGetEffectInputFloat2(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return pixelpart_unity::toUnity(input.value().toFloat2());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector2_t{ 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartGetEffectInputFloat3(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return pixelpart_unity::toUnity(input.value().toFloat3());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector4_t UNITY_INTERFACE_API PixelpartGetEffectInputFloat4(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t inputId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::EffectInput& input = effectRuntime->effectAsset.effect().inputs().at(pixelpart::id_t(inputId));

		return pixelpart_unity::toUnity(input.value().toFloat4());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector4_t{ 0.0f, 0.0f, 0.0f, 0.0f };
}
}