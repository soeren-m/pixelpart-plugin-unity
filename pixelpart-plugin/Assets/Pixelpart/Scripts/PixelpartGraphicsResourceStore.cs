using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;

namespace Pixelpart {
internal class PixelpartGraphicsResourceStore {
	public Dictionary<string, Texture2D> Textures { get; }

	public Dictionary<string, Mesh> Meshes { get; }

	public PixelpartGraphicsResourceStore() {
		Textures = new Dictionary<string, Texture2D>();
		Meshes = new Dictionary<string, Mesh>();
	}

	public void Load(IntPtr nativeEffect) {
		Clear();

		byte[] resourceIdBuffer = new byte[2048];

		uint numImageResources = Plugin.PixelpartGetImageResourceCount(nativeEffect);

		for(uint i = 0; i < numImageResources; i++) {
			int resourceIdLength = Plugin.PixelpartGetImageResourceId(nativeEffect, i, resourceIdBuffer, resourceIdBuffer.Length);
			string resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

			int imageWidth = Plugin.PixelpartGetImageResourceWidth(nativeEffect, resourceId);
			int imageHeight = Plugin.PixelpartGetImageResourceHeight(nativeEffect, resourceId);
			uint imageDataSize = Plugin.PixelpartGetImageResourceDataSize(nativeEffect, resourceId);
			byte[] imageData = new byte[imageDataSize];
			Plugin.PixelpartGetImageResourceData(nativeEffect, resourceId, imageData);

			Texture2D texture = new Texture2D(imageWidth, imageHeight, TextureFormat.RGBA32, false);
			texture.filterMode = FilterMode.Bilinear;
			texture.wrapMode = TextureWrapMode.Repeat;
			texture.LoadRawTextureData(imageData);
			texture.Apply();

			Textures[resourceId] = texture;
		}

		uint numMeshResources = Plugin.PixelpartGetMeshResourceCount(nativeEffect);

		for(uint i = 0; i < numMeshResources; i++) {
			int resourceIdLength = Plugin.PixelpartGetMeshResourceId(nativeEffect, i, resourceIdBuffer, resourceIdBuffer.Length);
			string resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

			int indexCount = Plugin.PixelpartGetMeshResourceIndexCount(nativeEffect, resourceId);
			int vertexCount = Plugin.PixelpartGetMeshResourceVertexCount(nativeEffect, resourceId);

			int[] triangles = new int[indexCount];
			Vector3[] vertices = new Vector3[vertexCount];
			Vector3[] normals = new Vector3[vertexCount];
			Vector2[] uv = new Vector2[vertexCount];
			Plugin.PixelpartGetMeshResourceVertexData(nativeEffect, resourceId, triangles, vertices, normals, uv);

			Mesh mesh = new Mesh();
			mesh.vertices = vertices;
			mesh.normals = normals;
			mesh.uv = uv;
			mesh.triangles = triangles;

			Meshes[resourceId] = mesh;
		}
	}

	public void Clear() {
		Textures.Clear();
		Meshes.Clear();
	}
}
}