using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
[CustomEditor(typeof(PixelpartEffect))]
public class PixelpartEffectInspector : Editor {
	private bool particleMaterialsVisible = true;

	public override void OnInspectorGUI() {
		serializedObject.Update();

		ShowAssetSettings();
		ShowPlaybackSettings();
		ShowInputSettings();
		ShowRenderingSettings();

		serializedObject.ApplyModifiedProperties();
	}

	private void ShowAssetSettings() {
		EditorGUILayout.LabelField("Asset", EditorStyles.boldLabel);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("EffectAsset"));
	}

	private void ShowPlaybackSettings() {
		EditorGUILayout.Space();
		EditorGUILayout.LabelField("Playback", EditorStyles.boldLabel);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Playing"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Loop"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("LoopTime"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Speed"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FrameRate"));
	}

	private void ShowInputSettings() {
		PixelpartEffect effect = (PixelpartEffect)target;

		EditorGUILayout.Space();
		EditorGUILayout.LabelField("Inputs", EditorStyles.boldLabel);

		SerializedProperty inputNamesProperty = serializedObject.FindProperty("effectInputNames");
		SerializedProperty inputValuesProperty = serializedObject.FindProperty("effectInputValues");

		bool inputsChanged = false;

		for(int inputIndex = 0; inputIndex < inputNamesProperty.arraySize && inputIndex < inputValuesProperty.arraySize; inputIndex++) {
			EditorGUI.BeginChangeCheck();

			EditorGUILayout.PropertyField(inputValuesProperty.GetArrayElementAtIndex(inputIndex),
				new GUIContent(inputNamesProperty.GetArrayElementAtIndex(inputIndex).stringValue));

			if(EditorGUI.EndChangeCheck()) {
				inputsChanged = true;
			}
		}

		if(inputsChanged) {
			effect.ApplyInputProperties();
		}
	}

	private void ShowRenderingSettings() {
		EditorGUILayout.Space();
		EditorGUILayout.LabelField("Rendering", EditorStyles.boldLabel);

		SerializedProperty materialsProperty = serializedObject.FindProperty("ParticleMaterials");
		SerializedProperty typeNamesProperty = serializedObject.FindProperty("ParticleTypeNames");

		particleMaterialsVisible = EditorGUILayout.Foldout(particleMaterialsVisible, materialsProperty.displayName);
		if(particleMaterialsVisible) {
			EditorGUI.indentLevel++;

			for(int materialIndex = 0; materialIndex < materialsProperty.arraySize && materialIndex < typeNamesProperty.arraySize; materialIndex++) {
				EditorGUILayout.PropertyField(materialsProperty.GetArrayElementAtIndex(materialIndex),
					new GUIContent(typeNamesProperty.GetArrayElementAtIndex(materialIndex).stringValue));
			}

			EditorGUI.indentLevel--;
		}

		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipH"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipV"));
	}
}
}
#endif