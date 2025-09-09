using System;
using System.Text;

namespace Pixelpart
{
    /// <summary>
    /// An object in an effect like a particle emitter or a force field.
    /// </summary>
    /// <remarks>
    /// Nodes can be moved, rotated and scaled and move together with their parent node if present.
    /// </remarks>
    public abstract class PixelpartNode
    {
        protected readonly IntPtr effectRuntime;

        /// <summary>
        /// ID of the node.
        /// </summary>
        public uint Id { get; }

        /// <summary>
        /// ID of the node's parent.
        /// </summary>
        public uint ParentId => Plugin.PixelpartNodeGetParentId(effectRuntime, Id);

        /// <summary>
        /// Name of the node.
        /// </summary>
        public string Name
        {
            get
            {
                var buffer = new byte[256];
                var size = Plugin.PixelpartNodeGetName(effectRuntime, Id, buffer, buffer.Length);

                return Encoding.UTF8.GetString(buffer, 0, size);
            }
        }

        /// <summary>
        /// Time in seconds until the node becomes active.
        /// </summary>
        public float LifetimeStart
        {
            get => Plugin.PixelpartNodeGetStart(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetStart(effectRuntime, Id, value);
        }

        /// <summary>
        /// How long the node stays active in seconds.
        /// </summary>
        public float LifetimeDuration
        {
            get => Plugin.PixelpartNodeGetDuration(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetDuration(effectRuntime, Id, value);
        }

        /// <summary>
        /// Wether the node repeats its behavior after its lifetime is over.
        /// </summary>
        public bool Repeat
        {
            get => Plugin.PixelpartNodeIsRepeating(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetRepeat(effectRuntime, Id, value);
        }

        /// <summary>
        /// Whether the node is active at the current point in time.
        /// </summary>
        public bool Active => Plugin.PixelpartNodeIsActive(effectRuntime, Id);

        /// <summary>
        /// Time fraction the node has been active for, in range 0.0 (0%) to 1.0 (100%).
        /// </summary>
        public float LocalTime => Plugin.PixelpartNodeGetLocalTime(effectRuntime, Id);

        /// <summary>
        /// Position of the node.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 Position { get; }

        /// <summary>
        /// Rotation of the node in degrees.
        /// </summary>
        /// <remarks>
        /// For 3D effects, the rotation is composed of roll, yaw and pitch.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat3 Rotation { get; }

        /// <summary>
        /// Size of the node.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 Scale { get; }

        /// <summary>
        /// Construct <see cref="PixelpartNode"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartNode(IntPtr effectRuntimePtr, uint id)
        {
            effectRuntime = effectRuntimePtr;
            Id = id;

            Position = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartNodeGetPosition(effectRuntimePtr, id));
            Rotation = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartNodeGetRotation(effectRuntimePtr, id));
            Scale = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartNodeGetScale(effectRuntimePtr, id));
        }
    }
}
