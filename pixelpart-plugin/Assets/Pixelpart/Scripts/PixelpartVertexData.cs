using System;
using UnityEngine;

namespace pixelpart {
public class PixelpartVertexData {
	public int[] triangles;
	public Vector3[] positions;
	public Vector2[] textureCoords;
	public Color[] colors;
	public Vector2[] velocities;
	public Vector2[] forces;
	public Vector2[] objectInfo;

	public PixelpartVertexData(int numTriangles, int numVertices) {
		triangles = new int[numTriangles * 3];
		positions = new Vector3[numVertices];
		textureCoords = new Vector2[numVertices];
		colors = new Color[numVertices];
		velocities = new Vector2[numVertices];
		forces = new Vector2[numVertices];
		objectInfo = new Vector2[numVertices];
	}

	public void Resize(int numTriangles, int numVertices) {
		Array.Resize(ref triangles, numTriangles * 3);
		Array.Resize(ref positions, numVertices);
		Array.Resize(ref textureCoords, numVertices);
		Array.Resize(ref colors, numVertices);
		Array.Resize(ref velocities, numVertices);
		Array.Resize(ref forces, numVertices);
		Array.Resize(ref objectInfo, numVertices);
	}
}
}