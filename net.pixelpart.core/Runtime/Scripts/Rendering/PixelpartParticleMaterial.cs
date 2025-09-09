using System;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartParticleMaterial
    {
        enum MaterialParameterType
        {
            Int = 0,
            Float = 1,
            Float2 = 2,
            Float3 = 3,
            Float4 = 4,
            Bool = 5,
            Enum = 6,
            Color = 7,
            Curve = 8,
            Gradient = 9,
            ResourceImage = 10
        }

        public Material Material { get; }

        private readonly IntPtr effectRuntime;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly PixelpartGraphicsResourceProvider graphicsResourceProvider;

        private readonly PixelpartMaterialDescriptor materialDescriptor;

        public PixelpartParticleMaterial(IntPtr effectRuntimePtr, uint emitterId, uint typeId, Material baseMaterial, PixelpartMaterialDescriptor materialDesc, PixelpartGraphicsResourceProvider resourceProvider)
        {
            effectRuntime = effectRuntimePtr;
            particleEmitterId = emitterId;
            particleTypeId = typeId;
            graphicsResourceProvider = resourceProvider;

            materialDescriptor = materialDesc;
            Material = new Material(baseMaterial);

            if (!string.IsNullOrEmpty(materialDescriptor.ResourceId))
            {
                for (var samplerIndex = 0; samplerIndex < materialDescriptor.TextureResourceIds.Length; samplerIndex++)
                {
                    var samplerName = materialDescriptor.SamplerNames[samplerIndex];
                    var resourceId = materialDescriptor.TextureResourceIds[samplerIndex];

                    if (graphicsResourceProvider.Textures.TryGetValue(resourceId, out Texture2D texture))
                    {
                        Material.SetTexture(samplerName, texture);
                    }
                    else
                    {
                        Debug.LogError("[Pixelpart] Cannot find texture \"" + resourceId + "\"");
                    }
                }
            }
        }

        public void ApplyParameters()
        {
            var effectTime = Plugin.PixelpartGetEffectTime(effectRuntime);
            var objectTime = Plugin.PixelpartNodeGetLocalTime(effectRuntime, particleEmitterId);

            Material.SetFloat("_EffectTime", effectTime);
            Material.SetFloat("_ObjectTime", objectTime);

            var parameterCount = Plugin.PixelpartParticleTypeGetMaterialParameterCount(effectRuntime, particleTypeId);

            var parameterIds = new uint[parameterCount];
            Plugin.PixelpartParticleTypeGetMaterialParameterIds(effectRuntime, particleTypeId, parameterIds);

            foreach (var parameterId in parameterIds)
            {
                ApplyParameter(parameterId);
            }
        }

        private void ApplyParameter(uint parameterId)
        {
            var parameterName = materialDescriptor.GetParameterName(parameterId);
            if (parameterName == null)
            {
                return;
            }

            var parameterType = (MaterialParameterType)Plugin.PixelpartParticleTypeGetMaterialParameterType(effectRuntime, particleTypeId, parameterId);

            switch (parameterType)
            {
                case MaterialParameterType.Int:
                case MaterialParameterType.Enum:
                    Material.SetInt(parameterName,
                        Plugin.PixelpartParticleTypeGetMaterialParameterValueInt(effectRuntime, particleTypeId, parameterId));
                    break;

                case MaterialParameterType.Float:
                    Material.SetFloat(parameterName,
                        Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat(effectRuntime, particleTypeId, parameterId));
                    break;

                case MaterialParameterType.Float2:
                case MaterialParameterType.Float3:
                case MaterialParameterType.Float4:
                case MaterialParameterType.Color:
                    Material.SetVector(parameterName,
                        Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat4(effectRuntime, particleTypeId, parameterId));
                    break;

                case MaterialParameterType.Bool:
                    Material.SetInt(parameterName,
                        Plugin.PixelpartParticleTypeGetMaterialParameterValueBool(effectRuntime, particleTypeId, parameterId) ? 1 : 0);
                    break;

                case MaterialParameterType.ResourceImage:
                {
                    var resourceIdBuffer = new byte[256];
                    var resourceIdLength = Plugin.PixelpartParticleTypeGetMaterialParameterValueResourceId(effectRuntime, particleTypeId, parameterId, resourceIdBuffer, resourceIdBuffer.Length);
                    var imageResourceId = System.Text.Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

                    if (graphicsResourceProvider.Textures.TryGetValue(imageResourceId, out Texture2D texture))
                    {
                        Material.SetTexture(parameterName, texture);
                    }
                    else
                    {
                        Debug.LogError("[Pixelpart] Cannot find texture \"" + imageResourceId + "\"");
                    }

                    break;
                }

                default:
                    break;
            }
        }
    }
}
