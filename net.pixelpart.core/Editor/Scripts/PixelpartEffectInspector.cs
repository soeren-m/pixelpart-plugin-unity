using UnityEngine;
using UnityEditor;

namespace Pixelpart
{
    [CustomEditor(typeof(PixelpartEffect))]
    public class PixelpartEffectInspector : Editor
    {
        private PixelpartEffect effect = null;

        private double prevUpdateTime = 0.0;

        private bool particleMaterialsVisible = true;

        public void OnEnable()
        {
            effect = (PixelpartEffect)target;

            if (!Application.isPlaying)
            {
                prevUpdateTime = EditorApplication.timeSinceStartup;

                EditorApplication.update -= OnUpdate;
                EditorApplication.update += OnUpdate;
            }
        }

        public void OnDisable()
        {
            if (!Application.isPlaying)
            {
                EditorApplication.update -= OnUpdate;
            }
        }

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
                effect.ApplyInputProperties();
            }
        }

        private void OnSceneGUI()
        {
            const int boxWidth = 180;
            const int boxHeight = 60;
            const int boxPadding = 10;
            const int boxMargin = 10;

            var sceneView = SceneView.currentDrawingSceneView;

            var boxRect = new Rect(
                sceneView.camera.pixelRect.width - boxWidth - boxMargin,
                sceneView.camera.pixelRect.height - boxHeight - boxMargin,
                boxWidth,
                boxHeight);
            var areaRect = new Rect(
                sceneView.camera.pixelRect.width - boxWidth - boxMargin + boxPadding,
                sceneView.camera.pixelRect.height - boxHeight - boxMargin + boxPadding,
                boxWidth - boxPadding * 2,
                boxHeight - boxPadding * 2);

            Handles.BeginGUI();
            GUI.Box(boxRect, "Pixelpart Effect");
            GUILayout.BeginArea(areaRect);

            GUILayout.Space(20);

            GUILayout.BeginHorizontal();
            EditorGUI.BeginDisabledGroup(effect?.EffectAsset == null);
            if (GUILayout.Button("Restart"))
            {
                effect?.RestartEffect();
            }
            EditorGUI.EndDisabledGroup();
            GUILayout.EndHorizontal();

            GUILayout.EndArea();
            Handles.EndGUI();

            if (Event.current.type == EventType.Repaint)
            {
                effect.RenderEffect(Camera.current);
            }
        }

        private void OnUpdate()
        {
            var currentTime = EditorApplication.timeSinceStartup;
            var deltaTime = (float)(currentTime - prevUpdateTime);
            prevUpdateTime = currentTime;

            effect.AdvanceEffect(deltaTime);

            effect.UpdateEffectMesh(Camera.main);
            effect.RenderEffect(Camera.main);

            RepaintPreview();
        }

        private void RepaintPreview()
        {
            SceneView.RepaintAll();

            var editorWindowAssembly = typeof(EditorWindow).Assembly;
            var gameView = EditorWindow.GetWindow(editorWindowAssembly.GetType("UnityEditor.GameView"), false, null, false);
            gameView?.Repaint();
        }
    }
}
