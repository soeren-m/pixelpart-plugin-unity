using UnityEditor;

namespace Pixelpart {
[CustomEditor(typeof(PixelpartEffectAsset))]
public class PixelpartEffectAssetInspector : Editor {
	private bool materialAssetVisible = false;

	public override void OnInspectorGUI() {
		var asset = target as PixelpartEffectAsset;
		if(asset == null) {
			return;
		}

		EditorGUILayout.LabelField("Data", (asset.Data.Length / 1000).ToString() + " kB");

		materialAssetVisible = EditorGUILayout.Foldout(materialAssetVisible, "Custom Materials");
		if(materialAssetVisible) {
			EditorGUI.indentLevel++;

			if(asset.CustomMaterials != null) {
				for(var materialIndex = 0; materialIndex < asset.CustomMaterials.Length; materialIndex++) {
					var materialDescriptor = asset.CustomMaterials[materialIndex];

					if(materialIndex > 0) {
						EditorGUILayout.Separator();
					}

					EditorGUILayout.LabelField("Material Path", materialDescriptor.MaterialPath);
					EditorGUILayout.LabelField("Resource ID", materialDescriptor.ResourceId);
					EditorGUILayout.LabelField("Instancing", materialDescriptor.Instancing.ToString());
					EditorGUILayout.LabelField("Blend Mode", materialDescriptor.BlendMode.ToString());
					EditorGUILayout.LabelField("Lighting Mode", materialDescriptor.LightingMode.ToString());
				}
			}

			EditorGUI.indentLevel--;
		}
	}
}
}