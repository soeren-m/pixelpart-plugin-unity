using System;
using System.Text;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartGraphicsResourceProvider
    {
        public IReadOnlyDictionary<string, Texture2D> Textures => new ReadOnlyDictionary<string, Texture2D>(textures);
        private readonly Dictionary<string, Texture2D> textures = new Dictionary<string, Texture2D>();

        public IReadOnlyDictionary<string, Mesh> Meshes => new ReadOnlyDictionary<string, Mesh>(meshes);
        private readonly Dictionary<string, Mesh> meshes = new Dictionary<string, Mesh>();

        private enum ColorSpace
        {
            Linear = 0,
            SRGB = 1
        }

        public PixelpartGraphicsResourceProvider()
        {

        }

        public void Load(IntPtr effectRuntime)
        {
            Clear();

            var resourceIdBuffer = new byte[2048];

            var imageResourceCount = Plugin.PixelpartGetImageResourceCount(effectRuntime);

            for (var resourceIndex = 0; resourceIndex < imageResourceCount; resourceIndex++)
            {
                var resourceIdLength = Plugin.PixelpartGetImageResourceId(effectRuntime, resourceIndex, resourceIdBuffer, resourceIdBuffer.Length);
                var resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

                var imageWidth = Plugin.PixelpartGetImageResourceWidth(effectRuntime, resourceId);
                var imageHeight = Plugin.PixelpartGetImageResourceHeight(effectRuntime, resourceId);
                var imageChannels = Plugin.PixelpartGetImageResourceChannels(effectRuntime, resourceId);
                var imageColorSpace = (ColorSpace)Plugin.PixelpartGetImageResourceColorSpace(effectRuntime, resourceId);
                var imageDataSize = Plugin.PixelpartGetImageResourceDataSize(effectRuntime, resourceId);

                var imageData = new byte[imageDataSize];
                Plugin.PixelpartGetImageResourceData(effectRuntime, resourceId, imageData);

                var texture = new Texture2D(
                    imageWidth, imageHeight,
                    imageChannels == 3 ? TextureFormat.RGB24 : TextureFormat.RGBA32,
                    false, imageColorSpace == ColorSpace.Linear);
                texture.filterMode = FilterMode.Bilinear;
                texture.wrapMode = TextureWrapMode.Repeat;
                texture.LoadRawTextureData(imageData);
                texture.Apply();

                textures[resourceId] = texture;
            }

            var meshResourceCount = Plugin.PixelpartGetMeshResourceCount(effectRuntime);

            for (var resourceIndex = 0; resourceIndex < meshResourceCount; resourceIndex++)
            {
                var resourceIdLength = Plugin.PixelpartGetMeshResourceId(effectRuntime, resourceIndex, resourceIdBuffer, resourceIdBuffer.Length);
                var resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

                var indexCount = Plugin.PixelpartGetMeshResourceIndexCount(effectRuntime, resourceId);
                var vertexCount = Plugin.PixelpartGetMeshResourceVertexCount(effectRuntime, resourceId);

                var triangles = new int[indexCount];
                var vertices = new Vector3[vertexCount];
                var normals = new Vector3[vertexCount];
                var uv = new Vector2[vertexCount];
                Plugin.PixelpartGetMeshResourceVertexData(effectRuntime, resourceId, triangles, vertices, normals, uv);

                var mesh = new Mesh();
                mesh.vertices = vertices;
                mesh.normals = normals;
                mesh.uv = uv;
                mesh.triangles = triangles;

                meshes[resourceId] = mesh;
            }
        }

        public void Clear()
        {
            textures.Clear();
            meshes.Clear();
        }
    }
}
