using System;

namespace Pixelpart
{
    /// <summary>
    /// <c>float</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyFloat
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public float Value => Plugin.PixelpartStaticPropertyFloatValue(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public float BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloatGetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloatSetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyFloat"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyFloat(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return value with effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="Value"/>.
        /// </summary>
        /// <returns>Value with effect inputs taken into account</returns>
        [Obsolete("deprecated, use Value")]
        public float Get() => Value;

        /// <summary>
        /// Set value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <param name="value">Value without effect inputs taken into account</param>
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(float value) => BaseValue = value;

        /// <summary>
        /// Return value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <returns>Value without effect inputs taken into account</returns>
        [Obsolete("deprecated, use BaseValue")]
        public float GetValue() => BaseValue;
    }
}
