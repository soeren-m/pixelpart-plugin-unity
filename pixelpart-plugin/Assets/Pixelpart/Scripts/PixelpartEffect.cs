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

	public List<Shader> ParticleShaders = new List<Shader>();
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

	private Material[] materials = null;
	private Mesh[] meshes = null;
	private VertexData[] vertexData = null;
	private uint[] sortedParticleTypeIndices = null;

	private Dictionary<string, Texture2D> textures = new Dictionary<string, Texture2D>();

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

		if(Is3D) {
			DrawEffect3D(Camera.main);
		}
		else {
			DrawEffect2D(Camera.main);
		}
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
				int numParticleTypes = (int)Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);

				if(EffectAsset.ParticleTypeAssets == null ||
				EffectAsset.ParticleTypeAssets.Length < numParticleTypes ||
				EffectAsset.ParticleTypeAssets.Any(x => x == null || x.TextureIds == null)) {
					Debug.LogError("Error loading effect asset", this);

					nativeEffect = IntPtr.Zero;
					return;
				}

				materials = new Material[numParticleTypes];
				meshes = new Mesh[numParticleTypes];
				vertexData = new VertexData[numParticleTypes];
				sortedParticleTypeIndices = new uint[numParticleTypes];
				textures.Clear();

				uint numImageResources = Plugin.PixelpartGetImageResourceCount(nativeEffect);

				for(uint i = 0; i < numImageResources; i++) {
					byte[] imageIdBuffer = new byte[2048];
					int imageIdLength = Plugin.PixelpartGetImageResourceId(nativeEffect, i, imageIdBuffer, imageIdBuffer.Length);
					string imageId = Encoding.UTF8.GetString(imageIdBuffer, 0, imageIdLength);

					int textureWidth = Plugin.PixelpartGetImageResourceWidth(nativeEffect, imageId);
					int textureHeight = Plugin.PixelpartGetImageResourceHeight(nativeEffect, imageId);
					uint textureDataSize = Plugin.PixelpartGetImageResourceDataSize(nativeEffect, imageId);
					byte[] textureData = new byte[textureDataSize];
					Plugin.PixelpartGetImageResourceData(nativeEffect, imageId, textureData);

					Texture2D texture = new Texture2D(textureWidth, textureHeight, TextureFormat.RGBA32, false);
					texture.filterMode = FilterMode.Bilinear;
					texture.wrapMode = TextureWrapMode.Repeat;
					texture.LoadRawTextureData(textureData);
					texture.Apply();

					textures[imageId] = texture;
				}

				for(int particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
					PixelpartParticleTypeAsset particleAsset = EffectAsset.ParticleTypeAssets[particleTypeIndex];

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
					};

					meshes[particleTypeIndex] = new Mesh();
					meshes[particleTypeIndex].MarkDynamic();

					vertexData[particleTypeIndex] = new VertexData(2, 4);
				}
			}
			else {
				Debug.LogError("Error loading effect asset", this);
			}
		}
	}

	private void DrawEffect3D(Camera camera) {
		uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);

		for(uint particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
			DrawParticles(camera,
				particleTypeIndex,
				Plugin.PixelpartFindParticleTypeByIndex(nativeEffect, particleTypeIndex));
		}
	}

	private void DrawEffect2D(Camera camera) {
		uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(nativeEffect);

		if(numParticleTypes > 0) {
			Plugin.PixelpartGetParticleTypesSortedByLayer(nativeEffect, sortedParticleTypeIndices);
		}

		for(uint i = 0; i < numParticleTypes; i++) {
			uint particleTypeIndex = sortedParticleTypeIndices[i];

			DrawParticles(camera,
				particleTypeIndex,
				Plugin.PixelpartFindParticleTypeByIndex(nativeEffect, particleTypeIndex));
		}
	}

	private void DrawParticles(Camera camera, uint particleTypeIndex, uint particleTypeId) {
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);
		bool visible = Plugin.PixelpartParticleTypeIsVisible(nativeEffect, particleTypeId);
		if(!visible) {
			return;
		}

		Material particleMaterial = materials[particleTypeIndex];
		Mesh particleMesh = meshes[particleTypeIndex];
		VertexData particleVertexData = vertexData[particleTypeIndex];

		int numTriangles = 0;
		int numVertices = 0;
		bool buildResult = Plugin.PixelpartPrepareParticleMesh(nativeEffect, particleTypeIndex, out numTriangles, out numVertices);
		if(!buildResult || numTriangles == 0 || numVertices == 0) {
			return;
		}

		particleVertexData.Resize(numTriangles, numVertices);

		Plugin.PixelpartBuildParticleMesh(nativeEffect,
			particleTypeIndex,
			(camera != null) ? camera.transform.position : new Vector3(0.0f, 0.0f, 0.0f),
			(camera != null) ? camera.transform.right : new Vector3(1.0f, 0.0f, 0.0f),
			(camera != null) ? camera.transform.up : new Vector3(0.0f, 1.0f, 0.0f),
			scaleX, scaleY,
			particleVertexData.Triangles,
			particleVertexData.Vertices,
			particleVertexData.Colors,
			particleVertexData.UV,
			particleVertexData.UV2,
			particleVertexData.UV3,
			particleVertexData.UV4);

		UpdateMesh(particleMesh, particleVertexData);

		uint particleEmitterId = Plugin.PixelpartParticleTypeGetParentId(nativeEffect, particleTypeId);
		float localTime = Plugin.PixelpartParticleEmitterGetLocalTime(nativeEffect, particleEmitterId);
		int blendMode = Plugin.PixelpartParticleTypeGetBlendMode(nativeEffect, particleTypeId);

		UpdateMaterial(particleMaterial, localTime, blendMode);

		Graphics.DrawMesh(particleMesh,
			transform.localToWorldMatrix,
			particleMaterial,
			gameObject.layer,
			null,
			0,
			null,
			UnityEngine.Rendering.ShadowCastingMode.Off, false,
			null, false);
	}

	private void UpdateMesh(Mesh mesh, VertexData vertexData) {
		mesh.Clear();

		mesh.vertices = vertexData.Vertices;
		mesh.colors = vertexData.Colors;
		mesh.uv = vertexData.UV;
		mesh.SetUVs(1, vertexData.UV2.ToList());
		mesh.SetUVs(2, vertexData.UV3.ToList());
		mesh.SetUVs(3, vertexData.UV4.ToList());

		mesh.triangles = vertexData.Triangles;
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
	public List<string> ParticleTypeNames = new List<string>();

	private PixelpartEffectAsset editorCachedEffectAsset = null;

	public void OnValidate() {
		if(EffectAsset == editorCachedEffectAsset) {
			return;
		}

		editorCachedEffectAsset = EffectAsset;
		ParticleShaders.Clear();
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
		}
	}

	public void OnDrawGizmos() {
		Gizmos.DrawIcon(transform.position, "PixelpartEffectIcon.png", true);
	}
#endif
}
}