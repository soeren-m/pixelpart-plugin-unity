using System;
using System.IO;
using System.Text;

namespace Pixelpart {
[System.Serializable]
public class PixelpartParticleTypeAsset {
	public string Name;
	public string DefaultShaderName;
	public string[] TextureIds;

	public PixelpartParticleTypeAsset(string name, string defaultShaderName, string[] textureIds) {
		Name = name;
		DefaultShaderName = defaultShaderName;
		TextureIds = textureIds;
	}
}
}