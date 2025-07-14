using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// <c>Vector2</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyFloat2
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public Vector2 Value => Plugin.PixelpartStaticPropertyFloat2Value(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public Vector2 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat2GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat2SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyFloat2"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyFloat2(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return value with effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="Value"/>.
        /// </summary>
        /// <returns>Value with effect inputs taken into account</returns>
        [Obsolete("deprecated, use Value")]
        public Vector2 Get() => Value;

        /// <summary>
        /// Set value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <param name="value">Value without effect inputs taken into account</param>
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector2 value) => BaseValue = value;

        /// <summary>
        /// Return value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <returns>Value without effect inputs taken into account</returns>
        [Obsolete("deprecated, use BaseValue")]
        public Vector2 GetValue() => BaseValue;
    }
}