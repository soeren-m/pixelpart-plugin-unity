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

        private readonly Vector4[] drawCallColors = new Vector4[maxInstancesPerDrawCall];
        private readonly Vector4[] drawCallVelocities = new Vector4[maxInstancesPerDrawCall];
        private readonly float[] drawCallLives = new float[maxInstancesPerDrawCall];
        private readonly float[] drawCallIds = new float[maxInstancesPerDrawCall];

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
            var meshResourceIdLength = Plugin.PixelpartParticleTypeGetMeshRendererMeshResourceId(effectRuntimePtr, typeId, meshResourceIdBuffer, meshResourceIdBuffer.Length);
            var meshResourceId = Encoding.UTF8.GetString(meshResourceIdBuffer, 0, meshResourceIdLength);

            if (!resourceProvider.Meshes.TryGetValue(meshResourceId, out mesh))
            {
                throw new InvalidOperationException("Failed to find mesh \"" + meshResourceId + "\"");
            }
        }

        public void Update(Camera camera, Transform transform, Vector3 scale)
        {
            Plugin.PixelpartConstructParticleGeometry(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, scale,
                bufferSizes);

            if (bufferSizes.Any(size => size <= 0))
            {
                return;
            }

            Array.Resize(ref instanceTransforms, bufferSizes[0]);
            Array.Resize(ref instanceColors, bufferSizes[1]);
            Array.Resize(ref instanceVelocities, bufferSizes[2]);
            Array.Resize(ref instanceLives, bufferSizes[3]);
            Array.Resize(ref instanceIds, bufferSizes[4]);

            Plugin.PixelpartGenerateParticleInstanceData(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, scale,
                instanceTransforms, instanceColors, instanceVelocities, instanceLives, instanceIds);
        }

        public void Render(Camera camera, Transform transform, int layer)
        {
            if (bufferSizes.Any(size => size <= 0))
            {
                return;
            }

            var visible = Plugin.PixelpartParticleTypeIsVisible(effectRuntime, particleTypeId);
            var particleLayer = Plugin.PixelpartParticleTypeGetLayer(effectRuntime, particleTypeId);
            if (!visible)
            {
                return;
            }

            particleMaterial.ApplyRuntimeParameters();

            var extents = new Vector3(0.1f, 0.1f, 0.1f);
            foreach (var instanceTransform in instanceTransforms)
            {
                var scale = Mathf.Max(
                    instanceTransform.lossyScale.x,
                    instanceTransform.lossyScale.y,
                    instanceTransform.lossyScale.z);

                extents = Vector3.Max(extents, instanceTransform.GetPosition() - transform.position + mesh.bounds.extents * scale);
            }

            var bounds = new Bounds(transform.position, extents * 2.0f);
            var adjustedBounds = AdjustBoundsForParticleLayer(bounds, camera, particleLayer);

            var drawCallCount = (instanceTransforms.Length - 1) / maxInstancesPerDrawCall + 1;

            for (var drawCallIndex = 0; drawCallIndex < drawCallCount; drawCallIndex++)
            {
                var instanceStartIndex = drawCallIndex * maxInstancesPerDrawCall;
                var instanceCount = Math.Min(instanceTransforms.Length - instanceStartIndex, maxInstancesPerDrawCall);

                Array.Copy(instanceColors, instanceStartIndex, drawCallColors, 0, instanceCount);
                Array.Copy(instanceVelocities, instanceStartIndex, drawCallVelocities, 0, instanceCount);
                Array.Copy(instanceLives, instanceStartIndex, drawCallLives, 0, instanceCount);
                Array.Copy(instanceIds, instanceStartIndex, drawCallIds, 0, instanceCount);

                materialPropertyBlock.SetVectorArray("_Color", drawCallColors);
                materialPropertyBlock.SetVectorArray("_Velocity", drawCallVelocities);
                materialPropertyBlock.SetFloatArray("_Life", drawCallLives);
                materialPropertyBlock.SetFloatArray("_ObjectId", drawCallIds);

                var renderParams = new RenderParams(particleMaterial.Material)
                {
                    camera = camera,
                    layer = layer,
                    matProps = materialPropertyBlock,
                    receiveShadows = true,
                    shadowCastingMode = ShadowCastingMode.Off,
                    worldBounds = adjustedBounds
                };

                Graphics.RenderMeshInstanced(renderParams, mesh, 0, instanceTransforms, instanceCount, instanceStartIndex);
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
