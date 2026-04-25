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

            EditorGUILayout.LabelField("Asset", EditorStyles.boldLabel);
            EditorGUILayout.PropertyField(serializedObject.FindProperty("effectAsset"));

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Playback", EditorStyles.boldLabel);
            EditorGUILayout.PropertyField(serializedObject.FindProperty("playing"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("loop"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("loopTime"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("warmupTime"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("speed"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("frameRate"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("seed"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("randomSeed"));

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

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Rendering", EditorStyles.boldLabel);

            EditorGUILayout.PropertyField(serializedObject.FindProperty("effectScale"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("flipH"));
            EditorGUILayout.PropertyField(serializedObject.FindProperty("flipV"));

            var materialsProperty = serializedObject.FindProperty("particleMaterials");
            var typeNamesProperty = serializedObject.FindProperty("particleTypeNames");

            particleMaterialsVisible = EditorGUILayout.Foldout(particleMaterialsVisible, materialsProperty.displayName);
            if (particleMaterialsVisible)
            {
                EditorGUI.indentLevel++;

                for (var materialIndex = 0; materialIndex < materialsProperty.arraySize; materialIndex++)
                {
                    EditorGUILayout.PropertyField(materialsProperty.GetArrayElementAtIndex(materialIndex),
                        new GUIContent(materialIndex < typeNamesProperty.arraySize ? typeNamesProperty.GetArrayElementAtIndex(materialIndex).stringValue : "Unknown"));
                }

                EditorGUI.indentLevel--;
            }

            serializedObject.ApplyModifiedProperties();

            if (inputsModified)
            {
                var effect = (PixelpartEffect)target;
                effect.ApplyInputProperties();
            }
        }
    }
}
