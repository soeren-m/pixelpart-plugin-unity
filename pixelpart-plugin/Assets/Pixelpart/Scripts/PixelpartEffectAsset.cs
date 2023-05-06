using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart {
public class PixelpartEffectAsset : ScriptableObject {
	public byte[] Data = null;
	public float Scale = 1.0f;

	public PixelpartParticleTypeAsset[] ParticleTypeAssets = null;

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

		AssetDatabase.Refresh();
	}

	private static void UpdateAsset(PixelpartEffectAsset asset, string filePath, string assetPath, byte[] data) {
		asset.Data = data;

		IntPtr nativeEffect = asset.LoadEffect();

		if(nativeEffect != IntPtr.Zero) {
			byte[] nameBuffer = new byte[2048];
			byte[] shaderCodeBuffer = new byte[16384];
			byte[] shaderTextureIdBuffer = new byte[2048];

			uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);
			asset.ParticleTypeAssets = new PixelpartParticleTypeAsset[numParticleTypes];

			for(uint particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
				uint particleTypeId = Plugin.PixelpartFindParticleTypeByIndex(nativeEffect, particleTypeIndex);
				int nameSize = Plugin.PixelpartParticleTypeGetName(nativeEffect, particleTypeId, nameBuffer, nameBuffer.Length);
				string particleTypeName = System.Text.Encoding.UTF8.GetString(nameBuffer, 0, nameSize);

				int shaderCodeLength = 0;
				int shaderTextureIdLength = 0;
				Plugin.PixelpartBuildParticleShader(nativeEffect, particleTypeIndex,
					shaderCodeBuffer,
					shaderTextureIdBuffer,
					out shaderCodeLength,
					out shaderTextureIdLength,
					shaderCodeBuffer.Length,
					shaderTextureIdBuffer.Length);

				string shaderCode = Encoding.UTF8.GetString(shaderCodeBuffer, 0, shaderCodeLength);
				string[] shaderTextureIds = Encoding.UTF8.GetString(shaderTextureIdBuffer, 0, shaderTextureIdLength).
					Split(new[] {';'}, 16, StringSplitOptions.RemoveEmptyEntries);

				string assetName = Path.GetFileNameWithoutExtension(assetPath);
				string shaderName = assetName + " " + GetUniqueShaderId(shaderCode).ToString();
				string shaderFullName = "Pixelpart/" + shaderName;
				shaderCode = shaderCode.Replace("{SHADER_NAME}", shaderFullName);

				string shaderFilepath = Path.GetDirectoryName(assetPath);
				shaderFilepath = Path.Combine(shaderFilepath, shaderName + ".shader");

				if(!File.Exists(shaderFilepath)) {
					StreamWriter writer = File.CreateText(shaderFilepath);
					writer.Write(shaderCode);
					writer.Close();
				}

				asset.ParticleTypeAssets[particleTypeIndex] = new PixelpartParticleTypeAsset(
					particleTypeName,
					shaderFullName,
					shaderTextureIds);
			}

			Plugin.PixelpartDeleteEffect(nativeEffect);
		}
		else {
			asset.Data = null;
			asset.Scale = 1.0f;

			Debug.LogError("Error importing \"" + filePath + "\" as PixelpartEffectAsset");
		}
	}

	private static int GetUniqueShaderId(string str) {
		unchecked {
			int hash1 = 5381;
			int hash2 = hash1;

			for(int i = 0; i < str.Length && str[i] != '\0'; i += 2) {
				hash1 = ((hash1 << 5) + hash1) ^ str[i];
				if(i == str.Length - 1 || str[i + 1] == '\0') {
					break;
				}

				hash2 = ((hash2 << 5) + hash2) ^ str[i + 1];
			}

			return Math.Abs(hash1 + (hash2 * 1566083941));
		}
	}
#endif
}
}