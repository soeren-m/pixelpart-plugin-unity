using System;
using System.IO;
using System.Text;

namespace Pixelpart {
[System.Serializable]
public class PixelpartCustomMaterialAsset {
	public string Name;
	public string ShaderAssetName;
	public string[] TextureIds;

	public PixelpartCustomMaterialAsset(string name, string shaderAssetName, string[] textureIds) {
		Name = name;
		ShaderAssetName = shaderAssetName;
		TextureIds = textureIds;
	}
}
}