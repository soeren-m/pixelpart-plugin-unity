using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartParticleMesh {
	private Material material;

	private Mesh mesh;

	private int[] triangles;
	private Vector3[] vertices;
	private Color[] colors;
	private Vector2[] uv;
	private Vector4[] uv2;
	private Vector4[] uv3;
	private Vector4[] uv4;

	private readonly IntPtr nativeEffect;
	private readonly uint particleTypeIndex;
	private readonly uint particleTypeId;

	public PixelpartParticleMesh(IntPtr effectPtr, uint ptypeIndex, uint ptypeId) {
		nativeEffect = effectPtr;
		particleTypeIndex = ptypeIndex;
		particleTypeId = ptypeId;

		// TODO: create material from builtin/custom shader

		mesh = new Mesh();
		mesh.MarkDynamic();

		triangles = new int[12];
		vertices = new Vector3[2];
		colors = new Color[2];
		uv = new Vector2[2];
		uv2 = new Vector4[2];
		uv3 = new Vector4[2];
		uv4 = new Vector4[2];
	}

	public void Draw(Camera camera, Vector3 scale) {
		bool visible = Plugin.PixelpartParticleTypeIsVisible(nativeEffect, particleTypeId);
		if(!visible || camera == null) {
			return;
		}

		ApplyMaterialParamters();

		ParticleRendererType renderer = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(nativeEffect, particleTypeId);

		switch(renderer) {
			case ParticleRendererType.Sprite: {
				DrawSprites(camera, scale);

				break;
			}

			case ParticleRendererType.Trail: {
				break;
			}

			case ParticleRendererType.Mesh: {
				break;
			}
		}
	}

	private void DrawSprites(Camera camera, Vector3 scale) {
		int numTriangles = 0;
		int numVertices = 0;
		Plugin.PixelpartPrepareParticleSpriteVertexData(nativeEffect, particleTypeIndex, out numTriangles, out numVertices);
		if(numTriangles == 0 || numVertices == 0) {
			return;
		}

		Array.Resize(ref triangles, numTriangles * 3);
		Array.Resize(ref vertices, numVertices);
		Array.Resize(ref colors, numVertices);
		Array.Resize(ref uv, numVertices);
		Array.Resize(ref uv2, numVertices);
		Array.Resize(ref uv3, numVertices);
		Array.Resize(ref uv4, numVertices);

		Plugin.PixelpartGetParticleSpriteVertexData(nativeEffect,
			particleTypeIndex,
			camera.transform.position,
			camera.transform.right,
			camera.transform.up,
			scale,
			triangles, vertices, colors,
			uv, uv2, uv3, uv4);

		mesh.Clear();

		mesh.vertices = vertices;
		mesh.colors = colors;
		mesh.uv = uv;
		mesh.SetUVs(1, uv2.ToList());
		mesh.SetUVs(2, uv3.ToList());
		mesh.SetUVs(3, uv4.ToList());
		mesh.triangles = triangles;

		Graphics.DrawMesh(particleMesh,
			transform.localToWorldMatrix,
			material,
			gameObject.layer,
			null, 0, null, UnityEngine.Rendering.ShadowCastingMode.Off, false, null, false);
	}

	private void DrawTrails(Camera camera, Vector3 scale) {
		int numTriangles = 0;
		int numVertices = 0;
		Plugin.PixelpartPrepareParticleTrailVertexData(nativeEffect, particleTypeIndex, out numTriangles, out numVertices);
		if(numTriangles == 0 || numVertices == 0) {
			return;
		}

		Array.Resize(ref triangles, numTriangles * 3);
		Array.Resize(ref vertices, numVertices);
		Array.Resize(ref colors, numVertices);
		Array.Resize(ref uv, numVertices);
		Array.Resize(ref uv2, numVertices);
		Array.Resize(ref uv3, numVertices);
		Array.Resize(ref uv4, numVertices);

		Plugin.PixelpartGetParticleTrailVertexData(nativeEffect,
			particleTypeIndex,
			camera.transform.position,
			camera.transform.right,
			camera.transform.up,
			scale,
			triangles, vertices, colors,
			uv, uv2, uv3, uv4);

		mesh.Clear();

		mesh.vertices = vertices;
		mesh.colors = colors;
		mesh.uv = uv;
		mesh.SetUVs(1, uv2.ToList());
		mesh.SetUVs(2, uv3.ToList());
		mesh.SetUVs(3, uv4.ToList());
		mesh.triangles = triangles;

		Graphics.DrawMesh(particleMesh,
			transform.localToWorldMatrix,
			material,
			gameObject.layer,
			null, 0, null, UnityEngine.Rendering.ShadowCastingMode.Off, false, null, false);
	}

	private void ApplyMaterialParamters() {
		float effectTime = Plugin.PixelpartGetEffectTime(nativeEffect);
		float objectTime = Plugin.PixelpartParticleEmitterGetLocalTime(nativeEffect, particleEmitterId);

		material.SetFloat("_EffectTime", effectTime);
		material.SetFloat("_ObjectTime", objectTime);

		// TODO: go through particle type material parameters and update material
	}
}
}