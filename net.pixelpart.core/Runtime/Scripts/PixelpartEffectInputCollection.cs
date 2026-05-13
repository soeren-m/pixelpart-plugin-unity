using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartEffectInputCollection
    {
        public IReadOnlyDictionary<string, uint> InputIds => new ReadOnlyDictionary<string, uint>(inputIds);
        private readonly Dictionary<string, uint> inputIds = new Dictionary<string, uint>();

        public IReadOnlyList<string> InputNames => new ReadOnlyCollection<string>(inputNames);
        private readonly List<string> inputNames = new List<string>();

        public IReadOnlyList<PixelpartVariantValue> InputValues => new ReadOnlyCollection<PixelpartVariantValue>(inputValues);
        private readonly List<PixelpartVariantValue> inputValues = new List<PixelpartVariantValue>();

        public PixelpartEffectInputCollection()
        {

        }

        public PixelpartEffectInputCollection(IntPtr effectRuntimePtr)
        {
            var inputCount = PixelpartPlugin.PixelpartGetEffectInputCount(effectRuntimePtr);
            var inputIdArray = new uint[inputCount];
            var inputTypeArray = new int[inputCount];

            var inputNamesBuffer = new byte[16384];
            var inputNamesLength = PixelpartPlugin.PixelpartGetEffectInputs(effectRuntimePtr, inputIdArray, inputTypeArray, inputNamesBuffer, inputNamesBuffer.Length);
            var inputNamesArray = Encoding.UTF8.GetString(inputNamesBuffer, 0, inputNamesLength)
                .Split(new[] { '|' }, 64, StringSplitOptions.RemoveEmptyEntries);

            for (var inputIndex = 0; inputIndex < inputCount; inputIndex++)
            {
                var inputId = inputIdArray[inputIndex];
                var inputName = inputNamesArray[inputIndex];
                var inputType = (PixelpartVariantValue.VariantType)inputTypeArray[inputIndex];

                inputIds[inputName] = inputId;
                inputNames.Add(inputName);

                var inputValue = new PixelpartVariantValue(PixelpartVariantValue.VariantType.Null, new Vector4(0.0f, 0.0f, 0.0f, 0.0f));
                switch (inputType)
                {
                    case PixelpartVariantValue.VariantType.Bool:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputBool(effectRuntimePtr, inputId));
                        break;
                    case PixelpartVariantValue.VariantType.Int:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputInt(effectRuntimePtr, inputId));
                        break;
                    case PixelpartVariantValue.VariantType.Float:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputFloat(effectRuntimePtr, inputId));
                        break;
                    case PixelpartVariantValue.VariantType.Float2:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputFloat2(effectRuntimePtr, inputId));
                        break;
                    case PixelpartVariantValue.VariantType.Float3:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputFloat3(effectRuntimePtr, inputId));
                        break;
                    case PixelpartVariantValue.VariantType.Float4:
                        inputValue = new PixelpartVariantValue(PixelpartPlugin.PixelpartGetEffectInputFloat4(effectRuntimePtr, inputId));
                        break;
                    default:
                        break;
                }

                inputValues.Add(inputValue);
            }
        }

        public PixelpartEffectInputCollection(IntPtr effectRuntimePtr, IList<string> names, IList<PixelpartVariantValue> values)
        {
            var inputCount = PixelpartPlugin.PixelpartGetEffectInputCount(effectRuntimePtr);
            var inputIdArray = new uint[inputCount];
            var inputTypeArray = new int[inputCount];

            var inputNamesBuffer = new byte[16384];
            _ = PixelpartPlugin.PixelpartGetEffectInputs(effectRuntimePtr, inputIdArray, inputTypeArray, inputNamesBuffer, inputNamesBuffer.Length);

            for (var inputIndex = 0; inputIndex < inputCount; inputIndex++)
            {
                inputIds[names[inputIndex]] = inputIdArray[inputIndex];
                inputNames.Add(names[inputIndex]);
                inputValues.Add(values[inputIndex]);
            }
        }

        public bool TryGetInputId(string name, out uint inputId)
        {
            if (!inputIds.TryGetValue(name, out inputId))
            {
                Debug.LogWarning("[Pixelpart] Unknown effect input \"" + name + "\"");
                inputId = 0;

                return false;
            }

            return true;
        }
    }
}
