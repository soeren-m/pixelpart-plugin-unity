using System;
using System.IO;
using System.Text;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace pixelpart {
public class PixelpartEffectAsset : ScriptableObject {
	public byte[] Data;
	public float Scale = 1.0f;

	public IntPtr LoadEffect() {
		return Plugin.PixelpartLoadEffect(Data, Data.Length);
	}

#if UNITY_EDITOR
	public static void CreateAsset(string path) {
		byte[] data = File.ReadAllBytes(path);

		string dataStringStart = System.Text.Encoding.UTF8.GetString(data, 0, Math.Min(data.Length, 256));
		
		if(dataStringStart.Contains("\"project\"")) {
			CreateAsset(path, data);
		}
	}

	public static void CreateAsset(string path, byte[] data) {
		string assetPath = Path.ChangeExtension(path, ".asset");
		var asset = AssetDatabase.LoadAssetAtPath<PixelpartEffectAsset>(assetPath);

		if(asset == null) {
			asset = CreateInstance<PixelpartEffectAsset>();
			asset.Data = data;
			asset.Scale = 1.0f;

			AssetDatabase.CreateAsset(asset, assetPath);
		}
		else {
			asset.Data = data;

			EditorUtility.SetDirty(asset);
		}

		AssetDatabase.Refresh();
	}
#endif
}
}