#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/effect/EffectEvent.h"
#include "pixelpart-runtime/effect/EffectEventCollection.h"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectEventCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().events().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectEvents(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t* ids, pixelpart_unity::char_t* names, pixelpart_unity::int_t namesBufferSize) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!ids || !names || namesBufferSize < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	const pixelpart::EffectEventCollection& events = effectRuntime->effectAsset.effect().events();

	std::vector<pixelpart::id_t> eventIds;
	eventIds.reserve(events.size());
	for(const auto& [eventId, event] : events) {
		eventIds.push_back(eventId);
	}

	std::sort(eventIds.begin(), eventIds.end());

	std::string namesString;
	for(std::size_t index = 0; index < eventIds.size(); index++) {
		const pixelpart::EffectEvent& event = events.at(eventIds[index]);

		ids[index] = eventIds[index].value();
		namesString += event.name();
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

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetInvokedEffectEvents(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t* eventIds) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	for(pixelpart::id_t eventId : effectRuntime->invokedEventIds) {
		*eventIds = static_cast<pixelpart_unity::uint_t>(eventId.value());
		eventIds++;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->invokedEventIds.size());
}
}
