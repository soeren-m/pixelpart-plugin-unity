using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;

namespace Pixelpart {
public class PixelpartGraphicsResourceProvider {
	public Dictionary<string, Texture2D> Textures { get; }

	public Dictionary<string, Mesh> Meshes { get; }

	public PixelpartGraphicsResourceProvider() {
		Textures = new Dictionary<string, Texture2D>();
		Meshes = new Dictionary<string, Mesh>();
	}

	public void Load(IntPtr internalEffect) {
		Clear();

		byte[] resourceIdBuffer = new byte[2048];

		uint numImageResources = Plugin.PixelpartGetImageResourceCount(internalEffect);

		for(uint i = 0; i < numImageResources; i++) {
			int resourceIdLength = Plugin.PixelpartGetImageResourceId(internalEffect, i, resourceIdBuffer, resourceIdBuffer.Length);
			string resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

			int imageWidth = Plugin.PixelpartGetImageResourceWidth(internalEffect, resourceId);
			int imageHeight = Plugin.PixelpartGetImageResourceHeight(internalEffect, resourceId);
			uint imageDataSize = Plugin.PixelpartGetImageResourceDataSize(internalEffect, resourceId);
			byte[] imageData = new byte[imageDataSize];
			Plugin.PixelpartGetImageResourceData(internalEffect, resourceId, imageData);

			Texture2D texture = new Texture2D(imageWidth, imageHeight, TextureFormat.RGBA32, false);
			texture.filterMode = FilterMode.Bilinear;
			texture.wrapMode = TextureWrapMode.Repeat;
			texture.LoadRawTextureData(imageData);
			texture.Apply();

			Textures[resourceId] = texture;
		}

		uint numMeshResources = Plugin.PixelpartGetMeshResourceCount(internalEffect);

		for(uint i = 0; i < numMeshResources; i++) {
			int resourceIdLength = Plugin.PixelpartGetMeshResourceId(internalEffect, i, resourceIdBuffer, resourceIdBuffer.Length);
			string resourceId = Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

			int indexCount = Plugin.PixelpartGetMeshResourceIndexCount(internalEffect, resourceId);
			int vertexCount = Plugin.PixelpartGetMeshResourceVertexCount(internalEffect, resourceId);

			int[] triangles = new int[indexCount];
			Vector3[] vertices = new Vector3[vertexCount];
			Vector3[] normals = new Vector3[vertexCount];
			Vector2[] uv = new Vector2[vertexCount];
			Plugin.PixelpartGetMeshResourceVertexData(internalEffect, resourceId, triangles, vertices, normals, uv);

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