using System;

namespace Pixelpart
{
    internal static class PixelpartArrayUtil
    {
        public static void EnsureMinSize<T>(ref T[] array, int minSize)
        {
            if (array.Length >= minSize)
            {
                return;
            }

            var newSize = array.Length == 0
                ? minSize
                : array.Length * 2;

            if (newSize < minSize)
            {
                newSize = minSize;
            }

            Array.Resize(ref array, newSize);
        }
    }
}
