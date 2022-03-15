using System;
using UnityEngine;

namespace pixelpart {
public class PixelpartMeshData {
	public int[] triangles;
	public Vector3[] positions;
	public Vector2[] uvs;
	public Color[] colors;

	public PixelpartMeshData(int numTriangles, int numVertices) {
		triangles = new int[numTriangles * 3];
		positions = new Vector3[numVertices];
		uvs = new Vector2[numVertices];
		colors = new Color[numVertices];
	}

	public void Resize(int numTriangles, int numVertices) {
		Array.Resize(ref triangles, numTriangles * 3);
		Array.Resize(ref positions, numVertices);
		Array.Resize(ref uvs, numVertices);
		Array.Resize(ref colors, numVertices);
	}
}
}