using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// <c>Vector4</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyFloat4
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public Vector4 Value => Plugin.PixelpartStaticPropertyFloat4Value(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public Vector4 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat4GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat4SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyFloat4"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyFloat4(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return value with effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="Value"/>.
        /// </summary>
        /// <returns>Value with effect inputs taken into account</returns>
        [Obsolete("deprecated, use Value")]
        public Vector4 Get() => Value;

        /// <summary>
        /// Set value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <param name="value">Value without effect inputs taken into account</param>
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector4 value) => BaseValue = value;

        /// <summary>
        /// Return value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <returns>Value without effect inputs taken into account</returns>
        [Obsolete("deprecated, use BaseValue")]
        public Vector4 GetValue() => BaseValue;
    }
}