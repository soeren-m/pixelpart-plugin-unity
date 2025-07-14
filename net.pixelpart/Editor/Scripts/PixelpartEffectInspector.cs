using UnityEngine;
using UnityEditor;

namespace Pixelpart
{
    [CustomEditor(typeof(PixelpartEffect))]
    public class PixelpartEffectInspector : Editor
    {
        private bool particleMaterialsVisible = true;

        public override void OnInspectorGUI()
        {
            serializedObject.Update();

            ShowAssetSettings();
            ShowPlaybackSettings();
            var inputsModified = ShowInputSettings();
            ShowRenderingSettings();

            serializedObject.ApplyModifiedProperties();

            var effect = (PixelpartEffect)target;
            if (inputsModified)
            {
                effect.ApplyInputProperties();
            }
        }

        private void ShowAssetSettings()
        {
            EditorGUILayout.LabelField("Asset", EditorStyles.boldLabel);
            EditorGUILayout.PropertyField(serializedObject.FindProperty("EffectAsset"));
        }

        private void ShowPlaybackSettings()
        {
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Playback", EditorStyles.boldLabel);
            EditorGUILayout.PropertyField(serializedObject.FindProperty("Playing"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("Loop"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("LoopTime"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("WarmupTime"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("Speed"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("FrameRate"));
        }

        private bool ShowInputSettings()
        {
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Inputs", EditorStyles.boldLabel);

            var inputNamesProperty = serializedObject.FindProperty("effectInputNames");
            var inputValuesProperty = serializedObject.FindProperty("effectInputValues");

            var inputsModified = false;
            for (var inputIndex = 0; inputIndex < inputNamesProperty.arraySize && inputIndex < inputValuesProperty.arraySize; inputIndex++)
            {
                EditorGUI.BeginChangeCheck();

                EditorGUILayout.PropertyField(inputValuesProperty.GetArrayElementAtIndex(inputIndex),
                    new GUIContent(inputNamesProperty.GetArrayElementAtIndex(inputIndex).stringValue));

                if (EditorGUI.EndChangeCheck())
                {
                    inputsModified = true;
                }
            }

            return inputsModified;
        }

        private void ShowRenderingSettings()
        {
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Rendering", EditorStyles.boldLabel);

            EditorGUILayout.PropertyField(serializedObject.FindProperty("EffectScale"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipH"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("FlipV"));

            var materialsProperty = serializedObject.FindProperty("ParticleMaterials");
            var typeNamesProperty = serializedObject.FindProperty("ParticleTypeNames");

            particleMaterialsVisible = EditorGUILayout.Foldout(particleMaterialsVisible, materialsProperty.displayName);
            if (particleMaterialsVisible)
            {
                EditorGUI.indentLevel++;

                for (var materialIndex = 0; materialIndex < materialsProperty.arraySize && materialIndex < typeNamesProperty.arraySize; materialIndex++)
                {
                    EditorGUILayout.PropertyField(materialsProperty.GetArrayElementAtIndex(materialIndex),
                        new GUIContent(typeNamesProperty.GetArrayElementAtIndex(materialIndex).stringValue));
                }

                EditorGUI.indentLevel--;
            }
        }
    }
}