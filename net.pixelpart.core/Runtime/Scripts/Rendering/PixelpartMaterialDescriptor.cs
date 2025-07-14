using System;

namespace Pixelpart
{
    [Serializable]
    public class PixelpartMaterialDescriptor
    {
        public string MaterialPath;

        public string ResourceId;

        public bool Instancing;

        public BlendModeType BlendMode;

        public LightingModeType LightingMode;

        public uint[] ParameterIds;

        public string[] ParameterNames;

        public string[] TextureResourceIds;

        public string[] SamplerNames;

        public PixelpartMaterialDescriptor(string materialPath, string resourceId,
            bool instancing, BlendModeType blendMode, LightingModeType lightingMode,
            uint[] parameterIds, string[] parameterNames,
            string[] textureResourceIds, string[] samplerNames)
        {
            MaterialPath = materialPath;
            ResourceId = resourceId;
            Instancing = instancing;
            BlendMode = blendMode;
            LightingMode = lightingMode;
            ParameterIds = parameterIds;
            ParameterNames = parameterNames;
            TextureResourceIds = textureResourceIds;
            SamplerNames = samplerNames;
        }

        public string GetParameterName(uint parameterId)
        {
            for (var parameterIndex = 0; parameterIndex < ParameterIds.Length && parameterIndex < ParameterNames.Length; parameterIndex++)
            {
                if (ParameterIds[parameterIndex] == parameterId)
                {
                    return ParameterNames[parameterIndex];
                }
            }

            return null;
        }

        public static PixelpartMaterialDescriptor CreateDescriptorForBuiltInMaterial(string materialPath, string resourceId,
            BlendModeType blendMode, LightingModeType lightingMode,
            uint[] parameterIds, string[] parameterNames)
        {
            return new PixelpartMaterialDescriptor(materialPath, resourceId,
                false, blendMode, lightingMode,
                parameterIds, parameterNames,
                new string[0], new string[0]);
        }

        public static PixelpartMaterialDescriptor CreateDescriptorForCustomMaterial(string materialPath, string resourceId,
            bool instancing, BlendModeType blendMode, LightingModeType lightingMode,
            uint[] parameterIds, string[] parameterNames,
            string[] textureResourceIds, string[] samplerNames)
        {
            return new PixelpartMaterialDescriptor(materialPath, resourceId,
                instancing, blendMode, lightingMode,
                parameterIds, parameterNames,
                textureResourceIds, samplerNames);
        }
    }
}