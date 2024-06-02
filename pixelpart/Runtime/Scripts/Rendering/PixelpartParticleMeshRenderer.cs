using System;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
public class PixelpartParticleMeshRenderer : PixelpartParticleRendererBase, PixelpartParticleRenderer {
	private readonly Mesh mesh;

	private readonly MaterialPropertyBlock materialPropertyBlock;

	private Matrix4x4[] transforms;
	private Vector4[] colors;
	private Vector4[] velocities;
	private float[] lives;
	private float[] ids;

	public PixelpartParticleMeshRenderer(IntPtr effectPtr, uint ptypeIndex, Material baseMaterial, PixelpartMaterialInfo materialInfo, PixelpartGraphicsResourceProvider resourceProvider) :
		base(effectPtr, ptypeIndex, baseMaterial, materialInfo, resourceProvider) {
		transforms = new Matrix4x4[1];
		colors = new Vector4[1];
		velocities = new Vector4[1];
		lives = new float[1];
		ids = new float[1];

		byte[] meshResourceIdBuffer = new byte[256];
		int meshResourceIdLength = Plugin.PixelpartParticleTypeGetMeshRendererMeshResourceId(internalEffect, particleTypeId, meshResourceIdBuffer, meshResourceIdBuffer.Length);
		string meshResourceId = System.Text.Encoding.UTF8.GetString(meshResourceIdBuffer, 0, meshResourceIdLength);

		if(!resourceProvider.Meshes.TryGetValue(meshResourceId, out mesh)) {
			Debug.LogError("[Pixelpart] Cannot find mesh \"" + meshResourceId + "\"");
		}

		materialPropertyBlock = new MaterialPropertyBlock();
	}

	public void Draw(Camera camera, Transform transform, Vector3 scale, int layer) {
		bool visible = Plugin.PixelpartParticleTypeIsVisible(internalEffect, particleTypeId);
		if(!visible || camera == null || mesh == null) {
			return;
		}

		int numParticles = (int)Plugin.PixelpartGetEffectNumParticles(internalEffect, particleTypeIndex);
		if(numParticles < 1) {
			return;
		}

		Array.Resize(ref transforms, numParticles);
		Array.Resize(ref colors, numParticles);
		Array.Resize(ref velocities, numParticles);
		Array.Resize(ref lives, numParticles);
		Array.Resize(ref ids, numParticles);

		Plugin.PixelpartGetParticleMeshInstanceData(internalEffect,
			particleTypeIndex,
			camera.transform.position,
			camera.transform.right,
			camera.transform.up,
			scale,
			transforms, colors, velocities, lives, ids);

		Matrix4x4[] globalTransforms = transforms
			.Select(mat => transform.localToWorldMatrix * mat).ToArray();

		ApplyMaterialParameters();

		const int maxNumParticlesPerDrawCall = 1023;
		int numDrawCalls = (numParticles - 1) / maxNumParticlesPerDrawCall + 1;

		for(int drawCallIndex = 0; drawCallIndex < numDrawCalls; drawCallIndex++) {
			int startIndex = drawCallIndex * maxNumParticlesPerDrawCall;

			Matrix4x4[] currentTransforms = globalTransforms
				.Skip(startIndex)
				.Take(maxNumParticlesPerDrawCall)
				.ToArray();

			var currentColors = colors.Skip(startIndex).Take(maxNumParticlesPerDrawCall);
			if(currentColors.Count() < maxNumParticlesPerDrawCall) {
				currentColors = currentColors.Concat(
					Enumerable.Repeat(new Vector4(0.0f, 0.0f, 0.0f, 0.0f), maxNumParticlesPerDrawCall - currentColors.Count()));
			}

			var currentVelocities = velocities.Skip(startIndex).Take(maxNumParticlesPerDrawCall);
			if(currentVelocities.Count() < maxNumParticlesPerDrawCall) {
				currentVelocities = currentVelocities.Concat(
					Enumerable.Repeat(new Vector4(0.0f, 0.0f, 0.0f, 0.0f), maxNumParticlesPerDrawCall - currentVelocities.Count()));
			}

			var currentLives = lives.Skip(startIndex).Take(maxNumParticlesPerDrawCall);
			if(currentLives.Count() < maxNumParticlesPerDrawCall) {
				currentLives = currentLives.Concat(
					Enumerable.Repeat(0.0f, maxNumParticlesPerDrawCall - currentLives.Count()));
			}

			var currentObjectIds = ids.Skip(startIndex).Take(maxNumParticlesPerDrawCall);
			if(currentObjectIds.Count() < maxNumParticlesPerDrawCall) {
				currentObjectIds = currentObjectIds.Concat(
					Enumerable.Repeat(0.0f, maxNumParticlesPerDrawCall - currentObjectIds.Count()));
			}

			materialPropertyBlock.SetVectorArray("_Color", currentColors.ToArray());
			materialPropertyBlock.SetVectorArray("_Velocity", currentVelocities.ToArray());
			materialPropertyBlock.SetFloatArray("_Life", currentLives.ToArray());
			materialPropertyBlock.SetFloatArray("_ObjectId", currentObjectIds.ToArray());

			Graphics.DrawMeshInstanced(mesh, 0,
				material, currentTransforms, currentTransforms.Length, materialPropertyBlock, ShadowCastingMode.Off, false, 0, null);
		}
	}
}
}