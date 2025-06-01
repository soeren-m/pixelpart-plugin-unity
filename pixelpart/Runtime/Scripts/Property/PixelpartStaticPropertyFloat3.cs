using System;
using UnityEngine;

namespace Pixelpart
{
    public class PixelpartStaticPropertyFloat3
    {
        public Vector3 Value => Plugin.PixelpartStaticPropertyFloat3Value(internalProperty);

        public Vector3 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat3GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat3SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        public PixelpartStaticPropertyFloat3(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        [Obsolete("deprecated, use Value")]
        public Vector3 Get() => Value;
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector3 value) => BaseValue = value;
        [Obsolete("deprecated, use BaseValue")]
        public Vector3 GetValue() => BaseValue;
    }
}