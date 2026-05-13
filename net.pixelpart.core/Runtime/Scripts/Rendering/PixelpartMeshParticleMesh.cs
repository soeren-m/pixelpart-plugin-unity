using System;
using System.Linq;
using System.Text;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart
{
    internal class PixelpartMeshParticleMesh : PixelpartParticleMesh
    {
        private const int maxInstancesPerDrawCall = 1023;

        private readonly IntPtr effectRuntime;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly PixelpartParticleMaterial particleMaterial;

        private readonly Mesh mesh;

        private readonly MaterialPropertyBlock materialPropertyBlock = new MaterialPropertyBlock();

        private readonly int[] bufferSizes = new int[5];

        private Matrix4x4[] instanceTransforms = new Matrix4x4[1];
        private Vector4[] instanceColors = new Vector4[1];
        private Vector4[] instanceVelocities = new Vector4[1];
        private float[] instanceLives = new float[1];
        private float[] instanceIds = new float[1];

        private readonly Vector4[] batchColors = new Vector4[maxInstancesPerDrawCall];
        private readonly Vector4[] batchVelocities = new Vector4[maxInstancesPerDrawCall];
        private readonly float[] batchLives = new float[maxInstancesPerDrawCall];
        private readonly float[] batchIds = new float[maxInstancesPerDrawCall];

        public PixelpartMeshParticleMesh(IntPtr effectRuntimePtr, uint emitterId, uint typeId,
            Material baseMaterial, PixelpartMaterialDescriptor materialDescriptor,
            PixelpartGraphicsResourceProvider resourceProvider)
        {
            effectRuntime = effectRuntimePtr;
            particleEmitterId = emitterId;
            particleTypeId = typeId;

            particleMaterial = new PixelpartParticleMaterial(effectRuntimePtr, emitterId, typeId,
                baseMaterial, materialDescriptor, resourceProvider);

            var meshResourceIdBuffer = new byte[512];
            var meshResourceIdLength = PixelpartPlugin.PixelpartParticleTypeGetMeshRendererMeshResourceId(effectRuntimePtr, typeId, meshResourceIdBuffer, meshResourceIdBuffer.Length);
            var meshResourceId = Encoding.UTF8.GetString(meshResourceIdBuffer, 0, meshResourceIdLength);

            if (!resourceProvider.Meshes.TryGetValue(meshResourceId, out mesh))
            {
                throw new InvalidOperationException("Failed to find mesh \"" + meshResourceId + "\"");
            }
        }

        public void Update(Camera camera, Transform transform, Vector3 scale)
        {
            PixelpartPlugin.PixelpartConstructParticleGeometry(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, scale,
                bufferSizes);

            if (bufferSizes.Any(size => size <= 0))
            {
                return;
            }

            PixelpartArrayUtil.EnsureMinSize(ref instanceTransforms, bufferSizes[0]);
            PixelpartArrayUtil.EnsureMinSize(ref instanceColors, bufferSizes[1]);
            PixelpartArrayUtil.EnsureMinSize(ref instanceVelocities, bufferSizes[2]);
            PixelpartArrayUtil.EnsureMinSize(ref instanceLives, bufferSizes[3]);
            PixelpartArrayUtil.EnsureMinSize(ref instanceIds, bufferSizes[4]);

            PixelpartPlugin.PixelpartGenerateParticleInstanceData(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, scale,
                instanceTransforms, instanceColors, instanceVelocities, instanceLives, instanceIds);
        }

        public void Render(Camera camera, Transform transform, int layer)
        {
            var instanceCount = bufferSizes[0];
            if (instanceCount <= 0)
            {
                return;
            }

            var visible = PixelpartPlugin.PixelpartParticleTypeIsVisible(effectRuntime, particleTypeId);
            var particleLayer = PixelpartPlugin.PixelpartParticleTypeGetLayer(effectRuntime, particleTypeId);
            if (!visible)
            {
                return;
            }

            particleMaterial.ApplyRuntimeParameters();

            var extents = new Vector3(0.1f, 0.1f, 0.1f);
            for (var index = 0; index < instanceCount; index++)
            {
                var instanceTransform = instanceTransforms[index];
                var scale = Mathf.Max(
                    instanceTransform.lossyScale.x,
                    instanceTransform.lossyScale.y,
                    instanceTransform.lossyScale.z);

                extents = Vector3.Max(extents, instanceTransform.GetPosition() - transform.position + mesh.bounds.extents * scale);
            }

            var bounds = new Bounds(transform.position, extents * 2.0f);
            var adjustedBounds = AdjustBoundsForParticleLayer(bounds, camera, particleLayer);

            var batchCount = (instanceCount - 1) / maxInstancesPerDrawCall + 1;

            for (var batchIndex = 0; batchIndex < batchCount; batchIndex++)
            {
                var batchStartIndex = batchIndex * maxInstancesPerDrawCall;
                var batchSize = Math.Min(instanceCount - batchStartIndex, maxInstancesPerDrawCall);

                Array.Copy(instanceColors, batchStartIndex, batchColors, 0, batchSize);
                Array.Copy(instanceVelocities, batchStartIndex, batchVelocities, 0, batchSize);
                Array.Copy(instanceLives, batchStartIndex, batchLives, 0, batchSize);
                Array.Copy(instanceIds, batchStartIndex, batchIds, 0, batchSize);

                materialPropertyBlock.SetVectorArray("_Color", batchColors);
                materialPropertyBlock.SetVectorArray("_Velocity", batchVelocities);
                materialPropertyBlock.SetFloatArray("_Life", batchLives);
                materialPropertyBlock.SetFloatArray("_ObjectId", batchIds);

                var renderParams = new RenderParams(particleMaterial.Material)
                {
                    camera = camera,
                    layer = layer,
                    matProps = materialPropertyBlock,
                    receiveShadows = true,
                    shadowCastingMode = ShadowCastingMode.Off,
                    worldBounds = adjustedBounds
                };

                Graphics.RenderMeshInstanced(renderParams, mesh, 0, instanceTransforms, batchSize, batchStartIndex);
            }
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
