using System.IO;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
public class PixelpartEffectAssetPostprocessor : AssetPostprocessor {
	static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromPaths) {
		foreach(string assetPath in importedAssets) {
			if(Path.GetExtension(assetPath) == ".ppfx") {
				PixelpartEffectAsset.CreateAsset(assetPath);
			}
		}
	}
}
}
#endif