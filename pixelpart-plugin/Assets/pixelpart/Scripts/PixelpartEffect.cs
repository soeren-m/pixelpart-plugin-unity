using System;
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
	public Shader Shader = null;
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

	private PixelpartMeshData[] particleMeshData = null;
	private float[] particlePositionBuffer = null;
	private float[] particleUVBuffer = null;
	private float[] particleColorBuffer = null;

	private PixelpartMeshData[] spriteMeshData = null;
	private float[] spritePositionBuffer = null;
	private float[] spriteUVBuffer = null;
	private float[] spriteColorBuffer = null;

	private uint[] sortedParticleEmitterIndices = null;
	private uint[] sortedSpriteIndices = null;
	
	public void OnDestroy() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartDestroyEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}
	}
	
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

			Draw();
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
		EffectAsset = asset;

		if(EffectAsset != cachedEffectAsset) {
			if(nativeEffect != IntPtr.Zero) {
				Plugin.PixelpartDestroyEffect(nativeEffect);
				nativeEffect = IntPtr.Zero;
			}

			if(EffectAsset) {
				nativeEffect = EffectAsset.LoadEffect();

				if(nativeEffect != IntPtr.Zero) {
					uint maxNumVertices = Plugin.PixelpartGetEffectMaxNumVerticesPerEmitter(nativeEffect);
					uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
					uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);

					sortedParticleEmitterIndices = new uint[numParticleEmitters];
					sortedSpriteIndices = new uint[numSprites];
							
					particleMeshes = new Mesh[numParticleEmitters];
					particleMaterials = new Material[numParticleEmitters];
					particleMeshData = new PixelpartMeshData[numParticleEmitters];
					particlePositionBuffer = new float[maxNumVertices * 2];
					particleUVBuffer = new float[maxNumVertices * 2];
					particleColorBuffer = new float[maxNumVertices * 4];

					for(uint emitterIndex = 0; emitterIndex < numParticleEmitters; emitterIndex++) {
						uint textureWidth = Plugin.PixelpartGetEffectParticleTextureWidth(nativeEffect, emitterIndex);
						uint textureHeight = Plugin.PixelpartGetEffectParticleTextureHeight(nativeEffect, emitterIndex);
						uint textureDataSize = Plugin.PixelpartGetEffectParticleTextureDataSize(nativeEffect, emitterIndex);
						byte[] textureData = new byte[textureDataSize];
						Plugin.PixelpartGetEffectParticleTextureData(nativeEffect, emitterIndex, textureData);

						Texture2D texture = new Texture2D((int)textureWidth, (int)textureHeight, TextureFormat.RGBA32, false);
						texture.filterMode = FilterMode.Bilinear;
						texture.wrapMode = TextureWrapMode.Repeat;
						texture.LoadRawTextureData(textureData);
						texture.Apply();

						Material material = new Material(Shader);
						material.mainTexture = texture;
						particleMaterials[emitterIndex] = material;

						particleMeshes[emitterIndex] = new Mesh();

						particleMeshData[emitterIndex] = new PixelpartMeshData(2, 4);
					}
					
					spriteMeshes = new Mesh[numSprites];
					spriteMaterials = new Material[numSprites];
					spriteMeshData = new PixelpartMeshData[numSprites];
					spritePositionBuffer = new float[4 * 2];
					spriteUVBuffer = new float[4 * 2];
					spriteColorBuffer = new float[4 * 4];

					for(uint spriteIndex = 0; spriteIndex < numSprites; spriteIndex++) {
						uint textureWidth = Plugin.PixelpartGetEffectSpriteTextureWidth(nativeEffect, spriteIndex);
						uint textureHeight = Plugin.PixelpartGetEffectSpriteTextureHeight(nativeEffect, spriteIndex);
						uint textureDataSize = Plugin.PixelpartGetEffectParticleTextureDataSize(nativeEffect, spriteIndex);
						byte[] textureData = new byte[textureDataSize];
						Plugin.PixelpartGetEffectSpriteTextureData(nativeEffect, spriteIndex, textureData);

						Texture2D texture = new Texture2D((int)textureWidth, (int)textureHeight, TextureFormat.RGBA32, false);
						texture.filterMode = FilterMode.Bilinear;
						texture.LoadRawTextureData(textureData);
						texture.Apply();

						Material material = new Material(Shader);
						material.mainTexture = texture;
						spriteMaterials[spriteIndex] = material;

						spriteMeshes[spriteIndex] = new Mesh();

						spriteMeshData[spriteIndex] = new PixelpartMeshData(2, 4);
					}
				}
			}

			cachedEffectAsset = EffectAsset;
		}
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
	public PixelpartForceField GetForceField(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint index = Plugin.PixelpartFindForceField(nativeEffect, name);
			if(index != NullId) {
				return new PixelpartForceField(nativeEffect, index);
			}
		}

		return null;
	}
	public PixelpartCollider GetCollider(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint index = Plugin.PixelpartFindCollider(nativeEffect, name);
			if(index != NullId) {
				return new PixelpartCollider(nativeEffect, index);
			}
		}

		return null;
	}
	public PixelpartSprite GetSprite(string name) {
		if(nativeEffect != IntPtr.Zero) {
			uint index = Plugin.PixelpartFindSprite(nativeEffect, name);
			if(index != NullId) {
				return new PixelpartSprite(nativeEffect, index);
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
	public PixelpartParticleEmitter GetParticleEmitterByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			uint id = Plugin.PixelpartFindParticleEmitterByIndex(nativeEffect, index);
			if(id != NullId) {
				return new PixelpartParticleEmitter(nativeEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField GetForceFieldByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartForceField(nativeEffect, index);
		}

		return null;
	}
	public PixelpartCollider GetColliderByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartCollider(nativeEffect, index);
		}

		return null;
	}
	public PixelpartSprite GetSpriteByIndex(uint index) {
		if(nativeEffect != IntPtr.Zero) {
			return new PixelpartSprite(nativeEffect, index);
		}

		return null;
	}

	private void Draw() {
		uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
		uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);
		Plugin.PixelpartGetParticleEmittersSortedByLayer(nativeEffect, sortedParticleEmitterIndices);
		Plugin.PixelpartGetSpritesSortedByLayer(nativeEffect, sortedSpriteIndices);

		uint maxLayer = Plugin.PixelpartGetEffectMaxLayer(nativeEffect);

		uint e = 0;
		uint s = 0;
		for(uint l = 0; l <= maxLayer; l++) {
			while(s < numSprites) {
				uint spriteIndex = sortedSpriteIndices[s];
				uint spriteLayer = Plugin.PixelpartSpriteGetLayer(nativeEffect, spriteIndex);
				if(spriteLayer != l) {
					break;
				}

				DrawSprite(spriteIndex, spriteLayer);

				s++;
			}

			while(e < numParticleEmitters) {
				uint emitterIndex = sortedParticleEmitterIndices[e];
				uint emitterId = Plugin.PixelpartGetParticleEmitterId(nativeEffect, emitterIndex);
				uint emitterLayer = Plugin.PixelpartParticleEmitterGetLayer(nativeEffect, emitterId);
				if(emitterLayer != l) {
					break;
				}

				DrawParticles(emitterIndex, emitterId, emitterLayer);

				e++;
			}
		}
	}

	private void DrawParticles(uint emitterIndex, uint emitterId, uint layer) {
		uint numTriangles = Plugin.PixelpartGetEffectNumParticleTriangles(nativeEffect, emitterIndex);
		uint numVertices = Plugin.PixelpartGetEffectNumParticleVertices(nativeEffect, emitterIndex);
		bool visible = Plugin.PixelpartParticleEmitterIsVisible(nativeEffect, emitterId);
		if(!visible || numTriangles == 0) {
			return;
		}
	
		int blendMode = Plugin.PixelpartParticleEmitterGetBlendMode(nativeEffect, emitterId);
		int colorMode = Plugin.PixelpartParticleEmitterGetColorMode(nativeEffect, emitterId);
		float alphaThreshold = Plugin.PixelpartParticleEmitterGetAlphaThreshold(nativeEffect, emitterId);
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);

		particleMeshData[emitterIndex].Resize((int)numTriangles, (int)numVertices);

		Plugin.PixelpartFillParticleTriangleData(
			nativeEffect,
			emitterIndex,
			scaleX,
			scaleY,
			particleMeshData[emitterIndex].triangles,
			particlePositionBuffer,
			particleUVBuffer,
			particleColorBuffer);

		for(uint i = 0; i < numVertices; i++) {
			particleMeshData[emitterIndex].positions[i].Set(
				particlePositionBuffer[i * 2 + 0],
				particlePositionBuffer[i * 2 + 1],
				-0.001f * (float)layer);
			particleMeshData[emitterIndex].uvs[i].Set(
				particleUVBuffer[i * 2 + 0],
				particleUVBuffer[i * 2 + 1]);
			particleMeshData[emitterIndex].colors[i] = new Color(
				particleColorBuffer[i * 4 + 0],
				particleColorBuffer[i * 4 + 1],
				particleColorBuffer[i * 4 + 2],
				particleColorBuffer[i * 4 + 3]);
		}

		particleMeshes[emitterIndex].Clear();
		particleMeshes[emitterIndex].MarkDynamic();
		particleMeshes[emitterIndex].vertices = particleMeshData[emitterIndex].positions;
		particleMeshes[emitterIndex].uv = particleMeshData[emitterIndex].uvs;
		particleMeshes[emitterIndex].colors = particleMeshData[emitterIndex].colors;
		particleMeshes[emitterIndex].triangles = particleMeshData[emitterIndex].triangles;
		particleMeshes[emitterIndex].RecalculateNormals();

		particleMaterials[emitterIndex].shader = Shader;
		particleMaterials[emitterIndex].SetInt("_ColorMode", colorMode);
		particleMaterials[emitterIndex].SetFloat("_AlphaThreshold", alphaThreshold);

		switch(blendMode) {
			case 1:
				particleMaterials[emitterIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				particleMaterials[emitterIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				particleMaterials[emitterIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				break;
			case 2:
				particleMaterials[emitterIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.ReverseSubtract);
				particleMaterials[emitterIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				particleMaterials[emitterIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;
			default:
				particleMaterials[emitterIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				particleMaterials[emitterIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				particleMaterials[emitterIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;
		}

		Graphics.DrawMesh(
			particleMeshes[emitterIndex],
			transform.localToWorldMatrix,
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

	private void DrawSprite(uint spriteIndex, uint layer) {
		bool active = Plugin.PixelpartSpriteIsActive(nativeEffect, spriteIndex);
		bool visible = Plugin.PixelpartSpriteIsVisible(nativeEffect, spriteIndex);
		if(!visible || !active) {
			return;
		}

		int blendMode = Plugin.PixelpartSpriteGetBlendMode(nativeEffect, spriteIndex);
		int colorMode = Plugin.PixelpartSpriteGetColorMode(nativeEffect, spriteIndex);
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);

		Plugin.PixelpartFillSpriteTriangleData(
			nativeEffect,
			spriteIndex,
			scaleX,
			scaleY,
			spriteMeshData[spriteIndex].triangles,
			spritePositionBuffer,
			spriteUVBuffer,
			spriteColorBuffer);

		for(uint i = 0; i < 4; i++) {
			spriteMeshData[spriteIndex].positions[i].Set(
				spritePositionBuffer[i * 2 + 0],
				spritePositionBuffer[i * 2 + 1],
				-0.001f * (float)layer);
			spriteMeshData[spriteIndex].uvs[i].Set(
				spriteUVBuffer[i * 2 + 0],
				spriteUVBuffer[i * 2 + 1]);
			spriteMeshData[spriteIndex].colors[i] = new Color(
				spriteColorBuffer[i * 4 + 0],
				spriteColorBuffer[i * 4 + 1],
				spriteColorBuffer[i * 4 + 2],
				spriteColorBuffer[i * 4 + 3]);
		}

		spriteMeshes[spriteIndex].Clear();
		spriteMeshes[spriteIndex].MarkDynamic();
		spriteMeshes[spriteIndex].vertices = spriteMeshData[spriteIndex].positions;
		spriteMeshes[spriteIndex].uv = spriteMeshData[spriteIndex].uvs;
		spriteMeshes[spriteIndex].colors = spriteMeshData[spriteIndex].colors;
		spriteMeshes[spriteIndex].triangles = spriteMeshData[spriteIndex].triangles;
		spriteMeshes[spriteIndex].RecalculateNormals();

		spriteMaterials[spriteIndex].shader = Shader;
		spriteMaterials[spriteIndex].SetInt("_ColorMode", colorMode);
		spriteMaterials[spriteIndex].SetFloat("_AlphaThreshold", 0.0f);

		switch(blendMode) {
			case 1:
				spriteMaterials[spriteIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				spriteMaterials[spriteIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				spriteMaterials[spriteIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				break;

			case 2:
				spriteMaterials[spriteIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.ReverseSubtract);
				spriteMaterials[spriteIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				spriteMaterials[spriteIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;

			default:
				spriteMaterials[spriteIndex].SetInt("_BlendOp", (int)UnityEngine.Rendering.BlendOp.Add);
				spriteMaterials[spriteIndex].SetInt("_SrcBlendMode", (int)UnityEngine.Rendering.BlendMode.One);
				spriteMaterials[spriteIndex].SetInt("_DstBlendMode", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
				break;
		}			

		Graphics.DrawMesh(
			spriteMeshes[spriteIndex],
			transform.localToWorldMatrix,
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
}
}