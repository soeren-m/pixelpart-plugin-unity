using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart
{
    internal class PixelpartParticleRenderer
    {
        private const int maxInstancesPerDrawCall = 1023;

        private readonly IntPtr effectRuntime;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly Mesh mesh;

        private readonly PixelpartParticleMaterial particleMaterial;

        private readonly MaterialPropertyBlock materialPropertyBlock;

        private readonly int[] bufferSizes;

        private int[] triangles = new int[1];
        private Vector3[] vertices = new Vector3[1];
        private Color[] colors = new Color[1];
        private Vector3[] normals = new Vector3[1];
        private Vector2[] uv0 = new Vector2[1];
        private Vector4[] uv1 = new Vector4[1];
        private Vector4[] uv2 = new Vector4[1];

        private Matrix4x4[] instanceTransforms = new Matrix4x4[1];
        private Vector4[] instanceColors = new Vector4[1];
        private Vector4[] instanceVelocities = new Vector4[1];
        private float[] instanceLives = new float[1];
        private float[] instanceIds = new float[1];

        private readonly Vector4[] drawCallColors = new Vector4[maxInstancesPerDrawCall];
        private readonly Vector4[] drawCallVelocities = new Vector4[maxInstancesPerDrawCall];
        private readonly float[] drawCallLives = new float[maxInstancesPerDrawCall];
        private readonly float[] drawCallIds = new float[maxInstancesPerDrawCall];

        public PixelpartParticleRenderer(IntPtr effectRuntimePtr, uint emitterId, uint typeId, Material baseMaterial, PixelpartMaterialDescriptor materialDescriptor, PixelpartGraphicsResourceProvider resourceProvider)
        {
            effectRuntime = effectRuntimePtr;
            particleEmitterId = emitterId;
            particleTypeId = typeId;

            var rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(effectRuntimePtr, typeId);

            switch (rendererType)
            {
                case ParticleRendererType.Sprite:
                case ParticleRendererType.Trail:
                {
                    mesh = new Mesh();
                    mesh.MarkDynamic();

                    bufferSizes = new int[7];

                    break;
                }

                case ParticleRendererType.Mesh:
                {
                    var meshResourceIdBuffer = new byte[512];
                    var meshResourceIdLength = Plugin.PixelpartParticleTypeGetMeshRendererMeshResourceId(effectRuntimePtr, typeId, meshResourceIdBuffer, meshResourceIdBuffer.Length);
                    var meshResourceId = System.Text.Encoding.UTF8.GetString(meshResourceIdBuffer, 0, meshResourceIdLength);

                    if (!resourceProvider.Meshes.TryGetValue(meshResourceId, out mesh))
                    {
                        Debug.LogError("[Pixelpart] Failed to find mesh \"" + meshResourceId + "\"");
                    }

                    bufferSizes = new int[5];

                    break;
                }

                default:
                {
                    bufferSizes = null;

                    break;
                }
            }

            particleMaterial = new PixelpartParticleMaterial(effectRuntimePtr, emitterId, typeId, baseMaterial, materialDescriptor, resourceProvider);
            materialPropertyBlock = new MaterialPropertyBlock();
        }

        public void Render(Camera camera, Transform effectTransform, Vector3 effectScale, int layer)
        {
            var visible = Plugin.PixelpartParticleTypeIsVisible(effectRuntime, particleTypeId);
            if (!visible || camera == null || mesh == null)
            {
                return;
            }

            var rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(effectRuntime, particleTypeId);
            var particleLayer = Plugin.PixelpartParticleTypeGetLayer(effectRuntime, particleTypeId);

            Plugin.PixelpartConstructParticleGeometry(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, effectScale,
                bufferSizes);

            if (bufferSizes.Any(size => size <= 0))
            {
                return;
            }

            var vertexGenerationResult = false;
            switch (rendererType)
            {
                case ParticleRendererType.Sprite:
                case ParticleRendererType.Trail:
                    vertexGenerationResult = GenerateVertexData(camera, effectScale);
                    break;
                case ParticleRendererType.Mesh:
                    vertexGenerationResult = GenerateInstanceData(camera, effectScale);
                    break;
                default:
                    break;
            }

            if (!vertexGenerationResult)
            {
                return;
            }

            particleMaterial.ApplyParameters();

            switch (rendererType)
            {
                case ParticleRendererType.Sprite:
                case ParticleRendererType.Trail:
                    RenderVertices(camera, effectTransform, layer, particleLayer);
                    break;
                case ParticleRendererType.Mesh:
                    RenderInstances(camera, effectTransform, layer, particleLayer);
                    break;
                default:
                    break;
            }
        }

        private bool GenerateVertexData(Camera camera, Vector3 effectScale)
        {
            Array.Resize(ref triangles, bufferSizes[0]);
            Array.Resize(ref vertices, bufferSizes[1]);
            Array.Resize(ref colors, bufferSizes[2]);
            Array.Resize(ref normals, bufferSizes[3]);
            Array.Resize(ref uv0, bufferSizes[4]);
            Array.Resize(ref uv1, bufferSizes[5]);
            Array.Resize(ref uv2, bufferSizes[6]);

            return Plugin.PixelpartGenerateParticleVertexData(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, effectScale,
                triangles, vertices, colors, normals, uv0, uv1, uv2);
        }

        private bool GenerateInstanceData(Camera camera, Vector3 effectScale)
        {
            Array.Resize(ref instanceTransforms, bufferSizes[0]);
            Array.Resize(ref instanceColors, bufferSizes[1]);
            Array.Resize(ref instanceVelocities, bufferSizes[2]);
            Array.Resize(ref instanceLives, bufferSizes[3]);
            Array.Resize(ref instanceIds, bufferSizes[4]);

            return Plugin.PixelpartGenerateParticleInstanceData(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, effectScale,
                instanceTransforms, instanceColors, instanceVelocities, instanceLives, instanceIds);
        }

        private void RenderVertices(Camera camera, Transform effectTransform, int layer, int particleLayer)
        {
            mesh.Clear();
            mesh.SetVertices(vertices, 0, vertices.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetColors(colors, 0, colors.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetNormals(normals, 0, normals.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(0, uv0, 0, uv0.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(1, uv1, 0, uv1.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(2, uv2, 0, uv2.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetTriangles(triangles, 0, false);

            var maxRelativeToTransform = mesh.bounds.max - effectTransform.position;
            var minRelativeToTransform = mesh.bounds.min - effectTransform.position;
            mesh.bounds = new Bounds(effectTransform.position, new Vector3(
                Mathf.Max(Mathf.Abs(minRelativeToTransform.x), Mathf.Abs(maxRelativeToTransform.x)) * 2.0f,
                Mathf.Max(Mathf.Abs(minRelativeToTransform.y), Mathf.Abs(maxRelativeToTransform.y)) * 2.0f,
                Mathf.Max(Mathf.Abs(minRelativeToTransform.z), Mathf.Abs(maxRelativeToTransform.z)) * 2.0f));

            mesh.MarkModified();

            var renderParams = new RenderParams(particleMaterial.Material)
            {
                camera = camera,
                layer = layer,
                receiveShadows = true,
                shadowCastingMode = ShadowCastingMode.Off,
                worldBounds = AdjustBoundsForParticleLayer(mesh.bounds, camera, particleLayer)
            };

            Graphics.RenderMesh(renderParams, mesh, 0, Matrix4x4.identity);
        }

        private void RenderInstances(Camera camera, Transform effectTransform, int layer, int particleLayer)
        {
            var extents = new Vector3(0.1f, 0.1f, 0.1f);
            foreach (var instanceTransform in instanceTransforms)
            {
                var scale = Mathf.Max(
                    instanceTransform.lossyScale.x,
                    instanceTransform.lossyScale.y,
                    instanceTransform.lossyScale.z);

                extents = Vector3.Max(extents, instanceTransform.GetPosition() - effectTransform.position + mesh.bounds.extents * scale);
            }

            var bounds = new Bounds(effectTransform.position, extents * 2.0f);
            var adjustedBounds = AdjustBoundsForParticleLayer(bounds, camera, particleLayer);

            var drawCallCount = (instanceTransforms.Length - 1) / maxInstancesPerDrawCall + 1;

            for (var drawCallIndex = 0; drawCallIndex < drawCallCount; drawCallIndex++)
            {
                var startIndex = drawCallIndex * maxInstancesPerDrawCall;
                var instanceCount = Math.Min(instanceTransforms.Length - startIndex, maxInstancesPerDrawCall);

                Array.Copy(instanceColors, startIndex, drawCallColors, 0, instanceCount);
                Array.Copy(instanceVelocities, startIndex, drawCallVelocities, 0, instanceCount);
                Array.Copy(instanceLives, startIndex, drawCallLives, 0, instanceCount);
                Array.Copy(instanceIds, startIndex, drawCallIds, 0, instanceCount);

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

                Graphics.RenderMeshInstanced(renderParams, mesh, 0, instanceTransforms, instanceCount, startIndex);
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
