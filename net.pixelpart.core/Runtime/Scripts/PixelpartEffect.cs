using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using UnityEngine.Serialization;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Pixelpart
{
    /// <summary>
    /// Component that plays a Pixelpart effect.
    /// </summary>
    /// <remarks>
    /// This class provides methods and properties to change how the effect is simulated and rendered.
    /// </remarks>
    [AddComponentMenu("Pixelpart/Pixelpart Effect")]
    [Icon("Packages/net.pixelpart.core/Editor/Resources/Icons/PixelpartEffectIcon.png")]
    [ExecuteAlways]
    public class PixelpartEffect : MonoBehaviour
    {
#if UNITY_EDITOR
        private const string gizmoIconPath = "Packages/net.pixelpart.core/Editor/Resources/Gizmos/PixelpartEffectGizmo.png";
#endif

        public const uint NullId = 0xFFFFFFFF;

        /// <summary>
        /// Event that is invoked when the effect is finished.
        /// </summary>
        /// <remarks>
        /// This event is never invoked for effects with repeating particle emitters.
        /// </remarks>
        public event EventHandler Finished;

        /// <summary>
        /// Custom effect event.
        /// </summary>
        public event EventHandler<PixelpartEffectEventArgs> EffectEvent;

        /// <summary>
        /// Effect resource that is shown.
        /// </summary>
        public PixelpartEffectAsset EffectAsset
        {
            get => effectAsset;
            set => effectAsset = value;
        }
        [SerializeField]
        [FormerlySerializedAs("EffectAsset")]
        private PixelpartEffectAsset effectAsset = null;

        /// <summary>
        /// Whether the effect is currently playing or not.
        /// </summary>
        public bool Playing
        {
            get => playing;
            set => playing = value;
        }
        [SerializeField]
        [FormerlySerializedAs("Playing")]
        private bool playing = true;

        /// <summary>
        /// Whether the effect restarts automatically after time <c>LoopTime</c>.
        /// </summary>
        public bool Loop
        {
            get => loop;
            set => loop = value;
        }
        [SerializeField]
        [FormerlySerializedAs("Loop")]
        private bool loop = false;

        /// <summary>
        /// Time in seconds after which the effect loops.
        /// </summary>
        /// <remarks>
        /// Only effective if <c>Loop = true</c>.
        /// </remarks>
        public float LoopTime
        {
            get => loopTime;
            set => loopTime = value;
        }
        [SerializeField]
        [FormerlySerializedAs("LoopTime")]
        [Range(0.0f, 100.0f)]
        private float loopTime = 1.0f;

        /// <summary>
        /// Time in seconds the effect is pre-simulated before being rendered.
        /// </summary>
        /// <remarks>
        /// This value impacts performance and should be kept as low as possible.
        /// </remarks>
        public float WarmupTime
        {
            get => warmupTime;
            set => warmupTime = value;
        }
        [SerializeField]
        [FormerlySerializedAs("WarmupTime")]
        [Range(0.0f, 10.0f)]
        private float warmupTime = 0.0f;

        /// <summary>
        /// How fast the effect is being played.
        /// </summary>
        public float Speed
        {
            get => speed;
            set => speed = value;
        }
        [SerializeField]
        [FormerlySerializedAs("Speed")]
        [Range(0.0f, 10.0f)]
        private float speed = 1.0f;

        /// <summary>
        /// At which rate the effect is simulated, in frames per second.
        /// </summary>
        public float FrameRate
        {
            get => frameRate;
            set => frameRate = value;
        }
        [SerializeField]
        [FormerlySerializedAs("FrameRate")]
        [Range(1.0f, 100.0f)]
        private float frameRate = 60.0f;

        /// <summary>
        /// Seed used to initialize the effect simulation.
        /// </summary>
        /// <remarks>
        /// This seed is used if <see cref="RandomSeed"/> is not enabled.
        /// </remarks>
        public int Seed
        {
            get => seed;
            set => seed = value;
        }
        [SerializeField]
        [FormerlySerializedAs("Seed")]
        [Min(0)]
        private int seed = 0;

        /// <summary>
        /// Whether to use a random seed to initialize the effect simulation.
        /// </summary>
        public bool RandomSeed
        {
            get => randomSeed;
            set => randomSeed = value;
        }
        [SerializeField]
        [FormerlySerializedAs("RandomSeed")]
        private bool randomSeed = false;

        /// <summary>
        /// Multiplier for the size of the effect.
        /// </summary>
        /// <remarks>
        /// Adjust this value if the effect appears too small or too large in the scene.
        /// </remarks>
        public float EffectScale
        {
            get => effectScale;
            set => effectScale = value;
        }
        [SerializeField]
        [FormerlySerializedAs("EffectScale")]
        [Min(0.0f)]
        private float effectScale = 1.0f;

        /// <summary>
        /// Whether the effect is flipped horizontally.
        /// </summary>
        public bool FlipH
        {
            get => flipH;
            set => flipH = value;
        }
        [SerializeField]
        [FormerlySerializedAs("FlipH")]
        private bool flipH = false;

        /// <summary>
        /// Whether the effect is flipped vertically.
        /// </summary>
        public bool FlipV
        {
            get => flipV;
            set => flipV = value;
        }
        [SerializeField]
        [FormerlySerializedAs("FlipV")]
        private bool flipV = false;

        /// <summary>
        /// Time in seconds since the effect has started playing.
        /// </summary>
        public float CurrentTime => effectRuntime != IntPtr.Zero
            ? PixelpartPlugin.PixelpartGetEffectTime(effectRuntime) : 0.0f;

        /// <summary>
        /// Whether the effect is a 3D effect.
        /// </summary>
        public bool Is3D => effectRuntime != IntPtr.Zero
            ? PixelpartPlugin.PixelpartIsEffect3d(effectRuntime) : false;

        /// <summary>
        /// Names of particle types in the effect.
        /// </summary>
        public List<string> ParticleTypeNames => particleTypeNames;
        [SerializeField]
        [FormerlySerializedAs("ParticleTypeNames")]
        private List<string> particleTypeNames = new List<string>();

        /// <summary>
        /// Material of each particle type in <see cref="ParticleTypeNames"/>.
        /// </summary>
        public List<Material> ParticleMaterials => particleMaterials;
        [SerializeField]
        [FormerlySerializedAs("ParticleMaterials")]
        private List<Material> particleMaterials = new List<Material>();

        private Vector3 FinalEffectScale => new Vector3(
            Math.Max(EffectScale, 0.000001f) * (FlipH ? -1.0f : +1.0f),
            Math.Max(EffectScale, 0.000001f) * (FlipV ? -1.0f : +1.0f),
            Math.Max(EffectScale, 0.000001f));

        [SerializeField]
        private List<string> effectInputNames = new List<string>();

        [SerializeField]
        private List<PixelpartVariantValue> effectInputValues = new List<PixelpartVariantValue>();

        private IntPtr effectRuntime = IntPtr.Zero;

        private PixelpartEffectAsset cachedEffectAsset = null;

        private PixelpartEffectInputCollection effectInputCollection = new PixelpartEffectInputCollection();

        private PixelpartEffectTriggerCollection effectTriggerCollection = new PixelpartEffectTriggerCollection();

        private PixelpartEffectEventCollection effectEventCollection = new PixelpartEffectEventCollection();

        private PixelpartEffectRenderer effectRenderer = null;

        private bool finishedEventInvoked = false;

        private uint[] invokedEventIds = null;

        /// <summary>
        /// Construct <see cref="PixelpartEffect"/>.
        /// </summary>
        public PixelpartEffect()
        {

        }

        public void Awake()
        {
            if (!Application.IsPlaying(gameObject))
            {
                return;
            }

            InitEffect();
        }

        public void Update()
        {
            if (!Application.IsPlaying(gameObject))
            {
                return;
            }

            AdvanceEffect(Time.deltaTime);
        }

        public void LateUpdate()
        {
            if (!Application.IsPlaying(gameObject))
            {
                return;
            }

            UpdateEffectMesh(Camera.main);
            RenderEffect(null);
        }

        public void OnDestroy()
        {
            DeleteEffect();
        }

        public void AdvanceEffect(float dt)
        {
            if (EffectAsset != cachedEffectAsset)
            {
                InitEffect();
            }

            if (effectRuntime == IntPtr.Zero || !Playing || !gameObject.activeSelf)
            {
                return;
            }

            ApplyTransform();

            var timeStep = 1.0f / Math.Max(FrameRate, 0.01f);
            PixelpartPlugin.PixelpartAdvanceEffect(effectRuntime, dt,
                Loop, LoopTime, Speed,
                timeStep, Seed, RandomSeed);

            var invokedEventCount = PixelpartPlugin.PixelpartGetInvokedEffectEvents(effectRuntime, invokedEventIds);
            for (var eventIndex = 0; eventIndex < invokedEventCount; eventIndex++)
            {
                var eventId = invokedEventIds[eventIndex];
                if (!effectEventCollection.TryGetEventName(eventId, out var eventName))
                {
                    continue;
                }

                EffectEvent?.Invoke(this, new PixelpartEffectEventArgs
                {
                    EventId = eventId,
                    EventName = eventName
                });
            }

            if (!finishedEventInvoked && !Loop && PixelpartPlugin.PixelpartIsEffectFinished(effectRuntime))
            {
                finishedEventInvoked = true;
                Finished?.Invoke(this, EventArgs.Empty);
            }
        }

        public void UpdateEffectMesh(Camera camera)
        {
            effectRenderer?.UpdateMesh(camera, transform, FinalEffectScale);
        }

        public void RenderEffect(Camera camera)
        {
            effectRenderer?.Render(camera, transform, gameObject.layer);
        }

        /// <summary>
        /// Restart the effect and remove all existing particles.
        /// </summary>
        public void RestartEffect()
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return;
            }

            PixelpartPlugin.PixelpartRestartEffect(effectRuntime, true);
        }

        /// <summary>
        /// Restart the effect, but do not remove existing particles.
        /// </summary>
        public void ResetEffect()
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return;
            }

            PixelpartPlugin.PixelpartRestartEffect(effectRuntime, false);
        }

        /// <summary>
        /// Generate <paramref name="count"/> particles of the given type from the given emitter.
        /// </summary>
        /// <param name="particleEmitterName">Name of the particle emitter</param>
        /// <param name="particleTypeName">Name of the particle type</param>
        /// <param name="count">Number of particles to generate</param>
        public void SpawnParticles(string particleEmitterName, string particleTypeName, int count)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return;
            }

            var particleEmitterId = PixelpartPlugin.PixelpartFindNode(effectRuntime, particleEmitterName);
            if (particleEmitterId == NullId)
            {
                Debug.LogWarning("[Pixelpart] Unknown particle emitter \"" + particleEmitterName + "\"", this);
                return;
            }

            var particleTypeId = PixelpartPlugin.PixelpartFindParticleType(effectRuntime, particleTypeName);
            if (particleTypeId == NullId)
            {
                Debug.LogWarning("[Pixelpart] Unknown particle type \"" + particleTypeName + "\"", this);
                return;
            }

            PixelpartPlugin.PixelpartSpawnParticles(effectRuntime, particleEmitterId, particleTypeId, count);
        }

        #region Inputs

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>bool</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputBool(string inputName, bool value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputBool(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>int</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputInt(string inputName, int value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputInt(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>float</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputFloat(string inputName, float value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputFloat(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>Vector2</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputFloat2(string inputName, Vector2 value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputFloat2(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>Vector3</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputFloat3(string inputName, Vector3 value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputFloat3(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Set the effect input <paramref name="inputName"/> to the given value.
        /// The effect input must be of type <c>Vector4</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <param name="value">New value</param>
        public void SetInputFloat4(string inputName, Vector4 value)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return;
            }

            PixelpartPlugin.PixelpartSetEffectInputFloat4(effectRuntime, inputId, value);
            SetInputPropertyValue(inputName, new PixelpartVariantValue(value));
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>bool</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public bool GetInputBool(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return false;
            }

            return PixelpartPlugin.PixelpartGetEffectInputBool(effectRuntime, inputId);
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>int</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public int GetInputInt(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return 0;
            }

            return PixelpartPlugin.PixelpartGetEffectInputInt(effectRuntime, inputId);
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>float</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public float GetInputFloat(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return 0.0f;
            }

            return PixelpartPlugin.PixelpartGetEffectInputFloat(effectRuntime, inputId);
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>Vector2</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public Vector2 GetInputFloat2(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return new Vector2(0.0f, 0.0f);
            }

            return PixelpartPlugin.PixelpartGetEffectInputFloat2(effectRuntime, inputId);
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>Vector3</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public Vector3 GetInputFloat3(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return new Vector3(0.0f, 0.0f, 0.0f);
            }

            return PixelpartPlugin.PixelpartGetEffectInputFloat3(effectRuntime, inputId);
        }

        /// <summary>
        /// Return value of an effect input.
        /// The effect input must be of type <c>Vector4</c>.
        /// </summary>
        /// <param name="inputName">Name of the effect input</param>
        /// <returns>Value of the effect input</returns>
        public Vector4 GetInputFloat4(string inputName)
        {
            if (!effectInputCollection.TryGetInputId(inputName, out uint inputId))
            {
                return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            }

            return PixelpartPlugin.PixelpartGetEffectInputFloat4(effectRuntime, inputId);
        }

        public void ApplyInputProperties()
        {
            if (effectRuntime == IntPtr.Zero)
            {
                return;
            }

            for (var inputIndex = 0; inputIndex < effectInputNames.Count && inputIndex < effectInputValues.Count; inputIndex++)
            {
                ApplyInputProperty(inputIndex);
            }
        }

        #endregion

        #region Triggers

        /// <summary>
        /// Activate trigger <paramref name="triggerName"/>.
        /// </summary>
        /// <param name="triggerName">Name of the trigger</param>
        public void ActivateTrigger(string triggerName)
        {
            if (!effectTriggerCollection.TryGetTriggerId(triggerName, out uint triggerId))
            {
                return;
            }

            PixelpartPlugin.PixelpartActivateEffectTrigger(effectRuntime, triggerId);
        }

        /// <summary>
        /// Return whether trigger <paramref name="triggerName"/> was activated.
        /// </summary>
        /// <param name="triggerName">Name of the trigger</param>
        /// <returns><c>true</c> if the trigger was activated</returns>
        public bool IsTriggerActivated(string triggerName)
        {
            if (!effectTriggerCollection.TryGetTriggerId(triggerName, out uint triggerId))
            {
                return false;
            }

            return PixelpartPlugin.PixelpartIsEffectTriggerActivated(effectRuntime, triggerId);
        }

        #endregion

        #region Nodes

        /// <summary>
        /// Return the node with the given name.
        /// </summary>
        /// <param name="nodeName">Node name</param>
        /// <returns>Node or <c>null</c> if no node with this name exists</returns>
        public PixelpartNode FindNode(string nodeName)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            var id = PixelpartPlugin.PixelpartFindNode(effectRuntime, nodeName);
            if (!PixelpartPlugin.PixelpartNodeExists(effectRuntime, id))
            {
                return null;
            }

            var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
            if (node == null)
            {
                Debug.LogWarning("[Pixelpart] Failed to create node", this);
            }

            return node;
        }

        /// <summary>
        /// Return the node with the given ID.
        /// </summary>
        /// <param name="id">Node ID</param>
        /// <returns>Node or <c>null</c> if no node with this ID exists</returns>
        public PixelpartNode GetNode(uint id)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            if (!PixelpartPlugin.PixelpartNodeExists(effectRuntime, id))
            {
                return null;
            }

            var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
            if (node == null)
            {
                Debug.LogWarning("[Pixelpart] Failed to create node", this);
            }

            return node;
        }

        /// <summary>
        /// Return the node at the given index.
        /// </summary>
        /// <param name="index">Node index, starting from 0</param>
        /// <returns>Node or <c>null</c> if no node at this index exists</returns>
        public PixelpartNode GetNodeAtIndex(int index)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            if (index < 0)
            {
                Debug.LogWarning("[Pixelpart] Node index must be positive", this);
                return null;
            }

            var id = PixelpartPlugin.PixelpartFindNodeByIndex(effectRuntime, index);
            if (id == NullId)
            {
                return null;
            }

            var node = PixelpartNodeFactory.CreateNode(effectRuntime, id);
            if (node == null)
            {
                Debug.LogWarning("[Pixelpart] Failed to create node", this);
            }

            return node;
        }

        #endregion

        #region Particle types

        /// <summary>
        /// Return the particle type with the given name.
        /// </summary>
        /// <param name="particleTypeName">Particle type name</param>
        /// <returns>Particle type or <c>null</c> if no particle type with this name exists</returns>
        public PixelpartParticleType FindParticleType(string particleTypeName)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            var id = PixelpartPlugin.PixelpartFindParticleType(effectRuntime, particleTypeName);
            if (id == NullId)
            {
                return null;
            }

            return new PixelpartParticleType(effectRuntime, id);
        }

        /// <summary>
        /// Return the particle type with the given ID.
        /// </summary>
        /// <param name="id">Particle type ID</param>
        /// <returns>Particle type or <c>null</c> if no particle type with this ID exists</returns>
        public PixelpartParticleType GetParticleType(uint id)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            if (!PixelpartPlugin.PixelpartParticleTypeExists(effectRuntime, id))
            {
                return null;
            }

            return new PixelpartParticleType(effectRuntime, id);
        }

        /// <summary>
        /// Return the particle type at the given index.
        /// </summary>
        /// <param name="index">Particle type index</param>
        /// <returns>Particle type or <c>null</c> if no particle type at this index exists</returns>
        public PixelpartParticleType GetParticleTypeAtIndex(int index)
        {
            if (effectRuntime == IntPtr.Zero)
            {
                Debug.LogWarning("[Pixelpart] PixelpartEffect component is not associated with any effect asset", this);
                return null;
            }

            if (index < 0)
            {
                Debug.LogWarning("[Pixelpart] Particle type index must be positive", this);
                return null;
            }

            var id = PixelpartPlugin.PixelpartFindParticleTypeByIndex(effectRuntime, index);
            if (id == NullId)
            {
                return null;
            }

            return new PixelpartParticleType(effectRuntime, id);
        }

        #endregion

        private void InitEffect()
        {
            DeleteEffect();

            cachedEffectAsset = EffectAsset;
            if (EffectAsset == null)
            {
                return;
            }

            try
            {
                effectRuntime = EffectAsset.LoadEffect();
            }
            catch (InvalidOperationException e)
            {
                Debug.LogError("[Pixelpart] Failed to load effect asset: " + e.Message, this);
                return;
            }

            effectInputCollection = new PixelpartEffectInputCollection(effectRuntime, effectInputNames, effectInputValues);
            effectTriggerCollection = new PixelpartEffectTriggerCollection(effectRuntime);
            effectEventCollection = new PixelpartEffectEventCollection(effectRuntime);

            effectRenderer = new PixelpartEffectRenderer(effectRuntime, ParticleMaterials, EffectAsset.CustomMaterials);

            finishedEventInvoked = false;
            invokedEventIds = new uint[effectEventCollection.EventNames.Count];

            ApplyInputProperties();
            ApplyTransform();

            PixelpartPlugin.PixelpartReseedEffect(effectRuntime, RandomSeed
                ? (int)(Time.realtimeSinceStartupAsDouble * 1e6)
                : Seed);

            if (WarmupTime > 0.0f)
            {
                var timeStep = 1.0f / Math.Max(FrameRate, 0.01f);
                PixelpartPlugin.PixelpartAdvanceEffect(effectRuntime, WarmupTime,
                    false, 0.0f, 1.0f,
                    timeStep, Seed, RandomSeed);
            }
        }

        private void DeleteEffect()
        {
            if (effectRuntime == IntPtr.Zero)
            {
                return;
            }

            PixelpartPlugin.PixelpartDeleteEffect(effectRuntime);
            effectRuntime = IntPtr.Zero;

            effectInputCollection = new PixelpartEffectInputCollection();
            effectTriggerCollection = new PixelpartEffectTriggerCollection();
            effectEventCollection = new PixelpartEffectEventCollection();

            effectRenderer = null;

            finishedEventInvoked = false;
            invokedEventIds = null;
        }

        private void ApplyTransform()
        {
            PixelpartPlugin.PixelpartSetEffectTransform(effectRuntime, transform.localToWorldMatrix, FinalEffectScale);
        }

        private void SetInputPropertyValue(string inputName, PixelpartVariantValue value)
        {
            int inputIndex = effectInputNames.IndexOf(inputName);
            if (inputIndex < 0)
            {
                return;
            }

            effectInputValues[inputIndex] = value;
        }

        private void ApplyInputProperty(int index)
        {
            if (!effectInputCollection.TryGetInputId(effectInputNames[index], out uint inputId))
            {
                return;
            }

            PixelpartVariantValue value = effectInputValues[index];

            switch (value.type)
            {
                case PixelpartVariantValue.VariantType.Bool:
                    PixelpartPlugin.PixelpartSetEffectInputBool(effectRuntime, inputId, value.x > 0.5f);
                    break;
                case PixelpartVariantValue.VariantType.Int:
                    PixelpartPlugin.PixelpartSetEffectInputInt(effectRuntime, inputId, (int)value.x);
                    break;
                case PixelpartVariantValue.VariantType.Float:
                    PixelpartPlugin.PixelpartSetEffectInputFloat(effectRuntime, inputId, value.x);
                    break;
                case PixelpartVariantValue.VariantType.Float2:
                    PixelpartPlugin.PixelpartSetEffectInputFloat2(effectRuntime, inputId, new Vector2(value.x, value.y));
                    break;
                case PixelpartVariantValue.VariantType.Float3:
                    PixelpartPlugin.PixelpartSetEffectInputFloat3(effectRuntime, inputId, new Vector3(value.x, value.y, value.z));
                    break;
                case PixelpartVariantValue.VariantType.Float4:
                    PixelpartPlugin.PixelpartSetEffectInputFloat4(effectRuntime, inputId, new Vector4(value.x, value.y, value.z, value.w));
                    break;
                default:
                    break;
            }
        }

#if UNITY_EDITOR
        [SerializeField]
        [HideInInspector]
        private PixelpartEffectAsset prevEffectAsset = null;

        public void OnValidate()
        {
            if (EffectAsset == prevEffectAsset)
            {
                return;
            }

            prevEffectAsset = EffectAsset;

            effectInputNames.Clear();
            effectInputValues.Clear();
            particleTypeNames.Clear();
            particleMaterials.Clear();

            if (EffectAsset == null)
            {
                return;
            }

            var tempEffectRuntime = IntPtr.Zero;

            try
            {
                tempEffectRuntime = EffectAsset.LoadEffect();
            }
            catch (InvalidOperationException e)
            {
                Debug.LogError("[Pixelpart] Failed to load effect asset: " + e.Message, this);
                return;
            }

            try
            {
                var inputCollection = new PixelpartEffectInputCollection(tempEffectRuntime);
                effectInputNames = new List<string>(inputCollection.InputNames);
                effectInputValues = new List<PixelpartVariantValue>(inputCollection.InputValues);

                var particleTypeCount = PixelpartPlugin.PixelpartGetEffectParticleTypeCount(tempEffectRuntime);

                for (var particleTypeIndex = 0; particleTypeIndex < particleTypeCount; particleTypeIndex++)
                {
                    var particleTypeId = PixelpartPlugin.PixelpartFindParticleTypeByIndex(tempEffectRuntime, particleTypeIndex);

                    var particleTypeNameBuffer = new byte[256];
                    var particleTypeNameBufferSize = PixelpartPlugin.PixelpartParticleTypeGetName(tempEffectRuntime, particleTypeId, particleTypeNameBuffer, particleTypeNameBuffer.Length);
                    var particleTypeName = Encoding.UTF8.GetString(particleTypeNameBuffer, 0, particleTypeNameBufferSize);

                    var rendererType = (PixelpartParticleType.ParticleRendererType)PixelpartPlugin.PixelpartParticleTypeGetRenderer(tempEffectRuntime, particleTypeId);
                    var needsInstancing = rendererType == PixelpartParticleType.ParticleRendererType.Mesh;

                    var materialIdBuffer = new byte[256];
                    var materialIdLength = PixelpartPlugin.PixelpartParticleTypeGetMaterialId(tempEffectRuntime, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
                    var materialId = Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);
                    var materialPath = string.Empty;

                    if (PixelpartPlugin.PixelpartParticleTypeIsMaterialBuiltIn(tempEffectRuntime, particleTypeId))
                    {
                        var builtInMaterial = PixelpartBuiltInMaterialProvider.Instance.GetMaterial(materialId);
                        if (builtInMaterial != null)
                        {
                            materialPath = builtInMaterial.MaterialPath;
                        }
                    }
                    else
                    {
                        var customMaterial =
                            EffectAsset.CustomMaterials.FirstOrDefault(materialAsset =>
                                materialAsset.ResourceId == materialId && materialAsset.Instancing == needsInstancing);

                        materialPath = customMaterial?.MaterialPath ?? string.Empty;
                    }

                    var material = AssetDatabase.LoadAssetAtPath<Material>(materialPath);
                    if (material == null)
                    {
                        Debug.LogWarning(
                            "[Pixelpart] Failed to find material asset \"" + materialPath + "\" for material \"" + materialId + "\"", this);
                    }

                    particleTypeNames.Add(particleTypeName);
                    particleMaterials.Add(material);
                }
            }
            finally
            {
                PixelpartPlugin.PixelpartDeleteEffect(tempEffectRuntime);
            }
        }

        public void OnDrawGizmos()
        {
            Gizmos.DrawIcon(transform.position, gizmoIconPath, true);
        }
#endif
    }
}
