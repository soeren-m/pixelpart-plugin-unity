#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/effect/Trigger.h"
#include <cstring>
#include <string>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetTriggerCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityInt>(effectRuntime->effectAsset.effect().triggers().size());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetTriggers(PixelpartEffectRuntime* effectRuntime, UnityUInt* ids, char* names, UnityInt namesBufferSize) {
	if(!effectRuntime || !ids || !names || namesBufferSize < 2) {
		return 0;
	}

	std::string namesString;
	std::size_t index = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().triggers()) {
		ids[index] = entry.first.value();
		namesString += entry.second.name();
		namesString += "|";

		index++;
	}

	if(!namesString.empty()) {
		namesString.pop_back();
	}

	UnityInt lengthNames = std::min(static_cast<UnityInt>(namesString.size()), namesBufferSize - 1);
	std::memcpy(names, namesString.c_str(), lengthNames);
	names[lengthNames] = '\0';

	return lengthNames;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartActivateTrigger(PixelpartEffectRuntime* effectRuntime, UnityUInt triggerId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		return;
	}

	effectRuntime->effectEngine->activateTrigger(pixelpart::id_t(triggerId));
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartIsTriggerActivated(PixelpartEffectRuntime* effectRuntime, UnityUInt triggerId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		return false;
	}

	return effectRuntime->effectEngine->runtimeContext().triggerActivated(pixelpart::id_t(triggerId));
}
}