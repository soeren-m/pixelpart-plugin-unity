using System;
using UnityEngine;

namespace Pixelpart
{
    public class PixelpartStaticPropertyFloat4
    {
        public Vector4 Value => Plugin.PixelpartStaticPropertyFloat4Value(internalProperty);

        public Vector4 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat4GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat4SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        public PixelpartStaticPropertyFloat4(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        [Obsolete("deprecated, use Value")]
        public Vector4 Get() => Value;
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector4 value) => BaseValue = value;
        [Obsolete("deprecated, use BaseValue")]
        public Vector4 GetValue() => BaseValue;
    }
}