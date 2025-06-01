using System;

namespace Pixelpart
{
    public class PixelpartStaticPropertyFloat
    {
        public float Value => Plugin.PixelpartStaticPropertyFloatValue(internalProperty);

        public float BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloatGetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloatSetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        public PixelpartStaticPropertyFloat(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        [Obsolete("deprecated, use Value")]
        public float Get() => Value;
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(float value) => BaseValue = value;
        [Obsolete("deprecated, use BaseValue")]
        public float GetValue() => BaseValue;
    }
}