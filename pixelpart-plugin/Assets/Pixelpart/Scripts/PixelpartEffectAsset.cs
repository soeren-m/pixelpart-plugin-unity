using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace pixelpart {
public class PixelpartEffectAsset : ScriptableObject {
	public byte[] Data;
	public float Scale = 1.0f;
	public PixelpartNodeAsset[] ParticleAssets = null;
	public PixelpartNodeAsset[] SpriteAssets = null;

	public IntPtr LoadEffect() {
		return Plugin.PixelpartLoadEffect(Data, Data.Length);
	}

#if UNITY_EDITOR
	public static void CreateAsset(string path) {
		byte[] data = File.ReadAllBytes(path);
		string keyword = System.Text.Encoding.UTF8.GetString(data, 0, Math.Min(data.Length, 256));

		if(keyword.Contains("\"project\"")) {
			CreateAsset(path, data);
		}
	}

	public static void CreateAsset(string path, byte[] data) {
		string assetPath = Path.ChangeExtension(path, ".asset");
		PixelpartEffectAsset asset = AssetDatabase.LoadAssetAtPath<PixelpartEffectAsset>(assetPath);

		if(asset == null) {
			asset = CreateInstance<PixelpartEffectAsset>();
			asset.Scale = 1.0f;
			UpdateAsset(asset, assetPath, data);

			AssetDatabase.CreateAsset(asset, assetPath);
		}
		else {
			UpdateAsset(asset, assetPath, data);

			EditorUtility.SetDirty(asset);
		}

		AssetDatabase.Refresh();
	}

	public static void UpdateAsset(PixelpartEffectAsset asset, string assetPath, byte[] data) {
		asset.Data = data;

		IntPtr nativeEffect = asset.LoadEffect();

		if(nativeEffect != IntPtr.Zero) {
			byte[] shaderCodeBuffer = new byte[16384];
			byte[] shaderTextureIdBuffer = new byte[2048];

			uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
			uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);
			asset.ParticleAssets = new PixelpartNodeAsset[numParticleEmitters];
			asset.SpriteAssets = new PixelpartNodeAsset[numSprites];

			for(uint emitterIndex = 0; emitterIndex < numParticleEmitters; emitterIndex++) {
				int shaderCodeLength = 0;
				int shaderTextureIdLength = 0;
				Plugin.PixelpartBuildParticleShader(nativeEffect, emitterIndex, shaderCodeBuffer, shaderTextureIdBuffer, out shaderCodeLength, out shaderTextureIdLength, shaderCodeBuffer.Length, shaderTextureIdBuffer.Length);
				string shaderCode = System.Text.Encoding.UTF8.GetString(shaderCodeBuffer, 0, shaderCodeLength);
				string[] shaderTextureIds = System.Text.Encoding.UTF8.GetString(shaderTextureIdBuffer, 0, shaderTextureIdLength).Split(new[] {';'}, 16, StringSplitOptions.RemoveEmptyEntries);

				string shaderName = "PixelpartShader" + GetUniqueShaderId(shaderCode).ToString();
				shaderCode = shaderCode.Replace("{SHADER_NAME}", "Pixelpart/" + shaderName);

				string filepath = Path.GetDirectoryName(assetPath);
				filepath = Path.Combine(filepath, shaderName + ".shader");

				if(!File.Exists(filepath)) {
					StreamWriter writer = File.CreateText(filepath);
					writer.Write(shaderCode);
					writer.Close();
				}

				asset.ParticleAssets[emitterIndex] = new PixelpartNodeAsset("Pixelpart/" + shaderName, shaderTextureIds);
			}

			for(uint spriteIndex = 0; spriteIndex < numSprites; spriteIndex++) {
				int shaderCodeLength = 0;
				int shaderTextureIdLength = 0;
				Plugin.PixelpartBuildSpriteShader(nativeEffect, spriteIndex, shaderCodeBuffer, shaderTextureIdBuffer, out shaderCodeLength, out shaderTextureIdLength, shaderCodeBuffer.Length, shaderTextureIdBuffer.Length);
				string shaderCode = System.Text.Encoding.UTF8.GetString(shaderCodeBuffer, 0, shaderCodeLength);
				string[] shaderTextureIds = System.Text.Encoding.UTF8.GetString(shaderTextureIdBuffer, 0, shaderTextureIdLength).Split(new[] {';'}, 16, StringSplitOptions.RemoveEmptyEntries);

				string shaderName = "PixelpartShader" + GetUniqueShaderId(shaderCode).ToString();
				shaderCode = shaderCode.Replace("{SHADER_NAME}", "Pixelpart/" + shaderName);

				string filepath = Path.GetDirectoryName(assetPath);
				filepath = Path.Combine(filepath, shaderName + ".shader");

				if(!File.Exists(filepath)) {
					StreamWriter writer = File.CreateText(filepath);
					writer.Write(shaderCode);
					writer.Close();
				}

				asset.SpriteAssets[spriteIndex] = new PixelpartNodeAsset("Pixelpart/" + shaderName, shaderTextureIds);
			}

			Plugin.PixelpartDeleteEffect(nativeEffect);
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