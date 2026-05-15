using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartEffectRenderer
    {
        private readonly IntPtr effectRuntime;

        private readonly PixelpartGraphicsResourceProvider graphicsResourceProvider;

        private readonly PixelpartParticleMesh[] particleMeshes;

        private readonly PixelpartParticleEmissionPair[] particleEmissionPairs;

        private readonly int[] sortedParticleEmissionPairs;

        public PixelpartEffectRenderer(IntPtr effectRuntimePtr, IList<Material> particleMaterials, IList<PixelpartMaterialDescriptor> customMaterials)
        {
            effectRuntime = effectRuntimePtr;

            graphicsResourceProvider = new PixelpartGraphicsResourceProvider();
            graphicsResourceProvider.Load(effectRuntimePtr);

            var emissionPairCount = PixelpartPlugin.PixelpartGetEffectParticleEmissionPairCount(effectRuntimePtr);

            particleMeshes = new PixelpartParticleMesh[emissionPairCount];
            particleEmissionPairs = new PixelpartParticleEmissionPair[emissionPairCount];
            sortedParticleEmissionPairs = new int[emissionPairCount];
            PixelpartPlugin.PixelpartGetEffectParticleEmissionPairs(effectRuntimePtr, particleEmissionPairs);

            var materialIdBuffer = new byte[2048];

            for (var emissionPairIndex = 0; emissionPairIndex < emissionPairCount; emissionPairIndex++)
            {
                var emissionPair = particleEmissionPairs[emissionPairIndex];
                var particleTypeIndex = PixelpartPlugin.PixelpartParticleTypeGetIndex(effectRuntimePtr, emissionPair.TypeId);

                if (particleTypeIndex > particleMaterials.Count)
                {
                    Debug.LogWarning("[Pixelpart] Failed to find material for particle type with id " + emissionPair.TypeId);
                    continue;
                }

                var baseMaterial = particleMaterials[particleTypeIndex];

                var materialIdLength = PixelpartPlugin.PixelpartParticleTypeGetMaterialId(effectRuntimePtr, emissionPair.TypeId, materialIdBuffer, materialIdBuffer.Length);
                var materialId = Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
                var materialBuiltIn = PixelpartPlugin.PixelpartParticleTypeIsMaterialBuiltIn(effectRuntimePtr, emissionPair.TypeId);

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

                var rendererType = (PixelpartParticleType.ParticleRendererType)PixelpartPlugin.PixelpartParticleTypeGetRenderer(effectRuntimePtr, emissionPair.TypeId);

                try
                {
                    switch (rendererType)
                    {
                        case PixelpartParticleType.ParticleRendererType.Sprite:
                        case PixelpartParticleType.ParticleRendererType.Trail:
                            particleMeshes[emissionPairIndex] = new PixelpartSpriteParticleMesh(effectRuntimePtr,
                                emissionPair.EmitterId, emissionPair.TypeId,
                                baseMaterial, materialDescriptor, graphicsResourceProvider);
                            break;
                        case PixelpartParticleType.ParticleRendererType.Mesh:
                            particleMeshes[emissionPairIndex] = new PixelpartMeshParticleMesh(effectRuntimePtr,
                                emissionPair.EmitterId, emissionPair.TypeId,
                                baseMaterial, materialDescriptor, graphicsResourceProvider);
                            break;
                        default:
                            Debug.LogWarning("[Pixelpart] Unknown particle renderer type");
                            break;
                    }
                }
                catch (Exception e)
                {
                    Debug.LogWarning("[Pixelpart] Exception while creating particle mesh: " + e.Message);
                    particleMeshes[emissionPairIndex] = null;
                }
            }
        }

        public void UpdateMesh(Camera camera, Transform transform, Vector3 scale)
        {
            foreach (var particleMesh in particleMeshes)
            {
                particleMesh?.Update(camera, transform, scale);
            }
        }

        public void Render(Camera camera, Transform transform, int layer)
        {
            foreach (var particleMesh in particleMeshes)
            {
                if (camera != null)
                {
                    particleMesh?.Render(camera, transform, layer);
                }
                else
                {
                    foreach (var cam in Camera.allCameras)
                    {
                        particleMesh?.Render(cam, transform, layer);
                    }
                }
            }
        }
    }
}
