using System.Runtime.InteropServices;

namespace Pixelpart
{
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    internal struct PixelpartParticleEmissionPair
    {
        public uint EmitterId;

        public uint TypeId;

        public PixelpartParticleEmissionPair(uint emitterId, uint typeId)
        {
            EmitterId = emitterId;
            TypeId = typeId;
        }
    }
}
