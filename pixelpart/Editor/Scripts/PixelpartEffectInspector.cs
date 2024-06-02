using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
[CustomEditor(typeof(PixelpartEffect))]
public class PixelpartEffectInspector : Editor {
	private bool showParticleShaders = true;

	public override void OnInspectorGUI() {
		PixelpartEffect effect = target as PixelpartEffect;
		if(effect == null) {
			return;
		}

		serializedObject.Update();

		EditorGUILayout.LabelField("Asset", EditorStyles.boldLabel);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("EffectAsset"));

		EditorGUILayout.Space();
		EditorGUILayout.LabelField("Playback", EditorStyles.boldLabel);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Playing"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Loop"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("LoopTime"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Speed"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FrameRate"));

		EditorGUILayout.Space();
		EditorGUILayout.LabelField("Rendering", EditorStyles.boldLabel);
		ShowParticleMaterials(serializedObject.FindProperty("ParticleMaterials"), effect);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipH"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipV"));

		serializedObject.ApplyModifiedProperties();
	}

	private void ShowParticleMaterials(SerializedProperty property, PixelpartEffect effect) {
		showParticleShaders = EditorGUILayout.Foldout(showParticleShaders, property.displayName);

		if(showParticleShaders) {
			GUI.enabled = false;
			EditorGUI.indentLevel++;

			for(int i = 0; i < property.arraySize; i++) {
				EditorGUILayout.PropertyField(property.GetArrayElementAtIndex(i),
					new GUIContent(i < effect.ParticleTypeNames.Count ? effect.ParticleTypeNames[i] : "Unknown"));
			}

			EditorGUI.indentLevel--;
			GUI.enabled = true;
		}
	}
}
}
#endif