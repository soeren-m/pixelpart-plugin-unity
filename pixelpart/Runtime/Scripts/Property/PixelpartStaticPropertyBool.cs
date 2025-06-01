using System;

namespace Pixelpart
{
    public class PixelpartStaticPropertyBool
    {
        public bool Value => Plugin.PixelpartStaticPropertyBoolValue(internalProperty);

        public bool BaseValue
        {
            get => Plugin.PixelpartStaticPropertyBoolGetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyBoolSetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        public PixelpartStaticPropertyBool(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        [Obsolete("deprecated, use Value")]
        public bool Get() => Value;
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(bool value) => BaseValue = value;
        [Obsolete("deprecated, use BaseValue")]
        public bool GetValue() => BaseValue;
    }
}