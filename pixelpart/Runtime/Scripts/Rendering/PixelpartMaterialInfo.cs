using System;

namespace Pixelpart {
[Serializable]
public class PixelpartMaterialInfo {
	public string MaterialPath;

	public BlendModeType BlendMode;

	public LightingModeType LightingMode;

	public uint[] ParameterIds;

	public string[] ParameterNames;

	public string[] TextureResourceIds;

	public string[] SamplerNames;

	public PixelpartMaterialInfo(string materialPath, BlendModeType blendMode, LightingModeType lightingMode,
		uint[] parameterIds, string[] parameterNames, string[] textureResourceIds, string[] samplerNames) {
		MaterialPath = materialPath;
		BlendMode = blendMode;
		LightingMode = lightingMode;
		ParameterIds = parameterIds;
		ParameterNames = parameterNames;
		TextureResourceIds = textureResourceIds;
		SamplerNames = samplerNames;
	}

	public string GetParameterName(uint parameterId) {
		for(var parameterIndex = 0; parameterIndex < ParameterIds.Length && parameterIndex < ParameterNames.Length; parameterIndex++) {
			if(ParameterIds[parameterIndex] == parameterId) {
				return ParameterNames[parameterIndex];
			}
		}

		return null;
	}
}
}