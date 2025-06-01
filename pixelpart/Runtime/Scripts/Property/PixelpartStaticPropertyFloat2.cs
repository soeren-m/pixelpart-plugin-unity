using System;
using UnityEngine;

namespace Pixelpart
{
    public class PixelpartStaticPropertyFloat2
    {
        public Vector2 Value => Plugin.PixelpartStaticPropertyFloat2Value(internalProperty);

        public Vector2 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat2GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat2SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        public PixelpartStaticPropertyFloat2(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        [Obsolete("deprecated, use Value")]
        public Vector2 Get() => Value;
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector2 value) => BaseValue = value;
        [Obsolete("deprecated, use BaseValue")]
        public Vector2 GetValue() => BaseValue;
    }
}