using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartGpuEffectRenderer
    {
        private readonly PixelpartGpuEffectRuntime gpuEffectRuntime;

        private readonly PixelpartParticleEmissionPair[] particleEmissionPairs;

        private readonly PixelpartGpuParticleRenderer[] particleRenderers;

        public PixelpartGpuEffectRenderer(PixelpartGpuEffectRuntime effectRuntime, IList<Material> particleMaterials, IList<PixelpartMaterialDescriptor> customMaterials)
        {
            gpuEffectRuntime = effectRuntime;

            var emissionPairCount = PixelpartPlugin.PixelpartGetEffectParticleEmissionPairCount(effectRuntimePtr);

            particleEmissionPairs = new PixelpartParticleEmissionPair[emissionPairCount];
            PixelpartPlugin.PixelpartGetEffectParticleEmissionPairs(effectRuntimePtr, particleEmissionPairs);

            particleRenderers = new PixelpartGpuParticleRenderer[emissionPairCount];

            for (var emissionPairIndex = 0; emissionPairIndex < emissionPairCount; emissionPairIndex++)
            {
                var emissionPair = particleEmissionPairs[emissionPairIndex];

                particleRenderers[emissionPairIndex] = new PixelpartGpuParticleRenderer(
                    effectRuntime, emissionPair.EmitterId, emissionPair.TypeId);
            }
        }

        public void Render(Camera camera, Transform transform, int layer)
        {
            foreach (var particleRenderer in particleRenderers)
            {
                if (camera != null)
                {
                    particleRenderer?.Render(camera, transform, layer);
                }
                else
                {
                    foreach (var cam in Camera.allCameras)
                    {
                        particleRenderer?.Render(cam, transform, layer);
                    }
                }
            }
        }
    }
}
