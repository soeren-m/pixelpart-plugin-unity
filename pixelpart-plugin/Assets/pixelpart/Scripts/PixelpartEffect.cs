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
	public Shader Shader = null;
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
	private float[] particleUVBuffer = null;
	private float[] particleColorBuffer = null;

	private PixelpartVertexData[] spriteVertexData = null;
	private float[] spritePositionBuffer = null;
	private float[] spriteUVBuffer = null;
	private float[] spriteColorBuffer = null;

	private uint[] sortedParticleEmitterIndices = null;
	private uint[] sortedSpriteIndices = null;

	private Dictionary<string, Texture2D> textures = new Dictionary<string, Texture2D>();

	public void OnDestroy() {
		if(nativeEffect != IntPtr.Zero) {
			Plugin.PixelpartDeleteEffect(nativeEffect);
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
		if(asset == cachedEffectAsset) {
			return;
		}

		EffectAsset = asset;
	
		if(nativeEffect != IntPtr.Zero) {
			textures.Clear();

			Plugin.PixelpartDeleteEffect(nativeEffect);
			nativeEffect = IntPtr.Zero;
		}

		if(EffectAsset) {
			nativeEffect = EffectAsset.LoadEffect();

			if(nativeEffect != IntPtr.Zero) {
				uint numParticlesMax = Plugin.PixelpartGetMaxNumParticles();
				uint numParticleEmitters = Plugin.PixelpartGetEffectNumParticleEmitters(nativeEffect);
				uint numSprites = Plugin.PixelpartGetEffectNumSprites(nativeEffect);

				sortedParticleEmitterIndices = new uint[numParticleEmitters];
				particleMeshes = new Mesh[numParticleEmitters];
				particleMaterials = new Material[numParticleEmitters];
				particleVertexData = new PixelpartVertexData[numParticleEmitters];
				particlePositionBuffer = new float[numParticlesMax * 4 * 2];
				particleUVBuffer = new float[numParticlesMax * 4 * 2];
				particleColorBuffer = new float[numParticlesMax * 4 * 4];

				sortedSpriteIndices = new uint[numSprites];
				spriteMeshes = new Mesh[numSprites];
				spriteMaterials = new Material[numSprites];
				spriteVertexData = new PixelpartVertexData[numSprites];
				spritePositionBuffer = new float[4 * 2];
				spriteUVBuffer = new float[4 * 2];
				spriteColorBuffer = new float[4 * 4];

				byte[] nameBuffer = new byte[256];

				for(uint emitterIndex = 0; emitterIndex < numParticleEmitters; emitterIndex++) {
					int nameLength = Plugin.PixelpartGetEffectParticleImageId(nativeEffect, emitterIndex, nameBuffer, nameBuffer.Length);
					string name = System.Text.Encoding.UTF8.GetString(nameBuffer, 0, nameLength);

					if(!textures.ContainsKey(name)) {
						textures[name] = new Texture2D(
							(int)Plugin.PixelpartGetEffectResourceImageWidth(nativeEffect, name),
							(int)Plugin.PixelpartGetEffectResourceImageHeight(nativeEffect, name),
							TextureFormat.RGBA32, false);
					}

					Material material = new Material(Shader);
					material.mainTexture = textures[name];
					particleMaterials[emitterIndex] = material;

					particleMeshes[emitterIndex] = new Mesh();
					particleVertexData[emitterIndex] = new PixelpartVertexData(2, 4);
				}

				for(uint spriteIndex = 0; spriteIndex < numSprites; spriteIndex++) {
					int nameLength = Plugin.PixelpartGetEffectSpriteImageId(nativeEffect, spriteIndex, nameBuffer, nameBuffer.Length);
					string name = System.Text.Encoding.UTF8.GetString(nameBuffer, 0, nameLength);

					if(!textures.ContainsKey(name)) {
						textures[name] = new Texture2D(
							(int)Plugin.PixelpartGetEffectResourceImageWidth(nativeEffect, name),
							(int)Plugin.PixelpartGetEffectResourceImageHeight(nativeEffect, name),
							TextureFormat.RGBA32, false);
					}

					Material material = new Material(Shader);
					material.mainTexture = textures[name];
					spriteMaterials[spriteIndex] = material;

					spriteMeshes[spriteIndex] = new Mesh();
					spriteVertexData[spriteIndex] = new PixelpartVertexData(2, 4);
				}

				foreach(var entry in textures) {
					uint textureDataSize = Plugin.PixelpartGetEffectResourceImageDataSize(nativeEffect, entry.Key);
					byte[] textureData = new byte[textureDataSize];
					Plugin.PixelpartGetEffectResourceImageData(nativeEffect, entry.Key, textureData);

					Texture2D texture = entry.Value;
					texture.filterMode = FilterMode.Bilinear;
					texture.wrapMode = TextureWrapMode.Repeat;
					texture.LoadRawTextureData(textureData);
					texture.Apply();
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

		if(numVertices * 2 > particlePositionBuffer.Length) {
			Array.Resize(ref particlePositionBuffer, (int)numVertices * 2);
			Array.Resize(ref particleUVBuffer, (int)numVertices * 2);
			Array.Resize(ref particleColorBuffer, (int)numVertices * 4);
		}

		Plugin.PixelpartGetParticleTriangleData(
			nativeEffect,
			scaleX,
			scaleY,
			particleVertexData[emitterIndex].triangles,
			particlePositionBuffer,
			particleUVBuffer,
			particleColorBuffer);

		UpdateMesh(
			particleMeshes[emitterIndex],
			particleVertexData[emitterIndex],
			particlePositionBuffer,
			particleUVBuffer,
			particleColorBuffer,
			numVertices,
			layer);

		UpdateMaterial(particleMaterials[emitterIndex],
			Plugin.PixelpartParticleEmitterGetBlendMode(nativeEffect, emitterId),
			Plugin.PixelpartParticleEmitterGetColorMode(nativeEffect, emitterId),
			Plugin.PixelpartParticleEmitterGetAlphaThreshold(nativeEffect, emitterId));

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
	private void DrawSprite(uint spriteIndex, uint layer) {
		float scaleX = EffectAsset.Scale * (FlipH ? -1.0f : +1.0f);
		float scaleY = EffectAsset.Scale * (FlipV ? -1.0f : +1.0f);
		bool active = Plugin.PixelpartSpriteIsActive(nativeEffect, spriteIndex);
		bool visible = Plugin.PixelpartSpriteIsVisible(nativeEffect, spriteIndex);
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
			spriteUVBuffer,
			spriteColorBuffer);

		UpdateMesh(spriteMeshes[spriteIndex],
			spriteVertexData[spriteIndex],
			spritePositionBuffer,
			spriteUVBuffer,
			spriteColorBuffer,
			4,
			layer);
		
		UpdateMaterial(spriteMaterials[spriteIndex],
			Plugin.PixelpartSpriteGetBlendMode(nativeEffect, spriteIndex),
			Plugin.PixelpartSpriteGetColorMode(nativeEffect, spriteIndex),
			0.0f);

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

	private void UpdateMesh(Mesh mesh, PixelpartVertexData vertexData, float[] positionBuffer, float[] uvBuffer, float[] colorBuffer, uint size, uint layer) {
		for(uint i = 0; i < size; i++) {
			vertexData.positions[i].Set(
				positionBuffer[i * 2 + 0],
				positionBuffer[i * 2 + 1],
				-0.001f * (float)layer);
			vertexData.uvs[i].Set(
				uvBuffer[i * 2 + 0],
				uvBuffer[i * 2 + 1]);
			vertexData.colors[i] = new Color(
				colorBuffer[i * 4 + 0],
				colorBuffer[i * 4 + 1],
				colorBuffer[i * 4 + 2],
				colorBuffer[i * 4 + 3]);
		}

		mesh.Clear();
		mesh.MarkDynamic();
		mesh.vertices = vertexData.positions;
		mesh.uv = vertexData.uvs;
		mesh.colors = vertexData.colors;
		mesh.triangles = vertexData.triangles;
		mesh.RecalculateNormals();
	}
	private void UpdateMaterial(Material material, int blendMode, int colorMode, float alphaThreshold) {
		material.shader = Shader;
		material.SetInt("_ColorMode", colorMode);
		material.SetFloat("_AlphaThreshold", alphaThreshold);

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