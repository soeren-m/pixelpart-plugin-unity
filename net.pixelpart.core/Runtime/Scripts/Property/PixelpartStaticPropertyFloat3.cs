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
        public Vector3 Value => PixelpartPlugin.PixelpartStaticPropertyFloat3Value(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public Vector3 BaseValue
        {
            get => PixelpartPlugin.PixelpartStaticPropertyFloat3GetBaseValue(internalProperty);
            set => PixelpartPlugin.PixelpartStaticPropertyFloat3SetBaseValue(internalProperty, value);
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
    }
}
