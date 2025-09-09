using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartTriggerCollection
    {
        public IReadOnlyDictionary<string, uint> TriggerIds => new ReadOnlyDictionary<string, uint>(triggerIds);
        private readonly Dictionary<string, uint> triggerIds = new Dictionary<string, uint>();

        public IReadOnlyList<string> TriggerNames => new ReadOnlyCollection<string>(triggerNames);
        private readonly List<string> triggerNames = new List<string>();

        public PixelpartTriggerCollection()
        {

        }

        public PixelpartTriggerCollection(IntPtr effectRuntimePtr)
        {
            var triggerCount = Plugin.PixelpartGetTriggerCount(effectRuntimePtr);
            var triggerIdArray = new uint[triggerCount];

            var triggerNamesBuffer = new byte[16384];
            var triggerNamesLength = Plugin.PixelpartGetTriggers(effectRuntimePtr, triggerIdArray, triggerNamesBuffer, triggerNamesBuffer.Length);
            var triggerNamesArray = Encoding.UTF8.GetString(triggerNamesBuffer, 0, triggerNamesLength)
                .Split(new[] { '|' }, 64, StringSplitOptions.RemoveEmptyEntries);

            for (var triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++)
            {
                var triggerId = triggerIdArray[triggerIndex];
                var triggerName = triggerNamesArray[triggerIndex];

                triggerIds[triggerName] = triggerId;
                triggerNames.Add(triggerName);
            }
        }

        public bool TryGetTriggerId(string name, out uint triggerId)
        {
            if (!triggerIds.TryGetValue(name, out triggerId))
            {
                Debug.LogWarning("[Pixelpart] Unknown trigger \"" + name + "\"");
                triggerId = 0;

                return false;
            }

            return true;
        }
    }
}
