using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace pixelpart {
[AddComponentMenu("Pixelpart/Pixelpart Effect")]
public class PixelpartEffect : MonoBehaviour {
	public const uint NullId = 0xFFFFFFFF;

	[Header("Asset")]
	public PixelpartEffectAsset EffectAsset = null;

	[Header("Playback")]
	public bool Playing = true;
	public bool Loop = false;
	[Range(0.0f, 100.0f)]
	public float LoopTime = 1.0f;
	[Range(0.0f, 100.0f)]
	public float Speed = 1.0f;
	[Range(1.0f, 100.0f)]
	public float FrameRate = 60.0f;

	[Header("Rendering")]
	public bool FlipH = false;
	public bool FlipV = false;
	public BillboardMode Billboard = BillboardMode.Disabled;
	public UnityEngine.Rendering.ShadowCastingMode CastShadows = UnityEngine.Rendering.ShadowCastingMode.Off;
	public bool ReceiveShadows = false;
	public bool UseLightProbes = false;
	public Transform ProbeAnchor = null;

	private PixelpartEffectAsset cachedEffectAsset = null;
	private IntPtr nativeEffect = IntPtr.Zero;

	private Mesh[] particleMeshes = null;
	private Mesh[] spriteMeshes = null;
	private Material[] particleMaterials = null;
	private Material[] spriteMaterials = null;

	private PixelpartVertexData[] particleVertexData = null;
	private float[] particlePositionBuffer = null;
	private float[] particleTextureCoordBuffer = null;
	private float[] particleColorBuffer = null;
	private float[] particleVelocityBuffer = null;
	private float[] particleForceBuffer = null;
	private float[] particleLifeBuffer = null;
	private int[] particleIdBuffer = null;

	private PixelpartVertexData[] spriteVertexData = null;
	private float[] spritePositionBuffer = null;
	private float[] spriteTextureCoordBuffer = null;
	private float[] spriteColorBuffer = null;
	private float[] spriteLifeBuffer = null;

	private uint[] sortedParticleEmitterIndices = null;
	private uint[] sortedSpriteIndices = null;

	private Dictionary<string, Texture2D> textures = new Dictionary<string, Texture2D>();

	public void Awake() {
		SetEffect(EffectAsset);
	}
	public void Update() {
		SetEffect(EffectAsset);

		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartPlayEffect(nativeEffect, Playing && gameObject.activeSelf);
			Plugin.PixelpartSetEffectLoop(nativeEffect, Loop);
			Plugin.PixelpartSetEffectLoopTime(nativeEffect, LoopTime);
			Plugin.PixelpartSetEffectSpeed(nativeEffect, Speed);
			Plugin.PixelpartSetEffectTimeStep(nativeEffect, 1.0f / FrameRate);
			Plugin.PixelpartUpdateEffect(nativeEffect, Time.deltaTime);

			DrawEffect();
		}
	}

	public void OnDestroy() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartDeleteEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}
	}

#if UNITY_EDITOR
	public void OnDrawGizmos() {
		Gizmos.DrawIcon(transform.position, "PixelpartEffectIcon.png", true);
	}
#endif

	public void Restart() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartRestartEffect(nativeEffect);
		}
	}
	public void Reset() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartResetEffect(nativeEffect);
		}
	}
	public float GetTime() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartGetEffectTime(nativeEffect);
		}

		return 0.0f;
	}

	public void SetEffect(PixelpartEffectAsset asset) {
		if(asset == cachedEffectAsset) {
			return;
		}

		if(nativeEffect != IntPtr.Zero) {
			textures.Clear();
			Plugin.PixelpartDeleteEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}

		EffectAsset = asset;

		if(EffectAsset) {
			nativeEffect = EffectAsset.LoadEffect();

			if(nativeEffect != IntPtr.Zero) {
				uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
				uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);
				uint initialVertexBufferSize = 4000;

				if(EffectAsset.ParticleAssets == null ||
				EffectAsset.SpriteAssets == null ||
				EffectAsset.ParticleAssets.Length < numParticleEmitters ||
				EffectAsset.SpriteAssets.Length < numSprites) {
					throw new InvalidOperationException("Effect asset not imported");
				}

				sortedParticleEmitterIndices = new uint[numParticleEmitters];
				particleMeshes = new Mesh[numParticleEmitters];
				particleMaterials = new Material[numParticleEmitters];
				particleVertexData = new PixelpartVertexData[numParticleEmitters];
				particlePositionBuffer = new float[initialVertexBufferSize * 2];
				particleTextureCoordBuffer = new float[initialVertexBufferSize * 2];
				particleColorBuffer = new float[initialVertexBufferSize * 4];
				particleVelocityBuffer = new float[initialVertexBufferSize * 2];
				particleForceBuffer = new float[initialVertexBufferSize * 2];
				particleLifeBuffer = new float[initialVertexBufferSize];
				particleIdBuffer = new int[initialVertexBufferSize];

				sortedSpriteIndices = new uint[numSprites];
				spriteMeshes = new Mesh[numSprites];
				spriteMaterials = new Material[numSprites];
				spriteVertexData = new PixelpartVertexData[numSprites];
				spritePositionBuffer = new float[4 * 2];
				spriteTextureCoordBuffer = new float[4 * 2];
				spriteColorBuffer = new float[4 * 4];
				spriteLifeBuffer = new float[4];

				uint numImageResources = Plugin.PixelpartGetEffectResourceImageCount(nativeEffect);

				for(uint i = 0; i < numImageResources; i++) {
					byte[] imageIdBuffer = new byte[2048];
					int imageIdLength = Plugin.PixelpartGetEffectResourceImageId(nativeEffect, i, imageIdBuffer, imageIdBuffer.Length);
					string imageId = System.Text.Encoding.UTF8.GetString(imageIdBuffer, 0, imageIdLength);

					uint textureWidth = Plugin.PixelpartGetEffectResourceImageWidth(nativeEffect, imageId);
					uint textureHeight = Plugin.PixelpartGetEffectResourceImageHeight(nativeEffect, imageId);
					uint textureDataSize = Plugin.PixelpartGetEffectResourceImageDataSize(nativeEffect, imageId);
					byte[] textureData = new byte[textureDataSize];
					Plugin.PixelpartGetEffectResourceImageData(nativeEffect, imageId, textureData);

					Texture2D texture = new Texture2D((int)textureWidth, (int)textureHeight, TextureFormat.RGBA32, false);
					texture.filterMode = FilterMode.Bilinear;
					texture.wrapMode = TextureWrapMode.Repeat;
					texture.LoadRawTextureData(textureData);
					texture.Apply();

					textures[imageId] = texture;
				}

				for(uint emitterIndex = 0; emitterIndex < numParticleEmitters; emitterIndex++) {
					Shader shader = Shader.Find(EffectAsset.ParticleAssets[emitterIndex].ShaderName);
					if(shader == null) {
						throw new InvalidOperationException("Shader for effect not found");
					}

					Material material = new Material(shader);
					for(int samplerIndex = 0; samplerIndex < EffectAsset.ParticleAssets[emitterIndex].TextureIds.Length; samplerIndex++) {
						string textureId = EffectAsset.ParticleAssets[emitterIndex].TextureIds[samplerIndex];
						if(!textures.ContainsKey(textureId)) {
							throw new InvalidOperationException("Texture \"" + textureId + "\" not found");
						}

						material.SetTexture("_Texture" + samplerIndex.ToString(), textures[textureId]);
					}

					particleMaterials[emitterIndex] = material;
					particleMeshes[emitterIndex] = new Mesh();
					particleVertexData[emitterIndex] = new PixelpartVertexData(2, 4);
				}

				for(uint spriteIndex = 0; spriteIndex < numSprites; spriteIndex++) {
					Shader shader = Shader.Find(EffectAsset.SpriteAssets[spriteIndex].ShaderName);
					if(shader == null) {
						throw new InvalidOperationException("Shader for effect not found");
					}

					Material material = new Material(shader);
					for(int samplerIndex = 0; samplerIndex < EffectAsset.SpriteAssets[spriteIndex].TextureIds.Length; samplerIndex++) {
						string textureId = EffectAsset.SpriteAssets[spriteIndex].TextureIds[samplerIndex];
						if(!textures.ContainsKey(textureId)) {
							throw new InvalidOperationException("Texture \"" + textureId + "\" not found");
						}

						material.SetTexture("_Texture" + samplerIndex.ToString(), textures[textureId]);
					}

					spriteMaterials[spriteIndex] = material;
					spriteMeshes[spriteIndex] = new Mesh();
					spriteVertexData[spriteIndex] = new PixelpartVertexData(2, 4);
				}
			}
		}

		cachedEffectAsset = EffectAsset;
	}

	public float GetAssetScale() {
		if(EffectAsset != null) {
			return EffectAsset.Scale;
		}

		return 0.0f;
	}

	public PixelpartParticleEmitter GetParticleEmitter(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindParticleEmitter(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartParticleEmitter(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartSprite GetSprite(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindSprite(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartSprite(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField GetForceField(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindForceField(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartForceField(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartCollider GetCollider(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindCollider(nativeEffect, name);
			if(id != NullId) {
				return new PixelpartCollider(nativeEffect, id);
			}
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitterById(uint id) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartParticleEmitter(nativeEffect, id);
		}

		return null;
	}
	public PixelpartSprite GetSpriteById(uint id) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartSprite(nativeEffect, id);
		}

		return null;
	}
	public PixelpartForceField GetForceFieldById(uint id) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartForceField(nativeEffect, id);
		}

		return null;
	}
	public PixelpartCollider GetColliderById(uint id) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartCollider(nativeEffect, id);
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitterByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindParticleEmitterByIndex(nativeEffect, index);
			if(id != NullId) {
				return new PixelpartParticleEmitter(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartSprite GetSpriteByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindSpriteByIndex(nativeEffect, index);
			if(id != NullId) {
				return new PixelpartSprite(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField GetForceFieldByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindForceFieldByIndex(nativeEffect, index);
			if(id != NullId) {
				return new PixelpartForceField(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartCollider GetColliderByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindColliderByIndex(nativeEffect, index);
			if(id != NullId) {
				return new PixelpartCollider(nativeEffect, id);
			}
		}

		return null;
	}

	private void DrawEffect() {
		uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
		uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);
		float effectTime = Plugin.PixelpartGetEffectTime(nativeEffect);

		if(numParticleEmitters > 0) {
			Plugin.PixelpartGetParticleEmittersSortedByLayer(nativeEffect, sortedParticleEmitterIndices);
		}
		if(numSprites > 0) {
			Plugin.PixelpartGetSpritesSortedByLayer(nativeEffect, sortedSpriteIndices);
		}

		uint maxLayer = Plugin.PixelpartGetEffectMaxLayer(nativeEffect);

		for(uint l = 0, e = 0, s = 0; l <= maxLayer; l++) {
			while(s < numSprites) {
				uint spriteIndex = sortedSpriteIndices[s];
				uint spriteId = Plugin.PixelpartFindSpriteByIndex(nativeEffect, spriteIndex);
				uint spriteLayer = Plugin.PixelpartSpriteGetLayer(nativeEffect, spriteId);
				if(spriteLayer != l) {
					break;
				}

				DrawSprite(spriteIndex, spriteId, spriteLayer, effectTime);

				s++;
			}

			while(e < numParticleEmitters) {
				uint emitterIndex = sortedParticleEmitterIndices[e];
				uint emitterId = Plugin.PixelpartFindParticleEmitterByIndex(nativeEffect, emitterIndex);
				uint emitterLayer = Plugin.PixelpartParticleEmitterGetLayer(nativeEffect, emitterId);
				if(emitterLayer != l) {
					break;
				}

				DrawParticles(emitterIndex, emitterId, emitterLayer, effectTime);

				e++;
			}
		}
	}

	private void DrawParticles(uint emitterIndex, uint emitterId, uint layer, float effectTime) {
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);
		bool visible = Plugin.PixelpartParticleEmitterIsVisible(nativeEffect, emitterId);
		if(!visible) {
			return;
		}

		Plugin.PixelpartPrepareParticleMeshBuild(nativeEffect, emitterIndex);

		uint numTriangles = Plugin.PixelpartGetParticleMeshNumTriangles(nativeEffect);
		uint numVertices = Plugin.PixelpartGetParticleMeshNumVertices(nativeEffect);
		if(numTriangles == 0 || numVertices == 0) {
			return;
		}

		particleVertexData[emitterIndex].Resize((int)numTriangles, (int)numVertices);

		if(numVertices > particleLifeBuffer.Length) {
			Array.Resize(ref particlePositionBuffer, (int)numVertices * 2);
			Array.Resize(ref particleTextureCoordBuffer, (int)numVertices * 2);
			Array.Resize(ref particleColorBuffer, (int)numVertices * 4);
			Array.Resize(ref particleVelocityBuffer, (int)numVertices * 2);
			Array.Resize(ref particleForceBuffer, (int)numVertices * 2);
			Array.Resize(ref particleLifeBuffer, (int)numVertices);
			Array.Resize(ref particleIdBuffer, (int)numVertices);
		}

		Plugin.PixelpartGetParticleTriangleData(
			nativeEffect,
			scaleX,
			scaleY,
			particleVertexData[emitterIndex].triangles,
			particlePositionBuffer,
			particleTextureCoordBuffer,
			particleColorBuffer,
			particleVelocityBuffer,
			particleForceBuffer,
			particleLifeBuffer,
			particleIdBuffer);

		UpdateMesh(
			particleMeshes[emitterIndex],
			particleVertexData[emitterIndex],
			particlePositionBuffer,
			particleTextureCoordBuffer,
			particleColorBuffer,
			particleVelocityBuffer,
			particleForceBuffer,
			particleLifeBuffer,
			particleIdBuffer,
			numVertices,
			layer);
		UpdateMaterial(
			particleMaterials[emitterIndex],
			effectTime,
			Plugin.PixelpartParticleEmitterGetLocalTime(nativeEffect, emitterId),
			Plugin.PixelpartParticleEmitterGetBlendMode(nativeEffect, emitterId));

		Graphics.DrawMesh(
			particleMeshes[emitterIndex],
			GetFinalTransform(),
			particleMaterials[emitterIndex],
			gameObject.layer,
			null,
			0,
			null,
			CastShadows,
			ReceiveShadows,
			ProbeAnchor,
			UseLightProbes);
	}
	private void DrawSprite(uint spriteIndex, uint spriteId, uint layer, float effectTime) {
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);
		bool visible = Plugin.PixelpartSpriteIsVisible(nativeEffect, spriteId);
		bool active = Plugin.PixelpartSpriteIsActive(nativeEffect, spriteId);
		if(!visible || !active) {
			return;
		}

		Plugin.PixelpartPrepareSpriteMeshBuild(nativeEffect, spriteIndex);
		Plugin.PixelpartGetSpriteTriangleData(
			nativeEffect,
			scaleX,
			scaleY,
			spriteVertexData[spriteIndex].triangles,
			spritePositionBuffer,
			spriteTextureCoordBuffer,
			spriteColorBuffer,
			spriteLifeBuffer);

		UpdateMesh(
			spriteMeshes[spriteIndex],
			spriteVertexData[spriteIndex],
			spritePositionBuffer,
			spriteTextureCoordBuffer,
			spriteColorBuffer,
			null,
			null,
			spriteLifeBuffer,
			null,
			4,
			layer);
		UpdateMaterial(
			spriteMaterials[spriteIndex],
			effectTime,
			Plugin.PixelpartSpriteGetLocalTime(nativeEffect, spriteId),
			Plugin.PixelpartSpriteGetBlendMode(nativeEffect, spriteId));

		Graphics.DrawMesh(
			spriteMeshes[spriteIndex],
			GetFinalTransform(),
			spriteMaterials[spriteIndex],
			gameObject.layer,
			null,
			0,
			null,
			CastShadows,
			ReceiveShadows,
			ProbeAnchor,
			UseLightProbes);
	}

	private void UpdateMesh(Mesh mesh, PixelpartVertexData vertexData, float[] positionBuffer, float[] textureCoordBuffer, float[] colorBuffer, float[] velocityBuffer, float[] forceBuffer, float[] lifeBuffer, int[] idBuffer, uint size, uint layer) {
		mesh.Clear();
		mesh.MarkDynamic();

		if(positionBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.positions[i].Set(
					positionBuffer[i * 2 + 0],
					positionBuffer[i * 2 + 1],
					-0.001f * (float)layer);
			}

			mesh.vertices = vertexData.positions;
		}

		if(textureCoordBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.textureCoords[i].Set(
					textureCoordBuffer[i * 2 + 0],
					textureCoordBuffer[i * 2 + 1]);
			}

			mesh.uv = vertexData.textureCoords;
		}

		if(colorBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.colors[i] = new Color(
					colorBuffer[i * 4 + 0],
					colorBuffer[i * 4 + 1],
					colorBuffer[i * 4 + 2],
					colorBuffer[i * 4 + 3]);
			}

			mesh.colors = vertexData.colors;
		}

		if(velocityBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.velocities[i].Set(
					velocityBuffer[i * 2 + 0],
					velocityBuffer[i * 2 + 1]);
			}

			mesh.uv2 = vertexData.velocities;
		}

		if(forceBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.forces[i].Set(
					forceBuffer[i * 2 + 0],
					forceBuffer[i * 2 + 1]);
			}

			mesh.uv3 = vertexData.forces;
		}

		if(lifeBuffer != null || idBuffer != null) {
			for(uint i = 0; i < size; i++) {
				vertexData.objectInfo[i].Set(
					(lifeBuffer != null) ? lifeBuffer[i] : 0.0f,
					(idBuffer != null) ? (float)idBuffer[i] : 0.0f);
			}

			mesh.uv4 = vertexData.objectInfo;
		}

		mesh.triangles = vertexData.triangles;
	}
	private void UpdateMaterial(Material material, float effectTime, float objectTime, int blendMode) {
		material.SetFloat("_EffectTime", effectTime);
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

	private Matrix4x4 GetFinalTransform() {
		if(Billboard == BillboardMode.Enabled) {
			Vector3 cameraPosition = Camera.main.transform.position;

			return Matrix4x4.LookAt(
				transform.position,
				transform.position * 2.0f - cameraPosition,
				Vector3.up);
		}
		else if(Billboard == BillboardMode.EnabledFixedY) {
			Vector3 cameraPosition = Camera.main.transform.position;
			cameraPosition.y = 0.0f;

			return Matrix4x4.LookAt(
				transform.position,
				transform.position * 2.0f - cameraPosition,
				Vector3.up);
		}

		return transform.localToWorldMatrix;
	}
}
}