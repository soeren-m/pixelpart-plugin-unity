using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

#if UNITY_EDITOR
using UnityEditor;
#endif

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

	public List<string> ParticleTypeNames = new List<string>();

	public List<Material> ParticleMaterials = new List<Material>();

	public bool FlipH = false;

	public bool FlipV = false;

	public bool Is3D {
		get {
			return internalEffect != IntPtr.Zero
				? Plugin.PixelpartIsEffect3D(internalEffect)
				: false;
		}
	}

	public float CurrentTime {
		get {
			return internalEffect != IntPtr.Zero
				? Plugin.PixelpartGetEffectTime(internalEffect)
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

	[SerializeField]
	private List<string> effectInputNames = new List<string>();

	[SerializeField]
	private List<PixelpartVariantValue> effectInputValues = new List<PixelpartVariantValue>();

	private IntPtr internalEffect = IntPtr.Zero;

	private PixelpartEffectAsset cachedEffectAsset = null;

	private PixelpartGraphicsResourceProvider graphicsResourceProvider = new PixelpartGraphicsResourceProvider();

	private Dictionary<string, uint> effectInputIdMap = new Dictionary<string, uint>();

	private PixelpartParticleRenderer[] particleRenderers;

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

		if(internalEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartPlayEffect(internalEffect, Playing && gameObject.activeSelf);
		Plugin.PixelpartSetEffectLoop(internalEffect, Loop);
		Plugin.PixelpartSetEffectLoopTime(internalEffect, LoopTime);
		Plugin.PixelpartSetEffectSpeed(internalEffect, Speed);
		Plugin.PixelpartSetEffectTimeStep(internalEffect, 1.0f / FrameRate);
		Plugin.PixelpartUpdateEffect(internalEffect, Time.deltaTime);
	}

	public void LateUpdate() {
		if(internalEffect == IntPtr.Zero) {
			return;
		}

		DrawEffect(Camera.main);
	}

	public void OnDestroy() {
		DeleteEffect();
	}

	public void RestartEffect() {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		Plugin.PixelpartRestartEffect(internalEffect);
	}
	public void ResetEffect() {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		Plugin.PixelpartResetEffect(internalEffect);
	}

	public void SpawnParticles(string particleTypeName, int count) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint particleTypeId = Plugin.PixelpartFindParticleType(internalEffect, particleTypeName);
		if(particleTypeId != NullId) {
			Plugin.PixelpartSpawnParticles(internalEffect, particleTypeId, count);
		}
	}

	public void SetInputBool(string name, bool value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputBool(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public void SetInputInt(string name, int value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputInt(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public void SetInputFloat(string name, float value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputFloat(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public void SetInputFloat2(string name, Vector2 value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputFloat2(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public void SetInputFloat3(string name, Vector3 value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputFloat3(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public void SetInputFloat4(string name, Vector4 value) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return;
		}

		Plugin.PixelpartSetEffectInputFloat4(internalEffect, inputId, value);
		SetInputPropertyValue(name, new PixelpartVariantValue(value));
	}
	public bool GetInputBool(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return false;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return false;
		}

		return Plugin.PixelpartGetEffectInputBool(internalEffect, inputId);
	}
	public int GetInputInt(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return 0;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return 0;
		}

		return Plugin.PixelpartGetEffectInputInt(internalEffect, inputId);
	}
	public float GetInputFloat(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return 0.0f;
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return 0.0f;
		}

		return Plugin.PixelpartGetEffectInputFloat(internalEffect, inputId);
	}
	public Vector2 GetInputFloat2(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return new Vector2(0.0f, 0.0f);
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return new Vector2(0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat2(internalEffect, inputId);
	}
	public Vector3 GetInputFloat3(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return new Vector3(0.0f, 0.0f, 0.0f);
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return new Vector3(0.0f, 0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat3(internalEffect, inputId);
	}
	public Vector4 GetInputFloat4(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		uint inputId = 0;
		if(!effectInputIdMap.TryGetValue(name, out inputId)) {
			Debug.LogWarning("[Pixelpart] Unknown effect input '" + name + "'", this);
			return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat4(internalEffect, inputId);
	}

	public void ApplyInputProperties() {
		for(int inputIndex = 0; inputIndex < effectInputNames.Count && inputIndex < effectInputValues.Count; inputIndex++) {
			ApplyInputProperty(inputIndex);
		}
	}

	public PixelpartParticleEmitter FindParticleEmitter(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindParticleEmitter(internalEffect, name);
		if(id != NullId) {
			return new PixelpartParticleEmitter(internalEffect, id);
		}

		return null;
	}
	public PixelpartParticleType FindParticleType(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindParticleType(internalEffect, name);
		if(id != NullId) {
			return new PixelpartParticleType(internalEffect, id);
		}

		return null;
	}
	public PixelpartForceField FindForceField(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindForceField(internalEffect, name);
		if(id != NullId) {
			return new PixelpartForceField(internalEffect, id);
		}

		return null;
	}
	public PixelpartCollider FindCollider(string name) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindCollider(internalEffect, name);
		if(id != NullId) {
			return new PixelpartCollider(internalEffect, id);
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitter(uint id) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(Plugin.PixelpartHasParticleEmitter(internalEffect, id)) {
			return new PixelpartParticleEmitter(internalEffect, id);
		}

		return null;
	}
	public PixelpartParticleType GetParticleType(uint id) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(Plugin.PixelpartHasParticleType(internalEffect, id)) {
			return new PixelpartParticleType(internalEffect, id);
		}

		return null;
	}
	public PixelpartForceField GetForceField(uint id) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(Plugin.PixelpartHasForceField(internalEffect, id)) {
			return new PixelpartForceField(internalEffect, id);
		}

		return null;
	}
	public PixelpartCollider GetCollider(uint id) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(Plugin.PixelpartHasCollider(internalEffect, id)) {
			return new PixelpartCollider(internalEffect, id);
		}

		return null;
	}

	public PixelpartParticleEmitter GetParticleEmitterAtIndex(int index) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index >= 0) {
			uint id = Plugin.PixelpartFindParticleEmitterByIndex(internalEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartParticleEmitter(internalEffect, id);
			}
		}

		return null;
	}
	public PixelpartParticleType GetParticleTypeAtIndex(int index) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index >= 0) {
			uint id = Plugin.PixelpartFindParticleTypeByIndex(internalEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartParticleType(internalEffect, id);
			}
		}

		return null;
	}
	public PixelpartForceField GetForceFieldAtIndex(int index) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index >= 0) {
			uint id = Plugin.PixelpartFindForceFieldByIndex(internalEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartForceField(internalEffect, id);
			}
		}

		return null;
	}
	public PixelpartCollider GetColliderAtIndex(int index) {
		if(internalEffect == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index >= 0) {
			uint id = Plugin.PixelpartFindColliderByIndex(internalEffect, (uint)index);
			if(id != NullId) {
				return new PixelpartCollider(internalEffect, id);
			}
		}

		return null;
	}

	private void InitEffect() {
		DeleteEffect();

		cachedEffectAsset = EffectAsset;

		if(EffectAsset != null) {
			internalEffect = EffectAsset.LoadEffect();

			if(internalEffect == IntPtr.Zero) {
				Debug.LogError("[Pixelpart] Failed to load effect asset", this);
				return;
			}

			int numEffectInputs = Plugin.PixelpartGetNumEffectInputs(internalEffect);
			uint[] effectInputIdArray = new uint[numEffectInputs];
			int[] effectInputTypeArray = new int[numEffectInputs];

			byte[] effectInputNamesBuffer = new byte[16384];
			int effectInputNamesLength = Plugin.PixelpartGetEffectInputs(internalEffect, effectInputIdArray, effectInputTypeArray, effectInputNamesBuffer, effectInputNamesBuffer.Length);
			string[] effectInputNamesArray = System.Text.Encoding.UTF8.GetString(effectInputNamesBuffer, 0, effectInputNamesLength).
				Split(new[] {'|'}, 64, StringSplitOptions.RemoveEmptyEntries);

			for(int inputIndex = 0; inputIndex < numEffectInputs; inputIndex++) {
				effectInputIdMap[effectInputNamesArray[inputIndex]] = effectInputIdArray[inputIndex];
			}

			ApplyInputProperties();

			graphicsResourceProvider.Load(internalEffect);

			int numParticleTypes = (int)Plugin.PixelpartGetEffectNumParticleTypes(internalEffect);

			particleRenderers = new PixelpartParticleRenderer[numParticleTypes];
			sortedParticleTypeIndices = new uint[numParticleTypes];

			if(ParticleMaterials.Count < numParticleTypes) {
				Debug.LogError("[Pixelpart] Could not find shader for every particle type");
				DeleteEffect();
				return;
			}

			byte[] materialIdBuffer = new byte[2048];

			for(int particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
				uint particleTypeId = Plugin.PixelpartFindParticleTypeByIndex(internalEffect, (uint)particleTypeIndex);
				ParticleRendererType rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(internalEffect, particleTypeId);

				int materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(internalEffect, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
				string materialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
				bool materialBuiltIn = Plugin.PixelpartParticleTypeIsMaterialBuiltIn(internalEffect, particleTypeId);

				PixelpartMaterialInfo materialInfo = null;
				if(materialBuiltIn) {
					materialInfo = PixelpartBuiltInMaterialProvider.Instance.GetMaterialInfo(materialId);
				}
				else {
					PixelpartCustomMaterialAsset customMaterialAsset = EffectAsset.CustomMaterialAssets.FirstOrDefault(
						asset => asset.ResourceId == materialId);
					materialInfo = customMaterialAsset?.MaterialInfo;
				}

				if(materialInfo == null) {
					Debug.LogWarning("[Pixelpart] Could not find shader information for material '" + materialId + "'");
					continue;
				}

				switch(rendererType) {
					case ParticleRendererType.Sprite:
						particleRenderers[particleTypeIndex] = new PixelpartParticleSpriteRenderer(internalEffect, (uint)particleTypeIndex,
							ParticleMaterials[particleTypeIndex], materialInfo,
							graphicsResourceProvider);
						break;
					case ParticleRendererType.Trail:
						particleRenderers[particleTypeIndex] = new PixelpartParticleTrailRenderer(internalEffect, (uint)particleTypeIndex,
							ParticleMaterials[particleTypeIndex], materialInfo,
							graphicsResourceProvider);
						break;
					case ParticleRendererType.Mesh:
						particleRenderers[particleTypeIndex] = new PixelpartParticleMeshRenderer(internalEffect, (uint)particleTypeIndex,
							ParticleMaterials[particleTypeIndex], materialInfo,
							graphicsResourceProvider);
						break;
					default:
						Debug.LogWarning("[Pixelpart] Particle type with id " + particleTypeId + " has unknown renderer type", this);
						break;
				}
			}
		}
	}

	private void DeleteEffect() {
		if(internalEffect == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartDeleteEffect(internalEffect);
		internalEffect = IntPtr.Zero;

		effectInputIdMap.Clear();
		graphicsResourceProvider.Clear();
	}

	private void DrawEffect(Camera camera) {
		uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(internalEffect);

		if(numParticleTypes > 0) {
			Plugin.PixelpartGetParticleTypesSortedForRendering(internalEffect, sortedParticleTypeIndices);
		}

		foreach(uint particleTypeIndex in sortedParticleTypeIndices) {
			DrawParticles(camera,
				particleTypeIndex,
				Plugin.PixelpartFindParticleTypeByIndex(internalEffect, particleTypeIndex));
		}
	}

	private void DrawParticles(Camera camera, uint particleTypeIndex, uint particleTypeId) {
		Vector3 scale = new Vector3(
			EffectAsset.Scale * (FlipH ? -1.0f : +1.0f),
			EffectAsset.Scale * (FlipV ? -1.0f : +1.0f),
			EffectAsset.Scale);

		particleRenderers[particleTypeIndex].Draw(camera, transform, scale, gameObject.layer);
	}

	private void SetInputPropertyValue(string name, PixelpartVariantValue value) {
		int inputIndex = effectInputNames.IndexOf(name);
		if(inputIndex < 0) {
			return;
		}

		effectInputValues[inputIndex] = value;
	}

	private void ApplyInputProperty(int index) {
		uint inputId = NullId;
		if(!effectInputIdMap.TryGetValue(effectInputNames[index], out inputId)) {
			return;
		}

		PixelpartVariantValue value = effectInputValues[index];

		switch(value.type) {
			case PixelpartVariantValue.VariantType.Bool:
				Plugin.PixelpartSetEffectInputBool(internalEffect, inputId, value.x > 0.5f);
				break;
			case PixelpartVariantValue.VariantType.Int:
				Plugin.PixelpartSetEffectInputInt(internalEffect, inputId, (int)value.x);
				break;
			case PixelpartVariantValue.VariantType.Float:
				Plugin.PixelpartSetEffectInputFloat(internalEffect, inputId, value.x);
				break;
			case PixelpartVariantValue.VariantType.Float2:
				Plugin.PixelpartSetEffectInputFloat2(internalEffect, inputId, new Vector2(value.x, value.y));
				break;
			case PixelpartVariantValue.VariantType.Float3:
				Plugin.PixelpartSetEffectInputFloat3(internalEffect, inputId, new Vector3(value.x, value.y, value.z));
				break;
			case PixelpartVariantValue.VariantType.Float4:
				Plugin.PixelpartSetEffectInputFloat4(internalEffect, inputId, new Vector4(value.x, value.y, value.z, value.w));
				break;
			default:
				break;
		}
	}

#if UNITY_EDITOR
	[SerializeField]
	[HideInInspector]
	private PixelpartEffectAsset prevEffectAsset = null;

	public void OnValidate() {
		if(EffectAsset == prevEffectAsset) {
			return;
		}

		prevEffectAsset = EffectAsset;

		effectInputNames.Clear();
		effectInputValues.Clear();
		ParticleTypeNames.Clear();
		ParticleMaterials.Clear();

		if(EffectAsset == null) {
			return;
		}

		IntPtr internalEffectPtr = EffectAsset.LoadEffect();
		if(internalEffectPtr == IntPtr.Zero) {
			return;
		}

		int numEffectInputs = Plugin.PixelpartGetNumEffectInputs(internalEffectPtr);
		uint[] effectInputIdArray = new uint[numEffectInputs];
		int[] effectInputTypeArray = new int[numEffectInputs];

		byte[] effectInputNamesBuffer = new byte[16384];
		int effectInputNamesLength = Plugin.PixelpartGetEffectInputs(internalEffectPtr, effectInputIdArray, effectInputTypeArray, effectInputNamesBuffer, effectInputNamesBuffer.Length);
		string[] effectInputNamesArray = System.Text.Encoding.UTF8.GetString(effectInputNamesBuffer, 0, effectInputNamesLength).
			Split(new[] {'|'}, 64, StringSplitOptions.RemoveEmptyEntries);

		for(int inputIndex = 0; inputIndex < numEffectInputs; inputIndex++) {
			effectInputNames.Add(effectInputNamesArray[inputIndex]);
			effectInputValues.Add(new PixelpartVariantValue(
				(PixelpartVariantValue.VariantType)effectInputTypeArray[inputIndex],
				Plugin.PixelpartGetEffectInputFloat4(internalEffectPtr, effectInputIdArray[inputIndex])));
		}

		uint numParticleTypes = Plugin.PixelpartGetEffectNumParticleTypes(internalEffectPtr);

		for(uint particleTypeIndex = 0; particleTypeIndex < numParticleTypes; particleTypeIndex++) {
			uint particleTypeId = Plugin.PixelpartFindParticleTypeByIndex(internalEffectPtr, (uint)particleTypeIndex);

			byte[] particleTypeNameBuffer = new byte[256];
			int particleTypeNameBufferSize = Plugin.PixelpartParticleTypeGetName(internalEffectPtr, particleTypeId, particleTypeNameBuffer, particleTypeNameBuffer.Length);
			string particleTypeName = System.Text.Encoding.UTF8.GetString(particleTypeNameBuffer, 0, particleTypeNameBufferSize);

			ParticleRendererType rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(internalEffectPtr, particleTypeId);
			bool needsInstancing = rendererType == ParticleRendererType.Mesh;

			byte[] materialIdBuffer = new byte[256];
			int materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(internalEffectPtr, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
			string materialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
			string materialPath = string.Empty;

			if(Plugin.PixelpartParticleTypeIsMaterialBuiltIn(internalEffectPtr, particleTypeId)) {
				materialPath = PixelpartBuiltInMaterialProvider.Instance.ResolveMaterialPath(materialId);
			}
			else {
				PixelpartCustomMaterialAsset customMaterial =
					EffectAsset.CustomMaterialAssets.FirstOrDefault(materialAsset =>
						materialAsset.ResourceId == materialId && materialAsset.Instancing == needsInstancing);

				materialPath = customMaterial?.MaterialInfo.MaterialPath ?? string.Empty;
			}

			Material material = (Material)AssetDatabase.LoadAssetAtPath(materialPath, typeof(Material));

			if(material == null) {
				Debug.LogWarning(
					"[Pixelpart] Failed to find material asset '" + materialPath + "' for material '" + materialId + "'", this);
			}

			ParticleTypeNames.Add(particleTypeName);
			ParticleMaterials.Add(material);
		}

		Plugin.PixelpartDeleteEffect(internalEffectPtr);
	}

	public void OnDrawGizmos() {
		Gizmos.DrawIcon(transform.position, "Packages/net.pixelpart/Editor/Resources/Gizmos/PixelpartEffectIcon.png", true);
	}
#endif
}
}