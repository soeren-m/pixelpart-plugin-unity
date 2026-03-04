#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/effect/EffectTrigger.h"
#include "pixelpart-runtime/effect/EffectTriggerCollection.h"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectTriggerCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().triggers().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectTriggers(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t* ids, pixelpart_unity::char_t* names, pixelpart_unity::int_t namesBufferSize) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!ids || !names || namesBufferSize < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	const pixelpart::EffectTriggerCollection& triggers = effectRuntime->effectAsset.effect().triggers();

	std::vector<pixelpart::id_t> triggerIds;
	triggerIds.reserve(triggers.size());
	for(const auto& entry : triggers) {
		triggerIds.push_back(entry.first);
	}

	std::sort(triggerIds.begin(), triggerIds.end());

	std::string namesString;
	for(std::size_t index = 0; index < triggerIds.size(); index++) {
		const pixelpart::EffectTrigger& trigger = triggers.at(triggerIds[index]);

		ids[index] = triggerIds[index].value();
		namesString += trigger.name();
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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartActivateEffectTrigger(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t triggerId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	effectRuntime->effectEngine->activateTrigger(pixelpart::id_t(triggerId));
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartIsEffectTriggerActivated(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t triggerId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	return effectRuntime->effectEngine->context().triggerActivated(pixelpart::id_t(triggerId));
}
}
