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

        private readonly Matrix4x4[] drawCallTransforms = new Matrix4x4[maxInstancesPerDrawCall];
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

        public void Render(Camera camera, Vector3 effectScale, int layer)
        {
            var visible = Plugin.PixelpartParticleTypeIsVisible(effectRuntime, particleTypeId);
            var rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(effectRuntime, particleTypeId);
            if (!visible || camera == null || mesh == null)
            {
                return;
            }

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
                    RenderVertices(layer);
                    break;
                case ParticleRendererType.Mesh:
                    RenderInstances(layer);
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

        private void RenderVertices(int layer)
        {
            mesh.Clear();
            mesh.SetVertices(vertices, 0, vertices.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetColors(colors, 0, colors.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetNormals(normals, 0, normals.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(0, uv0, 0, uv0.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(1, uv1, 0, uv1.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(2, uv2, 0, uv2.Length, MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetTriangles(triangles, 0, false);

            var boundingBoxMin = mesh.bounds.min;
            var boundingBoxMax = mesh.bounds.max;
            mesh.bounds = new Bounds(Vector3.zero, new Vector3(
                Mathf.Max(Mathf.Abs(boundingBoxMin.x), Mathf.Abs(boundingBoxMax.x)) * 2.0f,
                Mathf.Max(Mathf.Abs(boundingBoxMin.y), Mathf.Abs(boundingBoxMax.y)) * 2.0f,
                Mathf.Max(Mathf.Abs(boundingBoxMin.z), Mathf.Abs(boundingBoxMax.z)) * 2.0f));

            mesh.MarkModified();

            Graphics.DrawMesh(mesh, Matrix4x4.identity,
                particleMaterial.Material, layer, null, 0, null, ShadowCastingMode.Off, false, null, false);
        }

        private void RenderInstances(int layer)
        {
            var drawCallCount = (instanceTransforms.Length - 1) / maxInstancesPerDrawCall + 1;

            for (var drawCallIndex = 0; drawCallIndex < drawCallCount; drawCallIndex++)
            {
                var startIndex = drawCallIndex * maxInstancesPerDrawCall;
                var instanceCount = Math.Min(instanceTransforms.Length - startIndex, maxInstancesPerDrawCall);

                Array.Copy(instanceTransforms, startIndex, drawCallTransforms, 0, instanceCount);
                Array.Copy(instanceColors, startIndex, drawCallColors, 0, instanceCount);
                Array.Copy(instanceVelocities, startIndex, drawCallVelocities, 0, instanceCount);
                Array.Copy(instanceLives, startIndex, drawCallLives, 0, instanceCount);
                Array.Copy(instanceIds, startIndex, drawCallIds, 0, instanceCount);

                materialPropertyBlock.SetVectorArray("_Color", drawCallColors);
                materialPropertyBlock.SetVectorArray("_Velocity", drawCallVelocities);
                materialPropertyBlock.SetFloatArray("_Life", drawCallLives);
                materialPropertyBlock.SetFloatArray("_ObjectId", drawCallIds);

                Graphics.DrawMeshInstanced(mesh, 0,
                    particleMaterial.Material, drawCallTransforms, instanceCount, materialPropertyBlock, ShadowCastingMode.Off, false, layer, null);
            }
        }
    }
}
