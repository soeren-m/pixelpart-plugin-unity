using System;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
public class PixelpartParticleSpriteRenderer : PixelpartParticleRendererBase, PixelpartParticleRenderer {
	private readonly Mesh mesh;

	private int[] triangles;
	private Vector3[] vertices;
	private Color[] colors;
	private Vector3[] normals;
	private Vector2[] uv0;
	private Vector4[] uv1;
	private Vector4[] uv2;

	public PixelpartParticleSpriteRenderer(IntPtr effectPtr, uint ptypeIndex, Material baseMaterial, PixelpartMaterialInfo materialInfo, PixelpartGraphicsResourceProvider resourceProvider) :
		base(effectPtr, ptypeIndex, baseMaterial, materialInfo, resourceProvider) {
		triangles = new int[12];
		vertices = new Vector3[2];
		colors = new Color[2];
		normals = new Vector3[2];
		uv0 = new Vector2[2];
		uv1 = new Vector4[2];
		uv2 = new Vector4[2];

		mesh = new Mesh();
		mesh.MarkDynamic();
	}

	public void Draw(Camera camera, Transform transform, Vector3 scale, int layer) {
		bool visible = Plugin.PixelpartParticleTypeIsVisible(internalEffect, particleTypeId);
		if(!visible || camera == null) {
			return;
		}

		int numTriangles = 0;
		int numVertices = 0;
		Plugin.PixelpartPrepareParticleSpriteVertexData(internalEffect, particleTypeIndex, out numTriangles, out numVertices);
		if(numTriangles == 0 || numVertices == 0) {
			return;
		}

		Array.Resize(ref triangles, numTriangles * 3);
		Array.Resize(ref vertices, numVertices);
		Array.Resize(ref colors, numVertices);
		Array.Resize(ref normals, numVertices);
		Array.Resize(ref uv0, numVertices);
		Array.Resize(ref uv1, numVertices);
		Array.Resize(ref uv2, numVertices);

		Plugin.PixelpartGetParticleSpriteVertexData(internalEffect,
			particleTypeIndex,
			camera.transform.position,
			camera.transform.right,
			camera.transform.up,
			scale,
			triangles, vertices, colors, normals,
			uv0, uv1, uv2);

		ApplyMaterialParameters();

		mesh.Clear();
		mesh.vertices = vertices;
		mesh.colors = colors;
		mesh.normals = normals;
		mesh.uv = uv0;
		mesh.SetUVs(1, uv1.ToList());
		mesh.SetUVs(2, uv2.ToList());
		mesh.triangles = triangles;

		Graphics.DrawMesh(mesh, transform.localToWorldMatrix,
			material, 0, null, 0, null, ShadowCastingMode.Off, false, null, false);
	}
}
}