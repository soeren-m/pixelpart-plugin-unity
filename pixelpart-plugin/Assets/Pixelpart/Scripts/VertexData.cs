using System;
using UnityEngine;

namespace Pixelpart {
internal class VertexData {
	public int[] Triangles;
	public Vector3[] Vertices;
	public Color[] Colors;
	public Vector2[] UV;
	public Vector4[] UV2;
	public Vector4[] UV3;
	public Vector4[] UV4;

	public VertexData(int numTriangles, int numVertices) {
		Triangles = new int[numTriangles * 3];
		Vertices = new Vector3[numVertices];
		Colors = new Color[numVertices];
		UV = new Vector2[numVertices];
		UV2 = new Vector4[numVertices];
		UV3 = new Vector4[numVertices];
		UV4 = new Vector4[numVertices];
	}

	public void Resize(int numTriangles, int numVertices) {
		Array.Resize(ref Triangles, numTriangles * 3);
		Array.Resize(ref Vertices, numVertices);
		Array.Resize(ref Colors, numVertices);
		Array.Resize(ref UV, numVertices);
		Array.Resize(ref UV2, numVertices);
		Array.Resize(ref UV3, numVertices);
		Array.Resize(ref UV4, numVertices);
	}
}
}