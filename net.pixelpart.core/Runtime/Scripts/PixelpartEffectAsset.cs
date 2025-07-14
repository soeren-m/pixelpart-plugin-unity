using System;
using System.IO;
using System.Text;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart
{
    /// <summary>
    /// Asset for a Pixelpart effect that is contained in a <i>ppfx</i> file.
    /// </summary>
    /// <remarks>
    /// This class derives from <c>ScriptableObject</c> and stores the effect as a byte array.
    /// </remarks>
    public class PixelpartEffectAsset : ScriptableObject
    {
#if PIXELPART_USE_URP
        private const int renderPipelineId = 1;
#elif PIXELPART_USE_HDRP
        private const int renderPipelineId = 2;
#else
        private const int renderPipelineId = 0;
#endif

        /// <summary>
        /// Effect data as byte array.
        /// </summary>
        public byte[] Data = null;

        /// <summary>
        /// Information about custom materials if the effect contains any.
        /// </summary>
        public PixelpartMaterialDescriptor[] CustomMaterials = null;

        /// <summary>
        /// Load effect from a <i>ppfx</i> file.
        /// </summary>
        /// <param name="path">File to import</param>
        public void Load(string path)
        {
            Data = File.ReadAllBytes(path);
            CustomMaterials = null;

            try
            {
                var effectRuntime = LoadEffect();
                CreateCustomMaterialAssetDescriptors(effectRuntime, path);

                Plugin.PixelpartDeleteEffect(effectRuntime);
            }
            catch (InvalidOperationException e)
            {
                Data = null;
                Debug.LogError("[Pixelpart] Failed to import effect asset at \"" + path + "\": " + e.Message);
            }
        }

        /// <summary>
        /// Create internal representation of effect.
        /// </summary>
        /// <remarks>
        /// Only call after <see cref="Load"/>.
        /// </remarks>
        /// <returns>Pointer to internal effect runtime</returns>
        /// <exception cref="InvalidOperationException"></exception>
        public IntPtr LoadEffect()
        {
            if (Data == null || Data.Length < 1)
            {
                throw new InvalidOperationException("No data assigned to effect asset");
            }

            var errorBuffer = new byte[1024];
            var effectRuntime = Plugin.PixelpartLoadEffect(Data, Data.Length, errorBuffer, errorBuffer.Length, out int errorLength);
            if (effectRuntime == IntPtr.Zero)
            {
                throw new InvalidOperationException(Encoding.UTF8.GetString(errorBuffer, 0, errorLength));
            }

            return effectRuntime;
        }

        private void CreateCustomMaterialAssetDescriptors(IntPtr effectRuntime, string path)
        {
            var directory = Path.GetDirectoryName(path);
            var effectName = Path.GetFileNameWithoutExtension(path);

            var nameBuffer = new byte[2048];
            var shaderMainCodeBuffer = new byte[16384];
            var shaderParameterCodeBuffer = new byte[16384];
            var shaderParameterNamesBuffer = new byte[16384];
            var shaderParameterIdsBuffer = new uint[64];
            var shaderTextureResourceIdsBuffer = new byte[16384];
            var shaderSamplerNamesBuffer = new byte[16384];

            var materialCount = Plugin.PixelpartGetMaterialResourceCount(effectRuntime);
            CustomMaterials = new PixelpartMaterialDescriptor[materialCount * 2];

            for (var materialIndex = 0; materialIndex < materialCount; materialIndex++)
            {
                var nameSize = Plugin.PixelpartGetMaterialResourceId(effectRuntime, materialIndex, nameBuffer, nameBuffer.Length);
                var materialResourceId = Encoding.UTF8.GetString(nameBuffer, 0, nameSize);

                var blendMode = (BlendModeType)Plugin.PixelpartGetMaterialResourceBlendMode(effectRuntime, materialResourceId);
                var lightingMode = (LightingModeType)Plugin.PixelpartGetMaterialResourceLightingMode(effectRuntime, materialResourceId);

                var result = Plugin.PixelpartBuildMaterialShader(effectRuntime, materialResourceId, renderPipelineId,
                    shaderMainCodeBuffer, shaderParameterCodeBuffer,
                    shaderParameterNamesBuffer, shaderParameterIdsBuffer,
                    shaderTextureResourceIdsBuffer, shaderSamplerNamesBuffer,
                    out int shaderMainCodeLength, out int shaderParameterCodeLength,
                    out int shaderParameterNamesLength, out int shaderParameterIdsLength,
                    out int shaderTextureResourceIdsLength, out int shaderSamplerNamesLength,
                    shaderMainCodeBuffer.Length, shaderParameterCodeBuffer.Length,
                    shaderParameterNamesBuffer.Length, shaderParameterIdsBuffer.Length,
                    shaderTextureResourceIdsBuffer.Length, shaderSamplerNamesBuffer.Length);

                if (!result)
                {
                    Debug.LogWarning("[Pixelpart] Failed to generate shader for material \"" + materialResourceId + "\": " +
                        Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength));

                    continue;
                }

                var shaderParameterNames = Encoding.UTF8.GetString(shaderParameterNamesBuffer, 0, shaderParameterNamesLength).
                    Split(new char[] { ' ' }, 64, StringSplitOptions.RemoveEmptyEntries);
                var shaderTextureResourceIds = Encoding.UTF8.GetString(shaderTextureResourceIdsBuffer, 0, shaderTextureResourceIdsLength).
                    Split(new char[] { ' ' }, 8, StringSplitOptions.RemoveEmptyEntries);
                var shaderSamplerNames = Encoding.UTF8.GetString(shaderSamplerNamesBuffer, 0, shaderSamplerNamesLength).
                    Split(new char[] { ' ' }, 8, StringSplitOptions.RemoveEmptyEntries);

                var shaderParameterIds = new uint[shaderParameterIdsLength];
                Array.Copy(shaderParameterIdsBuffer, 0, shaderParameterIds, 0, shaderParameterIdsLength);

                var materialName = effectName.Replace(" ", "_") + "_" + materialResourceId.Replace(" ", "_");

                CustomMaterials[materialIndex * 2 + 0] = PixelpartMaterialDescriptor.CreateDescriptorForCustomMaterial(
                    Path.Combine(directory, materialName + ".mat"), materialResourceId,
                    false, blendMode, lightingMode,
                    shaderParameterIds, shaderParameterNames,
                    shaderTextureResourceIds, shaderSamplerNames);
                CustomMaterials[materialIndex * 2 + 1] = PixelpartMaterialDescriptor.CreateDescriptorForCustomMaterial(
                    Path.Combine(directory, materialName + "_Inst.mat"), materialResourceId,
                    true, blendMode, lightingMode,
                    shaderParameterIds, shaderParameterNames,
                    shaderTextureResourceIds, shaderSamplerNames);

#if UNITY_EDITOR
                var mainCode = Encoding.UTF8.GetString(shaderMainCodeBuffer, 0, shaderMainCodeLength);
                var parameterCode = Encoding.UTF8.GetString(shaderParameterCodeBuffer, 0, shaderParameterCodeLength);

                GenerateCustomShaderAsset(materialName, directory,
                    blendMode, lightingMode, false,
                    mainCode, parameterCode);
                GenerateCustomShaderAsset(materialName + "_Inst", directory,
                    blendMode, lightingMode, true,
                    mainCode, parameterCode);
#endif
            }
        }

#if UNITY_EDITOR
        private static void GenerateCustomShaderAsset(string materialName, string directory,
            BlendModeType blendMode, LightingModeType lightingMode, bool instanced,
            string mainCode, string parameterCode)
        {
            var shaderFilepath = Path.Combine(directory, materialName + ".shader");
            if (File.Exists(shaderFilepath))
            {
                return;
            }

            var shaderCode = PixelpartShaderGenerator.GenerateShaderCode(
                materialName, mainCode, parameterCode, blendMode, lightingMode, instanced);

            var writer = File.CreateText(shaderFilepath);
            writer.Write(shaderCode);
            writer.Close();

            AssetDatabase.ImportAsset(shaderFilepath);
        }
#endif
    }
}