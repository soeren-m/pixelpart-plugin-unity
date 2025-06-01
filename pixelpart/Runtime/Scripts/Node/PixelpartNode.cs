using System;
using System.Text;

namespace Pixelpart
{
    public abstract class PixelpartNode
    {
        protected readonly IntPtr effectRuntime;

        public uint Id { get; }

        public uint ParentId => Plugin.PixelpartNodeGetParentId(effectRuntime, Id);

        public string Name
        {
            get
            {
                var buffer = new byte[256];
                var size = Plugin.PixelpartNodeGetName(effectRuntime, Id, buffer, buffer.Length);

                return Encoding.UTF8.GetString(buffer, 0, size);
            }
        }

        public float LifetimeStart
        {
            get => Plugin.PixelpartNodeGetStart(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetStart(effectRuntime, Id, value);
        }

        public float LifetimeDuration
        {
            get => Plugin.PixelpartNodeGetDuration(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetDuration(effectRuntime, Id, value);
        }

        public bool Repeat
        {
            get => Plugin.PixelpartNodeIsRepeating(effectRuntime, Id);
            set => Plugin.PixelpartNodeSetRepeat(effectRuntime, Id, value);
        }

        public bool Active => Plugin.PixelpartNodeIsActive(effectRuntime, Id);

        public float LocalTime => Plugin.PixelpartNodeGetLocalTime(effectRuntime, Id);

        public PixelpartAnimatedPropertyFloat3 Position { get; }

        public PixelpartAnimatedPropertyFloat3 Rotation { get; }

        public PixelpartAnimatedPropertyFloat3 Scale { get; }

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