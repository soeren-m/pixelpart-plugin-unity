using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart
{
    internal class PixelpartParticleTrailRenderer : IPixelpartParticleRenderer
    {
        private readonly IntPtr internalEffect;

        private readonly uint particleEmitterId;

        private readonly uint particleTypeId;

        private readonly Mesh mesh;

        private readonly PixelpartParticleMaterial particleMaterial;

        private int[] triangles = new int[12];
        private Vector3[] vertices = new Vector3[2];
        private Color[] colors = new Color[2];
        private Vector3[] normals = new Vector3[2];
        private Vector2[] uv0 = new Vector2[2];
        private Vector4[] uv1 = new Vector4[2];
        private Vector4[] uv2 = new Vector4[2];

        public PixelpartParticleTrailRenderer(IntPtr effectRuntimePtr, uint emitterId, uint typeId, Material baseMaterial, PixelpartMaterialDescriptor materialDescriptor, PixelpartGraphicsResourceProvider resourceProvider)
        {
            internalEffect = effectRuntimePtr;
            particleEmitterId = emitterId;
            particleTypeId = typeId;

            mesh = new Mesh();
            mesh.MarkDynamic();

            particleMaterial = new PixelpartParticleMaterial(effectRuntimePtr, emitterId, typeId, baseMaterial, materialDescriptor, resourceProvider);
        }

        public void Render(Camera camera, Transform transform, Vector3 effectScale, int layer)
        {
            var visible = Plugin.PixelpartParticleTypeIsVisible(internalEffect, particleTypeId);
            if (!visible || camera == null)
            {
                return;
            }

            Plugin.PixelpartPrepareParticleTrailVertexData(internalEffect, particleEmitterId, particleTypeId, out int triangleCount, out int vertexCount);
            if (triangleCount == 0 || vertexCount == 0)
            {
                return;
            }

            Array.Resize(ref triangles, triangleCount * 3);
            Array.Resize(ref vertices, vertexCount);
            Array.Resize(ref colors, vertexCount);
            Array.Resize(ref normals, vertexCount);
            Array.Resize(ref uv0, vertexCount);
            Array.Resize(ref uv1, vertexCount);
            Array.Resize(ref uv2, vertexCount);

            var result = Plugin.PixelpartGetParticleTrailVertexData(internalEffect,
                particleEmitterId, particleTypeId,
                camera.transform.position,
                camera.transform.right,
                camera.transform.up,
                effectScale,
                triangles, vertices, colors, normals,
                uv0, uv1, uv2);
            if (!result)
            {
                return;
            }

            particleMaterial.ApplyParameters();

            mesh.Clear();
            mesh.vertices = vertices;
            mesh.colors = colors;
            mesh.normals = normals;
            mesh.uv = uv0;
            mesh.SetUVs(1, uv1.ToList());
            mesh.SetUVs(2, uv2.ToList());
            mesh.triangles = triangles;

            var boundingBoxMin = mesh.bounds.min;
            var boundingBoxMax = mesh.bounds.max;
            mesh.bounds = new Bounds(Vector3.zero, new Vector3(
                Mathf.Max(Mathf.Abs(boundingBoxMin.x), Mathf.Abs(boundingBoxMax.x)) * 2.0f,
                Mathf.Max(Mathf.Abs(boundingBoxMin.y), Mathf.Abs(boundingBoxMax.y)) * 2.0f,
                Mathf.Max(Mathf.Abs(boundingBoxMin.z), Mathf.Abs(boundingBoxMax.z)) * 2.0f));

            Graphics.DrawMesh(mesh, Matrix4x4.identity,
                particleMaterial.Material, layer, null, 0, null, ShadowCastingMode.Off, false, null, false);
        }
    }
}