using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// <c>Vector3</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyFloat3
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public Vector3 Value => Plugin.PixelpartStaticPropertyFloat3Value(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public Vector3 BaseValue
        {
            get => Plugin.PixelpartStaticPropertyFloat3GetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyFloat3SetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyFloat3"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyFloat3(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }

        /// <summary>
        /// Return value with effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="Value"/>.
        /// </summary>
        /// <returns>Value with effect inputs taken into account</returns>
        [Obsolete("deprecated, use Value")]
        public Vector3 Get() => Value;

        /// <summary>
        /// Set value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <param name="value">Value without effect inputs taken into account</param>
        [Obsolete("deprecated, use BaseValue")]
        public void SetValue(Vector3 value) => BaseValue = value;

        /// <summary>
        /// Return value without effect inputs taken into account.
        /// <b>Deprecated</b>, use <see cref="BaseValue"/>.
        /// </summary>
        /// <returns>Value without effect inputs taken into account</returns>
        [Obsolete("deprecated, use BaseValue")]
        public Vector3 GetValue() => BaseValue;
    }
}