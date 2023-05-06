using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
[CustomEditor(typeof(PixelpartEffect))]
public class PixelpartEffectInspector : Editor {
	public override void OnInspectorGUI() {
		PixelpartEffect effect = target as PixelpartEffect;
		if(effect == null) {
			return;
		}

		serializedObject.Update();

		EditorGUILayout.PropertyField(serializedObject.FindProperty("EffectAsset"));

		EditorGUILayout.PropertyField(serializedObject.FindProperty("Playing"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Loop"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("LoopTime"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("Speed"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FrameRate"));

		ShowParticleShaders(serializedObject.FindProperty("ParticleShaders"), effect);
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipH"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipV"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("CastShadows"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("ReceiveShadows"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("UseLightProbes"));
		EditorGUILayout.PropertyField(serializedObject.FindProperty("ProbeAnchor"));

		serializedObject.ApplyModifiedProperties();
	}

	private void ShowParticleShaders(SerializedProperty property, PixelpartEffect effect) {
		EditorGUILayout.PropertyField(property);

		EditorGUI.indentLevel++;

		if(property.isExpanded) {
			GUI.enabled = false;

			for(int i = 0; i < property.arraySize; i++) {
				EditorGUILayout.PropertyField(property.GetArrayElementAtIndex(i),
					new GUIContent(i < effect.ParticleTypeNames.Count ? effect.ParticleTypeNames[i] : "Unknown"));
			}

			GUI.enabled = true;
		}

		EditorGUI.indentLevel--;
	}
}
}
#endif