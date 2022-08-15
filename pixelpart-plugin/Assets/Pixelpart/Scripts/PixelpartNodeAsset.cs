using System;
using System.IO;
using System.Text;

namespace pixelpart {
[System.Serializable]
public class PixelpartNodeAsset {
	public string ShaderName;
	public string[] TextureIds;

	public PixelpartNodeAsset(string shaderName, string[] textureIds) {
		ShaderName = shaderName;
		TextureIds = textureIds;
	}
}
}