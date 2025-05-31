using System;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart {
[AddComponentMenu("Pixelpart/Effect")]
public class PixelpartEffect : MonoBehaviour {
#if UNITY_EDITOR
	private const string gizmoIconPath = "Packages/net.pixelpart/Editor/Resources/Gizmos/PixelpartEffectIcon.png";
#endif

	public const uint NullId = 0xFFFFFFFF;

	public PixelpartEffectAsset EffectAsset = null;

	public bool Playing = true;

	public bool Loop = false;

	[Range(0.0f, 100.0f)]
	public float LoopTime = 1.0f;

	[Range(0.0f, 10.0f)]
	public float WarmupTime = 0.0f;

	[Range(0.0f, 10.0f)]
	public float Speed = 1.0f;

	[Range(1.0f, 100.0f)]
	public float FrameRate = 60.0f;

	public float EffectScale = 1.0f;

	public bool FlipH = false;

	public bool FlipV = false;

	public List<string> ParticleTypeNames = new List<string>();

	public List<Material> ParticleMaterials = new List<Material>();

	public bool Is3D => effectRuntime != IntPtr.Zero
		? Plugin.PixelpartIsEffect3d(effectRuntime) : false;

	public float CurrentTime => effectRuntime != IntPtr.Zero
		? Plugin.PixelpartGetEffectTime(effectRuntime) : 0.0f;

	[SerializeField]
	private List<string> effectInputNames = new List<string>();

	[SerializeField]
	private List<PixelpartVariantValue> effectInputValues = new List<PixelpartVariantValue>();

	private IntPtr effectRuntime = IntPtr.Zero;

	private PixelpartEffectAsset cachedEffectAsset = null;

	private PixelpartEffectInputCollection effectInputCollection = new PixelpartEffectInputCollection();

	private PixelpartTriggerCollection triggerCollection = new PixelpartTriggerCollection();

	private PixelpartEffectRenderer effectRenderer = null;

	public PixelpartEffect() {

	}

	public void Awake() {
		InitEffect();
	}

	public void Update() {
		if(EffectAsset != cachedEffectAsset) {
			InitEffect();
		}

		if(effectRuntime == IntPtr.Zero || !Playing || !gameObject.activeSelf) {
			return;
		}

		UpdateTransform();

		var timeStep = 1.0f / Math.Max(FrameRate, 0.01f);
		Plugin.PixelpartAdvanceEffect(effectRuntime,
			Time.deltaTime,
			Loop, LoopTime,
			Speed,
			timeStep);
	}

	public void LateUpdate() {
		if(effectRuntime == IntPtr.Zero || effectRenderer == null) {
			return;
		}

		var scale = new Vector3(
			EffectScale * (FlipH ? -1.0f : +1.0f),
			EffectScale * (FlipV ? -1.0f : +1.0f),
			EffectScale);

		effectRenderer.Render(Camera.main, transform, scale, gameObject.layer);
	}

	public void OnDestroy() {
		DeleteEffect();
	}

	public void RestartEffect() {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		Plugin.PixelpartRestartEffect(effectRuntime, true);
	}

	public void ResetEffect() {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		Plugin.PixelpartRestartEffect(effectRuntime, false);
	}

	public void SpawnParticles(string particleEmitterName, string particleTypeName, int count) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return;
		}

		var particleEmitterId = Plugin.PixelpartFindNode(effectRuntime, particleEmitterName);
		if(particleEmitterId == NullId) {
			Debug.LogWarning("[Pixelpart] Unknown particle emitter \"" + particleEmitterName + "\"", this);
			return;
		}

		var particleTypeId = Plugin.PixelpartFindParticleType(effectRuntime, particleTypeName);
		if(particleTypeId == NullId) {
			Debug.LogWarning("[Pixelpart] Unknown particle type \"" + particleTypeName + "\"", this);
			return;
		}

		Plugin.PixelpartSpawnParticles(effectRuntime, particleEmitterId, particleTypeId, count);
	}

	public void SetInputBool(string inputName, bool value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputBool(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public void SetInputInt(string inputName, int value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputInt(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public void SetInputFloat(string inputName, float value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputFloat(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public void SetInputFloat2(string inputName, Vector2 value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputFloat2(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public void SetInputFloat3(string inputName, Vector3 value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputFloat3(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public void SetInputFloat4(string inputName, Vector4 value) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return;
		}

		Plugin.PixelpartSetEffectInputFloat4(effectRuntime, inputId, value);
		SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
	}

	public bool GetInputBool(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return false;
		}

		return Plugin.PixelpartGetEffectInputBool(effectRuntime, inputId);
	}

	public int GetInputInt(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return 0;
		}

		return Plugin.PixelpartGetEffectInputInt(effectRuntime, inputId);
	}

	public float GetInputFloat(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return 0.0f;
		}

		return Plugin.PixelpartGetEffectInputFloat(effectRuntime, inputId);
	}

	public Vector2 GetInputFloat2(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return new Vector2(0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat2(effectRuntime, inputId);
	}

	public Vector3 GetInputFloat3(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return new Vector3(0.0f, 0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat3(effectRuntime, inputId);
	}

	public Vector4 GetInputFloat4(string inputName) {
		if(!effectInputCollection.TryGetInputId(inputName, out uint inputId)) {
			return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		return Plugin.PixelpartGetEffectInputFloat4(effectRuntime, inputId);
	}

	public void ApplyInputProperties() {
		for(var inputIndex = 0; inputIndex < effectInputNames.Count && inputIndex < effectInputValues.Count; inputIndex++) {
			ApplyInputProperty(inputIndex);
		}
	}

	public void ActivateTrigger(string triggerName) {
		if(!triggerCollection.TryGetTriggerId(triggerName, out uint triggerId)) {
			return;
		}

		Plugin.PixelpartActivateTrigger(effectRuntime, triggerId);
	}

	public bool IsTriggerActivated(string triggerName) {
		if(!triggerCollection.TryGetTriggerId(triggerName, out uint triggerId)) {
			return false;
		}

		return Plugin.PixelpartIsTriggerActivated(effectRuntime, triggerId);
	}

	public PixelpartNode FindNode(string nodeName) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindNode(effectRuntime, nodeName);
		if(!Plugin.PixelpartNodeExists(effectRuntime, id)) {
			return null;
		}

		var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
		if(node == null) {
			Debug.LogWarning("[Pixelpart] Failed to create node", this);
		}

		return node;
	}

	public PixelpartNode GetNode(uint id) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(!Plugin.PixelpartNodeExists(effectRuntime, id)) {
			return null;
		}

		var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
		if(node == null) {
			Debug.LogWarning("[Pixelpart] Failed to create node", this);
		}

		return node;
	}

	public PixelpartNode GetNodeAtIndex(int index) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index < 0) {
			Debug.LogWarning("[Pixelpart] Node index must be positive", this);
			return null;
		}

		uint id = Plugin.PixelpartFindNodeByIndex(effectRuntime, index);
		if(id == NullId) {
			return null;
		}

		var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
		if(node == null) {
			Debug.LogWarning("[Pixelpart] Failed to create node", this);
		}

		return node;
	}

	public PixelpartParticleType FindParticleType(string particleTypeName) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		uint id = Plugin.PixelpartFindParticleType(effectRuntime, particleTypeName);
		if(id == NullId) {
			return null;
		}

		return new PixelpartParticleType(effectRuntime, id);
	}

	public PixelpartParticleType GetParticleType(uint id) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(!Plugin.PixelpartParticleTypeExists(effectRuntime, id)) {
			return null;
		}

		return new PixelpartParticleType(effectRuntime, id);
	}

	public PixelpartParticleType GetParticleTypeAtIndex(int index) {
		if(effectRuntime == IntPtr.Zero) {
			Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
			return null;
		}

		if(index < 0) {
			Debug.LogWarning("[Pixelpart] Particle type index must be positive", this);
			return null;
		}

		uint id = Plugin.PixelpartFindParticleTypeByIndex(effectRuntime, index);
		if(id == NullId) {
			return null;
		}

		return new PixelpartParticleType(effectRuntime, id);
	}

	[Obsolete("deprecated, use FindNode")]
	public PixelpartParticleEmitter FindParticleEmitter(string nodeName) => (PixelpartParticleEmitter)FindNode(nodeName);

	[Obsolete("deprecated, use FindNode")]
	public PixelpartForceField FindForceField(string nodeName) => (PixelpartForceField)FindNode(nodeName);

	[Obsolete("deprecated, use FindNode")]
	public PixelpartCollider FindCollider(string nodeName) => (PixelpartCollider)FindNode(nodeName);

	[Obsolete("deprecated, use GetNode")]
	public PixelpartParticleEmitter GetParticleEmitter(uint id) => (PixelpartParticleEmitter)GetNode(id);

	[Obsolete("deprecated, use GetNode")]
	public PixelpartForceField GetForceField(uint id) => (PixelpartForceField)GetNode(id);

	[Obsolete("deprecated, use GetNode")]
	public PixelpartCollider GetCollider(uint id) => (PixelpartCollider)GetNode(id);

	[Obsolete("deprecated, use GetNodeAtIndex")]
	public PixelpartParticleEmitter GetParticleEmitterAtIndex(int index) => (PixelpartParticleEmitter)GetNodeAtIndex(index);

	[Obsolete("deprecated, use GetNodeAtIndex")]
	public PixelpartForceField GetForceFieldAtIndex(int index) => (PixelpartForceField)GetNodeAtIndex(index);

	[Obsolete("deprecated, use GetNodeAtIndex")]
	public PixelpartCollider GetColliderAtIndex(int index) => (PixelpartCollider)GetNodeAtIndex(index);

	private void InitEffect() {
		DeleteEffect();

		cachedEffectAsset = EffectAsset;
		if(EffectAsset == null) {
			return;
		}

		try {
			effectRuntime = EffectAsset.LoadEffect();
		}
		catch(InvalidOperationException e) {
			Debug.LogError("[Pixelpart] Failed to load effect asset: " + e.Message, this);
			return;
		}

		effectInputCollection = new PixelpartEffectInputCollection(effectRuntime, effectInputNames, effectInputValues);
		triggerCollection = new PixelpartTriggerCollection(effectRuntime);
		effectRenderer = new PixelpartEffectRenderer(effectRuntime, ParticleMaterials, EffectAsset.CustomMaterials);

		if(WarmupTime > 0.0f) {
			UpdateTransform();

			var timeStep = 1.0f / Math.Max(FrameRate, 0.01f);
			Plugin.PixelpartAdvanceEffect(effectRuntime,
				WarmupTime, false, 0.0f, 1.0f, timeStep);
		}

		ApplyInputProperties();
	}

	private void DeleteEffect() {
		if(effectRuntime == IntPtr.Zero) {
			return;
		}

		Plugin.PixelpartDeleteEffect(effectRuntime);
		effectRuntime = IntPtr.Zero;

		effectInputCollection = new PixelpartEffectInputCollection();
		triggerCollection = new PixelpartTriggerCollection();
		effectRenderer = null;
	}

	private void UpdateTransform() {
		var scale = new Vector3(
			EffectScale * (FlipH ? -1.0f : +1.0f),
			EffectScale * (FlipV ? -1.0f : +1.0f),
			EffectScale);

		Plugin.PixelpartSetEffectTransform(effectRuntime,
			transform.localToWorldMatrix, scale);
	}

	private void SetInputPropertyValue(string inputName, PixelpartVariantValue value) {
		int inputIndex = effectInputNames.IndexOf(inputName);
		if(inputIndex < 0) {
			return;
		}

		effectInputValues[inputIndex] = value;
	}

	private void ApplyInputProperty(int index) {
		if(!effectInputCollection.TryGetInputId(effectInputNames[index], out uint inputId)) {
			return;
		}

		PixelpartVariantValue value = effectInputValues[index];

		switch(value.type) {
			case PixelpartVariantValue.VariantType.Bool:
				Plugin.PixelpartSetEffectInputBool(effectRuntime, inputId, value.x > 0.5f);
				break;
			case PixelpartVariantValue.VariantType.Int:
				Plugin.PixelpartSetEffectInputInt(effectRuntime, inputId, (int)value.x);
				break;
			case PixelpartVariantValue.VariantType.Float:
				Plugin.PixelpartSetEffectInputFloat(effectRuntime, inputId, value.x);
				break;
			case PixelpartVariantValue.VariantType.Float2:
				Plugin.PixelpartSetEffectInputFloat2(effectRuntime, inputId, new Vector2(value.x, value.y));
				break;
			case PixelpartVariantValue.VariantType.Float3:
				Plugin.PixelpartSetEffectInputFloat3(effectRuntime, inputId, new Vector3(value.x, value.y, value.z));
				break;
			case PixelpartVariantValue.VariantType.Float4:
				Plugin.PixelpartSetEffectInputFloat4(effectRuntime, inputId, new Vector4(value.x, value.y, value.z, value.w));
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

		var effectRuntimePtr = IntPtr.Zero;

		try {
			effectRuntimePtr = EffectAsset.LoadEffect();
		}
		catch(InvalidOperationException e) {
			Debug.LogError("[Pixelpart] Failed to load effect asset: " + e.Message, this);
			return;
		}

		var inputCollection = new PixelpartEffectInputCollection(effectRuntimePtr);
		effectInputNames = new List<string>(inputCollection.InputNames);
		effectInputValues = new List<PixelpartVariantValue>(inputCollection.InputValues);

		var particleTypeCount = Plugin.PixelpartGetEffectParticleTypeCount(effectRuntimePtr);

		for(var particleTypeIndex = 0; particleTypeIndex < particleTypeCount; particleTypeIndex++) {
			var particleTypeId = Plugin.PixelpartFindParticleTypeByIndex(effectRuntimePtr, particleTypeIndex);

			var particleTypeNameBuffer = new byte[256];
			var particleTypeNameBufferSize = Plugin.PixelpartParticleTypeGetName(effectRuntimePtr, particleTypeId, particleTypeNameBuffer, particleTypeNameBuffer.Length);
			var particleTypeName = Encoding.UTF8.GetString(particleTypeNameBuffer, 0, particleTypeNameBufferSize);

			var rendererType = (ParticleRendererType)Plugin.PixelpartParticleTypeGetRenderer(effectRuntimePtr, particleTypeId);
			var needsInstancing = rendererType == ParticleRendererType.Mesh;

			var materialIdBuffer = new byte[256];
			var materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(effectRuntimePtr, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
			var materialId = Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
			var materialPath = string.Empty;

			if(Plugin.PixelpartParticleTypeIsMaterialBuiltIn(effectRuntimePtr, particleTypeId)) {
				if(PixelpartBuiltInMaterialProvider.Instance.BuiltInMaterials.TryGetValue(materialId, out PixelpartMaterialDescriptor builtInMaterial)) {
					materialPath = builtInMaterial.MaterialPath;
				}
			}
			else {
				var customMaterial =
					EffectAsset.CustomMaterials.FirstOrDefault(materialAsset =>
						materialAsset.ResourceId == materialId && materialAsset.Instancing == needsInstancing);

				materialPath = customMaterial?.MaterialPath ?? string.Empty;
			}

			var material = AssetDatabase.LoadAssetAtPath<Material>(materialPath);
			if(material == null) {
				Debug.LogWarning(
					"[Pixelpart] Failed to find material asset \"" + materialPath + "\" for material \"" + materialId + "\"", this);
			}

			ParticleTypeNames.Add(particleTypeName);
			ParticleMaterials.Add(material);
		}

		Plugin.PixelpartDeleteEffect(effectRuntimePtr);
	}

	public void OnDrawGizmos() {
		Gizmos.DrawIcon(transform.position, gizmoIconPath, true);
	}
#endif
}
}