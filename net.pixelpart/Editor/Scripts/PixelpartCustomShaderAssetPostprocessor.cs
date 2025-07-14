using System;
using System.IO;
using UnityEngine;
using UnityEditor;

namespace Pixelpart
{
    public class PixelpartCustomShaderAssetPostprocessor : AssetPostprocessor
    {
        public static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromPaths)
        {
            try
            {
                AssetDatabase.StartAssetEditing();

                foreach (var assetPath in importedAssets)
                {
                    if (Path.GetExtension(assetPath) != ".shader")
                    {
                        continue;
                    }

                    var fileReader = new StreamReader(assetPath);
                    var line1 = fileReader.ReadLine();
                    fileReader.Close();

                    if (string.IsNullOrEmpty(line1))
                    {
                        continue;
                    }

                    var tokens = line1.Split(new char[] { '\"' }, 3, StringSplitOptions.RemoveEmptyEntries);
                    if (tokens.Length < 2)
                    {
                        continue;
                    }

                    var shaderName = tokens[1];
                    var shaderNameTokens = shaderName.Split('/');
                    if (shaderNameTokens.Length < 2 || shaderNameTokens[0] != "PixelpartCustom")
                    {
                        continue;
                    }

                    var instancing = shaderNameTokens[1].EndsWith("_Inst", false, null);

                    var materialFilepath = Path.GetDirectoryName(assetPath);
                    materialFilepath = Path.Combine(materialFilepath, shaderNameTokens[1] + ".mat");

                    CreateMaterialAsset(materialFilepath, shaderName, instancing);
                }
            }
            finally
            {
                AssetDatabase.StopAssetEditing();
            }
        }

        private static void CreateMaterialAsset(string filepath, string shaderName, bool instancing)
        {
            if (File.Exists(filepath))
            {
                Debug.Log("[Pixelpart] Skipping material creation because material already exists");
                return;
            }

            var shader = Shader.Find(shaderName);
            if (shader == null)
            {
                Debug.LogError("[Pixelpart] Failed to find custom shader '" + shaderName + "'");
                return;
            }

            var material = new Material(shader);
            material.enableInstancing = instancing;

            AssetDatabase.CreateAsset(material, filepath);
            AssetDatabase.SaveAssets();
            AssetDatabase.ImportAsset(filepath);
        }
    }
}