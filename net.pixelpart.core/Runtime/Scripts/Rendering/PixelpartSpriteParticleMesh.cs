using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart
{
    internal class PixelpartSpriteParticleMesh : PixelpartParticleMesh
    {
        private readonly IntPtr effectRuntime;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly PixelpartParticleMaterial particleMaterial;

        private readonly Mesh mesh = new Mesh();

        private readonly int[] bufferSizes = new int[7];

        private int[] triangles = new int[1];
        private Vector3[] vertices = new Vector3[1];
        private Color[] colors = new Color[1];
        private Vector3[] normals = new Vector3[1];
        private Vector2[] uv0 = new Vector2[1];
        private Vector4[] uv1 = new Vector4[1];
        private Vector4[] uv2 = new Vector4[1];

        public PixelpartSpriteParticleMesh(IntPtr effectRuntimePtr, uint emitterId, uint typeId,
            Material baseMaterial, PixelpartMaterialDescriptor materialDescriptor,
            PixelpartGraphicsResourceProvider resourceProvider)
        {
            effectRuntime = effectRuntimePtr;
            particleEmitterId = emitterId;
            particleTypeId = typeId;

            particleMaterial = new PixelpartParticleMaterial(effectRuntimePtr, emitterId, typeId,
                baseMaterial, materialDescriptor, resourceProvider);

            mesh.MarkDynamic();
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

            PixelpartArrayUtil.EnsureMinSize(ref triangles, bufferSizes[0]);
            PixelpartArrayUtil.EnsureMinSize(ref vertices, bufferSizes[1]);
            PixelpartArrayUtil.EnsureMinSize(ref colors, bufferSizes[2]);
            PixelpartArrayUtil.EnsureMinSize(ref normals, bufferSizes[3]);
            PixelpartArrayUtil.EnsureMinSize(ref uv0, bufferSizes[4]);
            PixelpartArrayUtil.EnsureMinSize(ref uv1, bufferSizes[5]);
            PixelpartArrayUtil.EnsureMinSize(ref uv2, bufferSizes[6]);

            var generationResult = Plugin.PixelpartGenerateParticleVertexData(effectRuntime, particleEmitterId, particleTypeId,
                camera.transform.position, camera.transform.forward, camera.transform.right, camera.transform.up, scale,
                triangles, vertices, colors, normals, uv0, uv1, uv2);

            if (!generationResult)
            {
                return;
            }

            mesh.Clear();
            mesh.SetVertices(vertices, 0, bufferSizes[1], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetColors(colors, 0, bufferSizes[2], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetNormals(normals, 0, bufferSizes[3], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(0, uv0, 0, bufferSizes[4], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(1, uv1, 0, bufferSizes[5], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetUVs(2, uv2, 0, bufferSizes[6], MeshUpdateFlags.DontNotifyMeshUsers);
            mesh.SetTriangles(triangles, 0, bufferSizes[0], 0, false, 0);

            var maxRelativeToTransform = mesh.bounds.max - transform.position;
            var minRelativeToTransform = mesh.bounds.min - transform.position;
            mesh.bounds = new Bounds(transform.position, new Vector3(
                Mathf.Max(Mathf.Abs(minRelativeToTransform.x), Mathf.Abs(maxRelativeToTransform.x)) * 2.0f,
                Mathf.Max(Mathf.Abs(minRelativeToTransform.y), Mathf.Abs(maxRelativeToTransform.y)) * 2.0f,
                Mathf.Max(Mathf.Abs(minRelativeToTransform.z), Mathf.Abs(maxRelativeToTransform.z)) * 2.0f));

            mesh.MarkModified();
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

        private static Bounds AdjustBoundsForParticleLayer(Bounds bounds, Camera camera, int particleLayer)
        {
            var meshToCamera = camera.transform.position - bounds.center;

            return new Bounds(
                bounds.center + meshToCamera * (particleLayer * 0.01f),
                bounds.size);
        }
    }
}
