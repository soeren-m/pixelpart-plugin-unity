using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
[AddComponentMenu("Pixelpart/Effect")]
public class PixelpartEffect : MonoBehaviour {
	public const uint NullId = 0xFFFFFFFF;

	public PixelpartEffectAsset EffectAsset = null;

	public bool Playing = true;
	public bool Loop = false;
	[Range(0.0f, 100.0f)]
	public float LoopTime = 1.0f;
	[Range(0.0f, 10.0f)]
	public float Speed = 1.0f;
	[Range(1.0f, 100.0f)]
	public float FrameRate = 60.0f;

	//public List<Shader> ParticleShaders = new List<Shader>();
	public bool FlipH = false;
	public bool FlipV = false;

	public bool Is3D {
		get {
			return nativeEffect != IntPtr.Zero
				? Plugin.PixelpartIsEffect3D(nativeEffect)
				: false;
		}
	}

	public float CurrentTime {
		get {
			return nativeEffect != IntPtr.Zero
				? Plugin.PixelpartGetEffectTime(nativeEffect)
				: 0.0f;
		}
	}

	public float AssetScale {
		get {
			return EffectAsset != null
				? EffectAsset.Scale
				: 0.0f;
		}
	}

	private IntPtr nativeEffect = IntPtr.Zero;

	private PixelpartEffectAsset cachedEffectAsset = null;

	private PixelpartGraphicsResourceStore graphicsResources = new PixelpartGraphicsResourceStore();

	private PixelpartParticleMesh[] particleMeshes;

	private uint[] sortedParticleTypeIndices = null;

	public PixelpartEffect() {

	}

	public void Awake() {
		InitEffect();
	}

	public void Update() {
		if(EffectAsset != cachedEffectAsset) {
			InitEffect();
		}

		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartPlayEffect(nativeEffect, Playing && gameObject.activeSelf);
		Plugin.PixelpartSetEffectLoop(nativeEffect, Loop);
		Plugin.PixelpartSetEffectLoopTime(nativeEffect, LoopTime);
		Plugin.PixelpartSetEffectSpeed(nativeEffect, Speed);
		Plugin.PixelpartSetEffectTimeStep(nativeEffect, 1.0f / FrameRate);
		Plugin.PixelpartUpdateEffect(nativeEffect, Time.deltaTime);
	}

	public void LateUpdate() {
		if(nativeEffect == IntPtr.Zero) {
			return;
		}

		DrawEffect(Camera.main);
	}

	public void OnDestroy() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartDeleteEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}
	}

	public void RestartEffect() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartRestartEffect(nativeEffect);
		}
	}
	public void ResetEffect() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartResetEffect(nativeEffect);
		}
	}

	public void SpawnParticles(string particleTypeName, int count) {
		if(nativeEffect != IntPtr.Zero) {
			uint particleTypeId = Plugin.PixelpartFindParticleType(nativeEffect, particleTypeName);
			if(particleTypeId != NullId) {
				Plugin.PixelpartSpawnParticles(nativeEffect, particleTypeId, count);
			}
		}
	}

	public void SetInput(string name, bool value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputBool(nativeEffect, name, value);
		}
	}
	public void SetInput(string name, int value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputInt(nativeEffect, name, value);
		}
	}
	public void SetInput(string name, float value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputFloat(nativeEffect, name, value);
		}
	}
	public void SetInput(string name, Vector2 value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputFloat2(nativeEffect, name, value);
		}
	}
	public void SetInput(string name, Vector3 value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputFloat3(nativeEffect, name, value);
		}
	}
	public void SetInput(string name, Vector4 value) {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartSetEffectInputFloat4(nativeEffect, name, value);
		}
	}
	public bool GetInputBool(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputBool(nativeEffect, name);
		}

		return false;
	}
	public int GetInputInt(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputInt(nativeEffect, name);
		}

		return 0;
	}
	public float GetInputFloat(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputFloat(nativeEffect, name);
		}

		return 0.0f;
	}
	public Vector2 GetInputFloat2(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputFloat2(nativeEffect, name);
		}

		return new Vector2(0.0f, 0.0f);
	}
	public Vector3 GetInputFloat3(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputFloat3(nativeEffect, name);
		}

		return new Vector3(0.0f, 0.0f, 0.0f);
	}
	public Vector4 GetInputFloat4(string name) {
		if(nativeEffect != IntPtr.Zero) {
			return Plugin.PixelpartGetEffectInputFloat4(nativeEffect, name);
		}

		return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	public PixelpartParticleEmitter FindParticleEmitter(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindParticleEmitter(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartParticleEmitter(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartParticleType FindParticleType(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindParticleType(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartParticleType(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField FindForceField(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindForceField(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartForceField(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartCollider FindCollider(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindCollider(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartCollider(nativeEffect, id);
			}
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitter(uint id) {
		if(nativeEffect != IntPtr.Zero && Plugin.PixelpartHasParticleEmitter(nativeEffect, id)) {
			return new PixelpartParticleEmitter(nativeEffect, id);
		}

		return null;
	}
	public PixelpartParticleType GetParticleType(uint id) {
		if(nativeEffect != IntPtr.Zero && Plugin.PixelpartHasParticleType(nativeEffect, id)) {
			return new PixelpartParticleType(nativeEffect, id);
		}

		return null;
	}
	public PixelpartForceField GetForceField(uint id) {
		if(nativeEffect != IntPtr.Zero && Plugin.PixelpartHasForceField(nativeEffect, id)) {
			return new PixelpartForceField(nativeEffect, id);
		}

		return null;
	}
	public PixelpartCollider GetCollider(uint id) {
		if(nativeEffect != IntPtr.Zero && Plugin.PixelpartHasCollider(nativeEffect, id)) {
			return new PixelpartCollider(nativeEffect, id);
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitterAtIndex(int index) {
		if(nativeEffect != IntPtr.Zero && index >= 0) {
			uint id = Plugin.PixelpartFindParticleEmitterByIndex(nativeEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartParticleEmitter(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartParticleType GetParticleTypeAtIndex(int index) {
		if(nativeEffect != IntPtr.Zero && index >= 0) {
			uint id = Plugin.PixelpartFindParticleTypeByIndex(nativeEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartParticleType(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField GetForceFieldAtIndex(int index) {
		if(nativeEffect != IntPtr.Zero && index >= 0) {
			uint id = Plugin.PixelpartFindForceFieldByIndex(nativeEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartForceField(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartCollider GetColliderAtIndex(int index) {
		if(nativeEffect != IntPtr.Zero && index >= 0) {
			uint id = Plugin.PixelpartFindColliderByIndex(nativeEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartCollider(nativeEffect, id);
			}
		}

		return null;
	}

	private void InitEffect() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartDeleteEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}

		cachedEffectAsset = EffectAsset;

		if(EffectAsset != null) {
			nativeEffect = EffectAsset.LoadEffect();

			if(nativeEffect != IntPtr.Zero) {
				graphicsResources.Load(nativeEffect);

				int numParticleTypes = (int)Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);

				/*if(EffectAsset.ParticleTypeAssets == null ||
				EffectAsset.ParticleTypeAssets.Length < numParticleTypes ||
				EffectAsset.ParticleTypeAssets.Any(x => x == null || x.TextureIds == null)) {
					Debug.LogError("Error loading effect asset", this);

					nativeEffect = IntPtr.Zero;
					return;
				}*/

				particleMeshes = new PixelpartParticleMesh[numParticleTypes];
				sortedParticleTypeIndices = new uint[numParticleTypes];

				for(int particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
					particleMeshes[particleTypeIndex] = new PixelpartParticleMesh(nativeEffect, (uint)particleTypeIndex);

					/*PixelpartParticleTypeAsset particleAsset = EffectAsset.ParticleTypeAssets[particleTypeIndex];

					materials[particleTypeIndex] = new Material(particleTypeIndex < ParticleShaders.Count
						? ParticleShaders[particleTypeIndex]
						: null);

					for(int samplerIndex = 0; samplerIndex < particleAsset.TextureIds.Length; samplerIndex++) {
						string textureId = particleAsset.TextureIds[samplerIndex];

						if(textures.ContainsKey(textureId)) {
							materials[particleTypeIndex].SetTexture("_Texture" + samplerIndex.ToString(), textures[textureId]);
						}
						else {
							Debug.LogWarning("Particle texture \"" + textureId + "\" not found", this);
						}
					};*/
				}
			}
			else {
				Debug.LogError("Error loading effect asset", this);

				graphicsResources.Clear();
			}
		}
	}

	private void DrawEffect(Camera camera) {
		uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);

		if(numParticleTypes > 0) {
			Plugin.PixelpartGetParticleTypesSortedForRendering(nativeEffect, sortedParticleTypeIndices);
		}

		for(uint i = 0; i < numParticleTypes; i++) {
			uint particleTypeIndex = sortedParticleTypeIndices[i];

			DrawParticles(camera,
				particleTypeIndex,
				Plugin.PixelpartFindParticleTypeByIndex(nativeEffect, particleTypeIndex));
		}
	}

	private void DrawParticles(Camera camera, uint particleTypeIndex, uint particleTypeId) {
		Vector3 scale = new Vector3(
			EffectAsset.Scale * (FlipH ? -1.0f : +1.0f),
			EffectAsset.Scale * (FlipV ? -1.0f : +1.0f),
			EffectAsset.Scale);

		particleMeshes[particleTypeIndex].Draw(camera, transform, scale, gameObject.layer);

		/*uint particleEmitterId = Plugin.PixelpartParticleTypeGetParentId(nativeEffect, particleTypeId);
		int blendMode = Plugin.PixelpartParticleTypeGetBlendMode(nativeEffect, particleTypeId);
		UpdateMaterial(particleMaterial, localTime, blendMode);*/
	}

	private void UpdateMaterial(Material material, float objectTime, int blendMode) {
		material.SetFloat("_EffectTime", CurrentTime);
		material.SetFloat("_ObjectTime", objectTime);

		switch(blendMode) {
			case 1:
				material.SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				material.SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				material.SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				break;

			case 2:
				material.SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.ReverseSubtract);
				material.SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				material.SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;

			default:
				material.SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				material.SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				material.SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;
		}
	}

#if UNITY_EDITOR
	//public List<string> ParticleTypeNames = new List<string>();

	private PixelpartEffectAsset editorCachedEffectAsset = null;

	public void OnValidate() {
		if(EffectAsset == editorCachedEffectAsset) {
			return;
		}

		editorCachedEffectAsset = EffectAsset;
		/*ParticleShaders.Clear();
		ParticleTypeNames.Clear();

		if(EffectAsset != null) {
			if(EffectAsset.ParticleTypeAssets == null ||
				EffectAsset.ParticleTypeAssets.Any(x => x == null || x.TextureIds == null)) {
				Debug.LogError("Effect asset not imported properly, please reimport", this);
				return;
			}

			for(int particleTypeIndex = 0; particleTypeIndex < EffectAsset.ParticleTypeAssets.Length; particleTypeIndex++) {
				PixelpartParticleTypeAsset particleAsset = EffectAsset.ParticleTypeAssets[particleTypeIndex];

				Shader shader = Shader.Find(particleAsset.DefaultShaderName);
				if(shader == null) {
					Debug.LogWarning(
						"Particle shader \"" + particleAsset.DefaultShaderName + "\" not found",
						this);
				}

				ParticleTypeNames.Add(particleAsset.Name);
				ParticleShaders.Add(shader);
			}
		}*/
	}

	public void OnDrawGizmos() {
		Gizmos.DrawIcon(transform.position, "PixelpartEffectIcon.png", true);
	}
#endif
}
}