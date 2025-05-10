using System.Runtime.InteropServices;

namespace Pixelpart {
[StructLayout(LayoutKind.Sequential, Pack = 8)]
internal struct PixelpartParticleRuntimeId {
	public uint EmitterId;

	public uint TypeId;

	public PixelpartParticleRuntimeId(uint emitterId, uint typeId) {
		EmitterId = emitterId;
		TypeId = typeId;
	}
}
}