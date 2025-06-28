#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/effect/Trigger.h"
#include <cstring>
#include <string>
#include <vector>
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

	const pixelpart::TriggerCollection& triggers = effectRuntime->effectAsset.effect().triggers();

	std::vector<pixelpart::id_t> triggerIds;
	triggerIds.reserve(triggers.size());
	for(const auto& entry : triggers) {
		triggerIds.push_back(entry.first);
	}

	std::sort(triggerIds.begin(), triggerIds.end());

	std::string namesString;
	for(std::size_t index = 0; index < triggerIds.size(); index++) {
		const pixelpart::Trigger& trigger = triggers.at(triggerIds[index]);

		ids[index] = triggerIds[index].value();
		namesString += trigger.name();
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