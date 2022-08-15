using System.IO;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace pixelpart {
public class PixelpartAssetPostprocessor : AssetPostprocessor {
	static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromPaths) {
		foreach(string assetPath in importedAssets) {
			if(Path.GetExtension(assetPath) == ".ppfx" || Path.GetExtension(assetPath) == ".json") {
				PixelpartEffectAsset.CreateAsset(assetPath);
			}
		}
	}
}
}
#endif