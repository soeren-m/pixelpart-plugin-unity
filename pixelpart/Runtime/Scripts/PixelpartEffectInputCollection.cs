using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using UnityEngine;

namespace Pixelpart {
internal class PixelpartEffectInputCollection {
	public IReadOnlyDictionary<string, uint> InputIds => new ReadOnlyDictionary<string, uint>(inputIds);
	private readonly Dictionary<string, uint> inputIds = new Dictionary<string, uint>();

	public IReadOnlyList<string> InputNames => new ReadOnlyCollection<string>(inputNames);
	private readonly List<string> inputNames = new List<string>();

	public IReadOnlyList<PixelpartVariantValue> InputValues => new ReadOnlyCollection<PixelpartVariantValue>(inputValues);
	private readonly List<PixelpartVariantValue> inputValues = new List<PixelpartVariantValue>();

	public PixelpartEffectInputCollection() {

	}

	public PixelpartEffectInputCollection(IntPtr effectRuntimePtr) {
		var inputCount = Plugin.PixelpartGetEffectInputCount(effectRuntimePtr);
		var inputIdArray = new uint[inputCount];
		var inputTypeArray = new int[inputCount];

		var inputNamesBuffer = new byte[16384];
		var inputNamesLength = Plugin.PixelpartGetEffectInputs(effectRuntimePtr, inputIdArray, inputTypeArray, inputNamesBuffer, inputNamesBuffer.Length);
		var inputNamesArray = Encoding.UTF8.GetString(inputNamesBuffer, 0, inputNamesLength)
			.Split(new[] {'|'}, 64, StringSplitOptions.RemoveEmptyEntries);

		for(var inputIndex = 0; inputIndex < inputCount; inputIndex++) {
			var inputId = inputIdArray[inputIndex];
			var inputName = inputNamesArray[inputIndex];
			var inputType = (PixelpartVariantValue.VariantType)inputTypeArray[inputIndex];

			inputIds[inputName] = inputId;
			inputNames.Add(inputName);
			inputValues.Add(new PixelpartVariantValue(inputType,
				Plugin.PixelpartGetEffectInputFloat4(effectRuntimePtr, inputId)));
		}
	}

	public PixelpartEffectInputCollection(IntPtr effectRuntimePtr, IList<string> names, IList<PixelpartVariantValue> values) {
		var inputCount = Plugin.PixelpartGetEffectInputCount(effectRuntimePtr);
		var inputIdArray = new uint[inputCount];
		var inputTypeArray = new int[inputCount];

		var inputNamesBuffer = new byte[16384];
		_ = Plugin.PixelpartGetEffectInputs(effectRuntimePtr, inputIdArray, inputTypeArray, inputNamesBuffer, inputNamesBuffer.Length);

		for(var inputIndex = 0; inputIndex < inputCount; inputIndex++) {
			inputIds[names[inputIndex]] = inputIdArray[inputIndex];
			inputNames.Add(names[inputIndex]);
			inputValues.Add(values[inputIndex]);
		}
	}

	public bool TryGetInputId(string name, out uint inputId) {
		if(!inputIds.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input \"" + name + "\"");
			inputId = 0;

			return false;
		}

		return true;
	}
}
}