#include "Common.h"
#include "Error.h"
#include "pixelpart-runtime/types/Id.h"
#include "pixelpart-runtime/effect/BuiltInMaterialMetadata.h"
#include "pixelpart-runtime/effect/BuiltInMaterialRepository.h"
#include <cstring>
#include <string>
#include <algorithm>

namespace pixelpart_unity {
pixelpart::BuiltInMaterialRepository builtInMaterialRepository;
}

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetBuiltInMaterialParameterCount(const pixelpart_unity::char_t* materialName) {
	if(!materialName) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	const pixelpart::BuiltInMaterialMetadata* builtInMaterial = pixelpart_unity::builtInMaterialRepository.material(std::string(materialName));
	if(!builtInMaterial) {
		pixelpart_unity::lastError = pixelpart_unity::resourceNotFoundError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(builtInMaterial->parameters().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetBuiltInMaterialParameters(const pixelpart_unity::char_t* materialName,
	pixelpart_unity::uint_t* parameterIds, pixelpart_unity::char_t* parameterNames, pixelpart_unity::int_t parameterNameBufferSize) {
	if(!materialName || !parameterIds || !parameterNames || parameterNameBufferSize < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	const pixelpart::BuiltInMaterialMetadata* builtInMaterial = pixelpart_unity::builtInMaterialRepository.material(std::string(materialName));
	if(!builtInMaterial) {
		pixelpart_unity::lastError = pixelpart_unity::resourceNotFoundError;
		return 0;
	}

	std::size_t parameterIndex = 0;
	std::string parameterNameString;

	for(const auto& [parameterId, parameter] : builtInMaterial->parameters()) {
		parameterIds[parameterIndex] = parameterId.value();
		parameterNameString += "_" + parameter.name();
		parameterNameString += "|";

		parameterIndex++;
	}

	if(!parameterNameString.empty()) {
		parameterNameString.pop_back();
	}

	std::size_t parameterNamesLength = std::min(parameterNameString.size(), static_cast<std::size_t>(parameterNameBufferSize - 1));
	std::memcpy(parameterNames, parameterNameString.c_str(), parameterNamesLength);
	parameterNames[parameterNamesLength] = '\0';

	return static_cast<pixelpart_unity::int_t>(parameterNamesLength);
}
}
