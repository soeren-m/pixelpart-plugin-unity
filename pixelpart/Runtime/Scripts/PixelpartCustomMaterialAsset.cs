using System;
using System.IO;
using System.Text;

namespace Pixelpart {
[System.Serializable]
public class PixelpartCustomMaterialAsset {
	public string ResourceId;

	public bool Instancing;

	public PixelpartMaterialInfo MaterialInfo;

	public PixelpartCustomMaterialAsset(string resourceId, bool instancing, PixelpartMaterialInfo materialInfo) {
		ResourceId = resourceId;
		Instancing = instancing;
		MaterialInfo = materialInfo;
	}
}
}