using System;
using System.IO;
using System.Text;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart {
public class PixelpartEffectAsset : ScriptableObject {
#if PIXELPART_USE_URP
	private const int renderPipeline = 1;
#elif PIXELPART_USE_HDRP
	private const int renderPipeline = 2;
#else
	private const int renderPipeline = 0;
#endif

	public byte[] Data = null;

	public float Scale = 1.0f;

	public PixelpartCustomMaterialAsset[] CustomMaterialAssets = null;

	public IntPtr LoadEffect() {
		return Plugin.PixelpartLoadEffect(Data, Data.Length);
	}

#if UNITY_EDITOR
	public static void CreateAsset(string path) {
		var data = File.ReadAllBytes(path);
		var assetPath = Path.ChangeExtension(path, ".asset");

		var asset = AssetDatabase.LoadAssetAtPath<PixelpartEffectAsset>(assetPath);

		if(asset == null) {
			asset = CreateInstance<PixelpartEffectAsset>();
			UpdateAsset(asset, path, assetPath, data);

			AssetDatabase.CreateAsset(asset, assetPath);
		}
		else {
			UpdateAsset(asset, path, assetPath, data);

			EditorUtility.SetDirty(asset);
		}

		AssetDatabase.ImportAsset(assetPath);
	}

	private static void UpdateAsset(PixelpartEffectAsset asset, string filePath, string assetPath, byte[] data) {
		asset.Data = data;

		var internalEffect = asset.LoadEffect();
		if(internalEffect == IntPtr.Zero) {
			asset.Data = null;
			asset.Scale = 1.0f;

			Debug.LogError("[Pixelpart] Failed to import \"" + filePath + "\"");
			return;
		}

		var effectName = Path.GetFileNameWithoutExtension(filePath);

		var nameBuffer = new byte[2048];
		var shaderMainCodeBuffer = new byte[16384];
		var shaderParameterCodeBuffer = new byte[16384];
		var shaderParameterNamesBuffer = new byte[16384];
		var shaderParameterIdsBuffer = new uint[64];
		var shaderTextureResourceIdsBuffer = new byte[16384];
		var shaderSamplerNamesBuffer = new byte[16384];

		var materialCount = Plugin.PixelpartGetMaterialResourceCount(internalEffect);
		asset.CustomMaterialAssets = new PixelpartCustomMaterialAsset[(int)materialCount * 2];

		for(uint materialIndex = 0; materialIndex < materialCount; materialIndex++) {
			var nameSize = Plugin.PixelpartGetMaterialResourceId(internalEffect, materialIndex, nameBuffer, nameBuffer.Length);
			var materialResourceId = Encoding.UTF8.GetString(nameBuffer, 0, nameSize);

			var blendMode = (BlendModeType)Plugin.PixelpartGetMaterialResourceBlendMode(internalEffect, materialResourceId);
			var lightingMode = (LightingModeType)Plugin.PixelpartGetMaterialResourceLightingMode(internalEffect, materialResourceId);

			var result = Plugin.PixelpartBuildMaterialShader(internalEffect, materialResourceId, renderPipeline,
				shaderMainCodeBuffer, shaderParameterCodeBuffer,
				shaderParameterNamesBuffer, shaderParameterIdsBuffer,
				shaderTextureResourceIdsBuffer, shaderSamplerNamesBuffer,
				out int shaderMainCodeLength, out int shaderParameterCodeLength,
				out int shaderParameterNamesLength, out int shaderParameterIdsLength,
				out int shaderTextureResourceIdsLength, out int shaderSamplerNamesLength,
				shaderMainCodeBuffer.Length, shaderParameterCodeBuffer.Length,
				shaderParameterNamesBuffer.Length, shaderParameterIdsBuffer.Length,
				shaderTextureResourceIdsBuffer.Length, shaderSamplerNamesBuffer.Length);

			if(!result) {
				var errorMessage = Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength);

				Debug.LogWarning("[Pixelpart] Failed to generate shader for material '" + materialResourceId + "': " + errorMessage);
				continue;
			}

			var mainCode = Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength);
			var parameterCode = Encoding.UTF8.GetString(shaderParameterCodeBuffer, 0, shaderParameterCodeLength);
			var shaderParameterNames = Encoding.UTF8.GetString(shaderParameterNamesBuffer, 0, shaderParameterNamesLength).
				Split(new char[] {' '}, 64, StringSplitOptions.RemoveEmptyEntries);
			var shaderTextureResourceIds = Encoding.UTF8.GetString(shaderTextureResourceIdsBuffer, 0, shaderTextureResourceIdsLength).
				Split(new char[] {' '}, 8, StringSplitOptions.RemoveEmptyEntries);
			var shaderSamplerNames = Encoding.UTF8.GetString(shaderSamplerNamesBuffer, 0, shaderSamplerNamesLength).
				Split(new char[] {' '}, 8, StringSplitOptions.RemoveEmptyEntries);

			var shaderParameterIds = new uint[shaderParameterIdsLength];
			Array.Copy(shaderParameterIdsBuffer, 0, shaderParameterIds, 0, shaderParameterIdsLength);

			var materialName = effectName.Replace(" ", "_") + "_" + materialResourceId.Replace(" ", "_");

			asset.CustomMaterialAssets[materialIndex * 2 + 0] = GenerateShaderAndMaterial(
				assetPath, materialResourceId, materialName, blendMode, lightingMode, false,
				mainCode, parameterCode,
				shaderParameterIds, shaderParameterNames, shaderTextureResourceIds, shaderSamplerNames);
			asset.CustomMaterialAssets[materialIndex * 2 + 1] = GenerateShaderAndMaterial(
				assetPath, materialResourceId, materialName + "_Inst", blendMode, lightingMode, true,
				mainCode, parameterCode,
				shaderParameterIds, shaderParameterNames, shaderTextureResourceIds, shaderSamplerNames);
		}

		Plugin.PixelpartDeleteEffect(internalEffect);
	}

	private static PixelpartCustomMaterialAsset GenerateShaderAndMaterial(string assetPath, string materialResourceId, string materialName,
		BlendModeType blendMode, LightingModeType lightingMode, bool instanced,
		string shaderMainCode, string shaderParameterCode, uint[] shaderParameterIds, string[] shaderParameterNames, string[] shaderTextureResourceIds, string[] shaderSamplerNames) {
		var shaderFilepath = Path.GetDirectoryName(assetPath);
		shaderFilepath = Path.Combine(shaderFilepath, materialName + ".shader");

		if(!File.Exists(shaderFilepath)) {
			var shaderCode = PixelpartShaderGenerator.GenerateShaderCode(
				materialName, shaderMainCode, shaderParameterCode, blendMode, lightingMode, instanced);

			var writer = File.CreateText(shaderFilepath);
			writer.Write(shaderCode);
			writer.Close();

			AssetDatabase.ImportAsset(shaderFilepath);
		}

		var materialFilepath = Path.GetDirectoryName(assetPath);
		materialFilepath = Path.Combine(materialFilepath, materialName + ".mat");

		var materialInfo = new PixelpartMaterialInfo(
			materialFilepath, blendMode, lightingMode, shaderParameterIds, shaderParameterNames, shaderTextureResourceIds, shaderSamplerNames);

		return new PixelpartCustomMaterialAsset(materialResourceId, instanced, materialInfo);
	}
#endif
}
}