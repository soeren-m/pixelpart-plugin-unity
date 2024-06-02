using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart {
public class PixelpartEffectAsset : ScriptableObject {
	public byte[] Data = null;

	public float Scale = 1.0f;

	public PixelpartCustomMaterialAsset[] CustomMaterialAssets = null;

	public IntPtr LoadEffect() {
		return Plugin.PixelpartLoadEffect(Data, Data.Length);
	}

#if UNITY_EDITOR
	public static void CreateAsset(string path) {
		byte[] data = File.ReadAllBytes(path);
		string assetPath = Path.ChangeExtension(path, ".asset");

		PixelpartEffectAsset asset = AssetDatabase.LoadAssetAtPath<PixelpartEffectAsset>(assetPath);

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

		IntPtr internalEffect = asset.LoadEffect();
		if(internalEffect == IntPtr.Zero) {
			asset.Data = null;
			asset.Scale = 1.0f;

			Debug.LogError("[Pixelpart] Failed to import \"" + filePath + "\"");
			return;
		}

		string effectName = Path.GetFileNameWithoutExtension(filePath);

		byte[] nameBuffer = new byte[2048];
		byte[] shaderMainCodeBuffer = new byte[16384];
		byte[] shaderParameterCodeBuffer = new byte[16384];
		byte[] shaderParameterNamesBuffer = new byte[16384];
		uint[] shaderParameterIdsBuffer = new uint[64];
		byte[] shaderTextureResourceIdsBuffer = new byte[16384];
		byte[] shaderSamplerNamesBuffer = new byte[16384];

		uint numMaterials = Plugin.PixelpartGetMaterialResourceCount(internalEffect);
		asset.CustomMaterialAssets = new PixelpartCustomMaterialAsset[(int)numMaterials * 2];

		for(uint materialIndex = 0; materialIndex < numMaterials; materialIndex++) {
			int nameSize = Plugin.PixelpartGetMaterialResourceId(internalEffect, materialIndex, nameBuffer, nameBuffer.Length);
			string materialResourceId = System.Text.Encoding.UTF8.GetString(nameBuffer, 0, nameSize);

			BlendModeType blendMode = (BlendModeType)Plugin.PixelpartGetMaterialResourceBlendMode(internalEffect, materialResourceId);
			LightingModeType lightingMode = (LightingModeType)Plugin.PixelpartGetMaterialResourceLightingMode(internalEffect, materialResourceId);

#if PIXELPART_USE_URP
			int renderPipeline = 1;
#elif PIXELPART_USE_HDRP
			int renderPipeline = 2;
#else
			int renderPipeline = 0;
#endif

			int shaderMainCodeLength = 0;
			int shaderParameterCodeLength = 0;
			int shaderParameterNamesLength = 0;
			int shaderParameterIdsLength = 0;
			int shaderTextureResourceIdsLength = 0;
			int shaderSamplerNamesLength = 0;
			bool result = Plugin.PixelpartBuildMaterialShader(internalEffect, materialResourceId, renderPipeline,
				shaderMainCodeBuffer, shaderParameterCodeBuffer, shaderParameterNamesBuffer, shaderParameterIdsBuffer, shaderTextureResourceIdsBuffer, shaderSamplerNamesBuffer,
				out shaderMainCodeLength, out shaderParameterCodeLength, out shaderParameterNamesLength, out shaderParameterIdsLength, out shaderTextureResourceIdsLength, out shaderSamplerNamesLength,
				shaderMainCodeBuffer.Length, shaderParameterCodeBuffer.Length, shaderParameterNamesBuffer.Length, shaderParameterIdsBuffer.Length, shaderTextureResourceIdsBuffer.Length, shaderSamplerNamesBuffer.Length);

			if(!result) {
				string errorMessage = Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength);

				Debug.LogWarning("[Pixelpart] Failed to generate shader for material '" + materialResourceId + "': " + errorMessage);
				continue;
			}

			string mainCode = Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength);
			string parameterCode = Encoding.UTF8.GetString(shaderParameterCodeBuffer, 0, shaderParameterCodeLength);
			string[] shaderParameterNames = Encoding.UTF8.GetString(shaderParameterNamesBuffer, 0, shaderParameterNamesLength).
				Split(new char[] {' '}, 64, StringSplitOptions.RemoveEmptyEntries);
			string[] shaderTextureResourceIds = Encoding.UTF8.GetString(shaderTextureResourceIdsBuffer, 0, shaderTextureResourceIdsLength).
				Split(new char[] {' '}, 8, StringSplitOptions.RemoveEmptyEntries);
			string[] shaderSamplerNames = Encoding.UTF8.GetString(shaderSamplerNamesBuffer, 0, shaderSamplerNamesLength).
				Split(new char[] {' '}, 8, StringSplitOptions.RemoveEmptyEntries);

			uint[] shaderParameterIds = new uint[shaderParameterIdsLength];
			Array.Copy(shaderParameterIdsBuffer, 0, shaderParameterIds, 0, shaderParameterIdsLength);

			string materialName = effectName.Replace(" ", "_") + "_" + materialResourceId.Replace(" ", "_");

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
		string shaderFilepath = Path.GetDirectoryName(assetPath);
		shaderFilepath = Path.Combine(shaderFilepath, materialName + ".shader");

		if(!File.Exists(shaderFilepath)) {
			string shaderCode = PixelpartShaderGenerator.GenerateShaderCode(
				materialName, shaderMainCode, shaderParameterCode, blendMode, lightingMode, instanced);

			StreamWriter writer = File.CreateText(shaderFilepath);
			writer.Write(shaderCode);
			writer.Close();

			AssetDatabase.ImportAsset(shaderFilepath);
		}

		string materialFilepath = Path.GetDirectoryName(assetPath);
		materialFilepath = Path.Combine(materialFilepath, materialName + ".mat");

		PixelpartMaterialInfo materialInfo = new PixelpartMaterialInfo(
			materialFilepath, blendMode, lightingMode, shaderParameterIds, shaderParameterNames, shaderTextureResourceIds, shaderSamplerNames);

		return new PixelpartCustomMaterialAsset(materialResourceId, instanced, materialInfo);
	}
#endif
}
}