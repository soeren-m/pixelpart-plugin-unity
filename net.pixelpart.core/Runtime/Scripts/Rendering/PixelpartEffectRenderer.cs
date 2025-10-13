using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartEffectRenderer
    {
        private readonly IntPtr effectRuntime;

        private readonly PixelpartGraphicsResourceProvider graphicsResourceProvider;

        private readonly PixelpartParticleRenderer[] particleRenderers;

        private readonly PixelpartParticleEmissionPair[] particleEmissionPairs;

        private readonly int[] sortedParticleEmissionPairs;

        public PixelpartEffectRenderer(IntPtr effectRuntimePtr, IList<Material> particleMaterials, IList<PixelpartMaterialDescriptor> customMaterials)
        {
            effectRuntime = effectRuntimePtr;

            graphicsResourceProvider = new PixelpartGraphicsResourceProvider();
            graphicsResourceProvider.Load(effectRuntimePtr);

            var emissionPairCount = Plugin.PixelpartGetEffectParticleEmissionPairCount(effectRuntimePtr);

            particleRenderers = new PixelpartParticleRenderer[emissionPairCount];
            particleEmissionPairs = new PixelpartParticleEmissionPair[emissionPairCount];
            sortedParticleEmissionPairs = new int[emissionPairCount];
            Plugin.PixelpartGetEffectParticleEmissionPairs(effectRuntimePtr, particleEmissionPairs);

            var materialIdBuffer = new byte[2048];

            for (var emissionPairIndex = 0; emissionPairIndex < emissionPairCount; emissionPairIndex++)
            {
                var emissionPair = particleEmissionPairs[emissionPairIndex];
                var particleTypeIndex = Plugin.PixelpartParticleTypeGetIndex(effectRuntimePtr, emissionPair.TypeId);

                if (particleTypeIndex > particleMaterials.Count)
                {
                    Debug.LogWarning("[Pixelpart] Failed to find material for particle type with id " + emissionPair.TypeId);
                    continue;
                }

                var baseMaterial = particleMaterials[particleTypeIndex];

                var materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(effectRuntimePtr, emissionPair.TypeId, materialIdBuffer, materialIdBuffer.Length);
                var materialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
                var materialBuiltIn = Plugin.PixelpartParticleTypeIsMaterialBuiltIn(effectRuntimePtr, emissionPair.TypeId);

                PixelpartMaterialDescriptor materialDescriptor = null;
                if (materialBuiltIn)
                {
                    if (!PixelpartBuiltInMaterialProvider.Instance.BuiltInMaterials.TryGetValue(materialId, out materialDescriptor))
                    {
                        materialDescriptor = null;
                    }
                }
                else
                {
                    materialDescriptor = customMaterials.FirstOrDefault(desc => desc.ResourceId == materialId);
                }

                if (materialDescriptor == null)
                {
                    Debug.LogWarning("[Pixelpart] Failed to find material information for \"" + materialId + "\"");
                    continue;
                }

                particleRenderers[emissionPairIndex] = new PixelpartParticleRenderer(effectRuntimePtr,
                    emissionPair.EmitterId, emissionPair.TypeId,
                    baseMaterial, materialDescriptor, graphicsResourceProvider);
            }
        }

        public void Render(Camera camera, Transform transform, Vector3 effectScale, int layer)
        {
            if (particleEmissionPairs.Length == 0)
            {
                return;
            }

            Plugin.PixelpartGetSortedParticleEmissionPairs(effectRuntime, sortedParticleEmissionPairs);

            foreach (var index in sortedParticleEmissionPairs)
            {
                if (particleRenderers[index] == null)
                {
                    continue;
                }

                particleRenderers[index].Render(camera, effectScale, layer);
            }
        }
    }
}
