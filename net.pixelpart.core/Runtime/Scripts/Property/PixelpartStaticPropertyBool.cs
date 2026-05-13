using System;

namespace Pixelpart
{
    /// <summary>
    /// <c>bool</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyBool
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public bool Value => PixelpartPlugin.PixelpartStaticPropertyBoolValue(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public bool BaseValue
        {
            get => PixelpartPlugin.PixelpartStaticPropertyBoolGetBaseValue(internalProperty);
            set => PixelpartPlugin.PixelpartStaticPropertyBoolSetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyBool"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyBool(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }
    }
}
