using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartGpuParticleRenderer
    {
        private readonly PixelpartGpuEffectRuntime gpuEffectRuntime;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly Material material;

        public PixelpartGpuParticleRenderer(PixelpartGpuEffectRuntime effectRuntime, uint emitterId, uint typeId)
        {
            gpuEffectRuntime = effectRuntime;
            particleEmitterId = emitterId;
            particleTypeId = typeId;

            // TODO: material
        }

        public void Render(Camera camera, Transform transform, int layer)
        {
            material.SetBuffer("particleBuffer", gpuEffectRuntime.GetParticleBuffer(particleEmitterId, particleTypeId));

            var particleCount = gpuEffectRuntime.GetParticleCount(particleEmitterId, particleTypeId);

            // TODO
            var bounds = new Bounds(transform.position, 100*Vector3.one);
            var renderParams = new RenderParams(material)
            {
                camera = camera,
                layer = layer,
                receiveShadows = true,
                shadowCastingMode = ShadowCastingMode.Off,
                worldBounds = AdjustBoundsForParticleLayer(bounds, camera, particleLayer)
            };

            // TODO: render sprites/trails/meshes
            Graphics.RenderPrimitives(renderParams, MeshTopology.Points, 1, particleCount);
        }

        private static Bounds AdjustBoundsForParticleLayer(Bounds bounds, Camera camera, int particleLayer)
        {
            var meshToCamera = camera.transform.position - bounds.center;

            return new Bounds(
                bounds.center + meshToCamera * (particleLayer * 0.01f),
                bounds.size);
        }
    }
}
