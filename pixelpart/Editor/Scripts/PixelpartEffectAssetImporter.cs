using UnityEngine;
using UnityEditor;
#if UNITY_2020_2_OR_NEWER
using UnityEditor.AssetImporters;
#else
using UnityEditor.Experimental.AssetImporters;
#endif

namespace Pixelpart {
[ScriptedImporter(1, "ppfx", AllowCaching = true)]
public class PixelpartEffectAssetImporter : ScriptedImporter {
	private const string assetIconPath = "Gizmos/PixelpartEffectIcon";

	public override void OnImportAsset(AssetImportContext ctx) {
		var asset = ScriptableObject.CreateInstance<PixelpartEffectAsset>();
		asset.Load(assetPath);

		var icon = Resources.Load<Texture2D>(assetIconPath);

		ctx.AddObjectToAsset(assetPath, asset, icon);
		ctx.SetMainObject(asset);
	}
}
}