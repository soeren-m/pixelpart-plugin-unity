using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace pixelpart {
[CustomEditor(typeof(PixelpartEffectAsset))]
public class PixelpartEffectAssetInspector : UnityEditor.Editor {
	public override void OnInspectorGUI() {
		var asset = target as PixelpartEffectAsset;
		if(asset == null) {
			return;
		}

		EditorGUILayout.LabelField("Data Size", asset.Data.Length.ToString() + " bytes");

		var scale = EditorGUILayout.FloatField("Scale", asset.Scale);
		scale = Math.Max(scale, 0.0f);
		if(asset.Scale != scale) {
			asset.Scale = scale;
			EditorUtility.SetDirty(asset);
		}
	}
}
}
#endif