using System;

namespace Pixelpart
{
    /// <summary>
    /// <c>int</c> property affected by effect inputs.
    /// </summary>
    /// <remarks>
    /// Scene objects of an effect like particle emitters and force fields have properties that do not change over time,
    /// but can be affected by effect inputs. Such properties are represented by one of the <c>PixelpartStaticProperty</c> classes.
    /// </remarks>
    public class PixelpartStaticPropertyInt
    {
        /// <summary>
        /// Value with effect inputs taken into account.
        /// </summary>
        public int Value => Plugin.PixelpartStaticPropertyIntValue(internalProperty);

        /// <summary>
        /// Value without effect inputs taken into account.
        /// </summary>
        public int BaseValue
        {
            get => Plugin.PixelpartStaticPropertyIntGetBaseValue(internalProperty);
            set => Plugin.PixelpartStaticPropertyIntSetBaseValue(internalProperty, value);
        }

        private readonly IntPtr internalProperty;

        /// <summary>
        /// Construct <see cref="PixelpartStaticPropertyInt"/>.
        /// </summary>
        /// <param name="internalPropertyPtr">Internal property pointer</param>
        public PixelpartStaticPropertyInt(IntPtr internalPropertyPtr)
        {
            internalProperty = internalPropertyPtr;
        }
    }
}
