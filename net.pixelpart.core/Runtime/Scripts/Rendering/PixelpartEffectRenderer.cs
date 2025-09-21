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

        private readonly PixelpartParticleRuntimeId[] particleRuntimeIds;

        private readonly int[] sortedParticleRuntimeIndices;

        public PixelpartEffectRenderer(IntPtr effectRuntimePtr, IList<Material> particleMaterials, IList<PixelpartMaterialDescriptor> customMaterials)
        {
            effectRuntime = effectRuntimePtr;

            graphicsResourceProvider = new PixelpartGraphicsResourceProvider();
            graphicsResourceProvider.Load(effectRuntimePtr);

            var runtimeInstanceCount = Plugin.PixelpartGetEffectParticleRuntimeInstanceCount(effectRuntimePtr);

            particleRenderers = new PixelpartParticleRenderer[runtimeInstanceCount];
            particleRuntimeIds = new PixelpartParticleRuntimeId[runtimeInstanceCount];
            sortedParticleRuntimeIndices = new int[runtimeInstanceCount];
            Plugin.PixelpartGetEffectParticleRuntimeInstances(effectRuntimePtr, particleRuntimeIds);

            var materialIdBuffer = new byte[2048];

            for (var runtimeInstanceIndex = 0; runtimeInstanceIndex < runtimeInstanceCount; runtimeInstanceIndex++)
            {
                var runtimeId = particleRuntimeIds[runtimeInstanceIndex];
                var particleTypeIndex = Plugin.PixelpartParticleTypeGetIndex(effectRuntimePtr, runtimeId.TypeId);

                if (particleTypeIndex > particleMaterials.Count)
                {
                    Debug.LogWarning("[Pixelpart] Failed to find material for particle type with id " + runtimeId.TypeId);
                    continue;
                }

                var baseMaterial = particleMaterials[particleTypeIndex];

                var materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(effectRuntimePtr, runtimeId.TypeId, materialIdBuffer, materialIdBuffer.Length);
                var materialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
                var materialBuiltIn = Plugin.PixelpartParticleTypeIsMaterialBuiltIn(effectRuntimePtr, runtimeId.TypeId);

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

                particleRenderers[runtimeInstanceIndex] = new PixelpartParticleRenderer(effectRuntimePtr,
                    runtimeId.EmitterId, runtimeId.TypeId,
                    baseMaterial, materialDescriptor, graphicsResourceProvider);
            }
        }

        public void Render(Camera camera, Transform transform, Vector3 effectScale, int layer)
        {
            if (particleRuntimeIds.Length == 0)
            {
                return;
            }

            Plugin.PixelpartGetSortedParticleRuntimeInstances(effectRuntime, sortedParticleRuntimeIndices);

            foreach (var index in sortedParticleRuntimeIndices)
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
