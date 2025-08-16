#include "Error.h"
#include "Common.h"
#include <cstring>
#include <algorithm>

namespace pixelpart_unity {
std::string lastError = "";

const std::string invalidEffectRuntimeError = "Invalid effect runtime";
const std::string invalidPropertyError = "Invalid property";
const std::string invalidArgumentError = "Invalid argument";
}

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartLastError(pixelpart_unity::char_t* buffer, pixelpart_unity::int_t bufferSize) {
	if(bufferSize < 1 || pixelpart_unity::lastError.empty()) {
		return 0;
	}

	std::strncpy(buffer, pixelpart_unity::lastError.c_str(), static_cast<std::size_t>(bufferSize));

	return static_cast<pixelpart_unity::int_t>(std::min(static_cast<std::size_t>(bufferSize) - 1, pixelpart_unity::lastError.size()));
}
}