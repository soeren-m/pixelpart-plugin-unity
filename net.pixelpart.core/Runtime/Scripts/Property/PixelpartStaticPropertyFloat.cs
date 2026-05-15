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
        public float Value => PixelpartPlugin.PixelpartStaticPropertyFloatValue(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public float BaseValue
        {
            get => PixelpartPlugin.PixelpartStaticPropertyFloatGetBaseValue(internalProperty);
            set => PixelpartPlugin.PixelpartStaticPropertyFloatSetBaseValue(internalProperty, value);
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
    }
}
