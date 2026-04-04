using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartEffectEventCollection
    {
        public IReadOnlyDictionary<uint, string> EventNames => new ReadOnlyDictionary<uint, string>(eventNames);
        private readonly Dictionary<uint, string> eventNames = new Dictionary<uint, string>();

        public PixelpartEffectEventCollection()
        {

        }

        public PixelpartEffectEventCollection(IntPtr effectRuntimePtr)
        {
            var eventCount = Plugin.PixelpartGetEffectEventCount(effectRuntimePtr);
            var eventIdArray = new uint[eventCount];

            var eventNamesBuffer = new byte[16384];
            var eventNamesLength = Plugin.PixelpartGetEffectEvents(effectRuntimePtr, eventIdArray, eventNamesBuffer, eventNamesBuffer.Length);
            var eventNamesArray = Encoding.UTF8.GetString(eventNamesBuffer, 0, eventNamesLength)
                .Split(new[] { '|' }, 64, StringSplitOptions.RemoveEmptyEntries);

            for (var eventIndex = 0; eventIndex < eventCount; eventIndex++)
            {
                var eventId = eventIdArray[eventIndex];
                var eventName = eventNamesArray[eventIndex];

                eventNames[eventId] = eventName;
            }
        }

        public bool TryGetEventName(uint id, out string eventName)
        {
            if (!eventNames.TryGetValue(id, out eventName))
            {
                Debug.LogWarning("[Pixelpart] Unknown event id " + id);
                eventName = string.Empty;

                return false;
            }

            return true;
        }
    }
}
