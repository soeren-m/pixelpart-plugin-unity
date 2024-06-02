using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
[CustomEditor(typeof(PixelpartEffectAsset))]
public class PixelpartEffectAssetInspector : Editor {
	public override void OnInspectorGUI() {
		PixelpartEffectAsset asset = target as PixelpartEffectAsset;
		if(asset == null) {
			return;
		}

		EditorGUILayout.LabelField("Data Size", (asset.Data.Length / 1000).ToString() + " kB");

		float scale = EditorGUILayout.FloatField("Scale", asset.Scale);
		scale = Math.Max(scale, 0.0f);
		if(asset.Scale != scale) {
			asset.Scale = scale;
			EditorUtility.SetDirty(asset);
		}
	}
}
}
#endif