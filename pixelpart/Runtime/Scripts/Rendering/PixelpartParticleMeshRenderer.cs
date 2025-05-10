using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
internal class PixelpartParticleMeshRenderer : IPixelpartParticleRenderer {
	private const int maxParticlesPerDrawCall = 1023;

	private readonly IntPtr internalEffect;

	private readonly uint particleEmitterId;

	private readonly uint particleTypeId;

	private readonly Mesh mesh;

	private readonly PixelpartParticleMaterial particleMaterial;

	private readonly MaterialPropertyBlock materialPropertyBlock;

	private Matrix4x4[] transforms = new Matrix4x4[1];
	private Vector4[] colors = new Vector4[1];
	private Vector4[] velocities = new Vector4[1];
	private float[] lives = new float[1];
	private float[] ids = new float[1];

	public PixelpartParticleMeshRenderer(IntPtr effectRuntimePtr, uint emitterId, uint typeId, Material baseMaterial, PixelpartMaterialInfo materialInfo, PixelpartGraphicsResourceProvider resourceProvider) {
		internalEffect = effectRuntimePtr;
		particleEmitterId = emitterId;
		particleTypeId = typeId;

		var meshResourceIdBuffer = new byte[256];
		var meshResourceIdLength = Plugin.PixelpartParticleTypeGetMeshRendererMeshResourceId(internalEffect, typeId, meshResourceIdBuffer, meshResourceIdBuffer.Length);
		var meshResourceId = System.Text.Encoding.UTF8.GetString(meshResourceIdBuffer, 0, meshResourceIdLength);

		if(!resourceProvider.Meshes.TryGetValue(meshResourceId, out mesh)) {
			Debug.LogError("[Pixelpart] Failed to find mesh \"" + meshResourceId + "\"");
		}

		particleMaterial = new PixelpartParticleMaterial(effectRuntimePtr, emitterId, typeId, baseMaterial, materialInfo, resourceProvider);
		materialPropertyBlock = new MaterialPropertyBlock();
	}

	public void Render(Camera camera, Transform transform, Vector3 scale, int layer) {
		var visible = Plugin.PixelpartParticleTypeIsVisible(internalEffect, particleTypeId);
		if(!visible || camera == null || mesh == null) {
			return;
		}

		var particleCount = (int)Plugin.PixelpartGetEffectParticleCount(internalEffect, particleEmitterId, particleTypeId);
		if(particleCount < 1) {
			return;
		}

		Array.Resize(ref transforms, particleCount);
		Array.Resize(ref colors, particleCount);
		Array.Resize(ref velocities, particleCount);
		Array.Resize(ref lives, particleCount);
		Array.Resize(ref ids, particleCount);

		Plugin.PixelpartGetParticleMeshInstanceData(internalEffect,
			particleEmitterId, particleTypeId,
			camera.transform.position,
			camera.transform.right,
			camera.transform.up,
			scale,
			transforms, colors, velocities, lives, ids);

		var globalTransforms = transforms
			.Select(mat => transform.localToWorldMatrix * mat)
			.ToArray();

		particleMaterial.ApplyParameters();

		var drawCallCount = (particleCount - 1) / maxParticlesPerDrawCall + 1;

		for(var drawCallIndex = 0; drawCallIndex < drawCallCount; drawCallIndex++) {
			var startIndex = drawCallIndex * maxParticlesPerDrawCall;

			var currentTransforms = globalTransforms
				.Skip(startIndex)
				.Take(maxParticlesPerDrawCall)
				.ToArray();

			var currentColors = colors.Skip(startIndex).Take(maxParticlesPerDrawCall);
			if(currentColors.Count() < maxParticlesPerDrawCall) {
				currentColors = currentColors.Concat(
					Enumerable.Repeat(new Vector4(0.0f, 0.0f, 0.0f, 0.0f), maxParticlesPerDrawCall - currentColors.Count()));
			}

			var currentVelocities = velocities.Skip(startIndex).Take(maxParticlesPerDrawCall);
			if(currentVelocities.Count() < maxParticlesPerDrawCall) {
				currentVelocities = currentVelocities.Concat(
					Enumerable.Repeat(new Vector4(0.0f, 0.0f, 0.0f, 0.0f), maxParticlesPerDrawCall - currentVelocities.Count()));
			}

			var currentLives = lives.Skip(startIndex).Take(maxParticlesPerDrawCall);
			if(currentLives.Count() < maxParticlesPerDrawCall) {
				currentLives = currentLives.Concat(
					Enumerable.Repeat(0.0f, maxParticlesPerDrawCall - currentLives.Count()));
			}

			var currentObjectIds = ids.Skip(startIndex).Take(maxParticlesPerDrawCall);
			if(currentObjectIds.Count() < maxParticlesPerDrawCall) {
				currentObjectIds = currentObjectIds.Concat(
					Enumerable.Repeat(0.0f, maxParticlesPerDrawCall - currentObjectIds.Count()));
			}

			materialPropertyBlock.SetVectorArray("_Color", currentColors.ToArray());
			materialPropertyBlock.SetVectorArray("_Velocity", currentVelocities.ToArray());
			materialPropertyBlock.SetFloatArray("_Life", currentLives.ToArray());
			materialPropertyBlock.SetFloatArray("_ObjectId", currentObjectIds.ToArray());

			Graphics.DrawMeshInstanced(mesh, 0,
				particleMaterial.Material, currentTransforms, currentTransforms.Length, materialPropertyBlock, ShadowCastingMode.Off, false, 0, null);
		}
	}
}
}