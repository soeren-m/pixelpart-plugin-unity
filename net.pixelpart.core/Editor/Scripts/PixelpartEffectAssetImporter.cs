using UnityEngine;
using UnityEditor;
using UnityEditor.AssetImporters;

namespace Pixelpart
{
    [ScriptedImporter(1, "ppfx", AllowCaching = true)]
    public class PixelpartEffectAssetImporter : ScriptedImporter
    {
        private const string assetIconPath = "Gizmos/PixelpartEffectIcon";

        public override void OnImportAsset(AssetImportContext ctx)
        {
            var asset = ScriptableObject.CreateInstance<PixelpartEffectAsset>();
            asset.Load(assetPath);

            var icon = Resources.Load<Texture2D>(assetIconPath);

            ctx.AddObjectToAsset(assetPath, asset, icon);
            ctx.SetMainObject(asset);
        }
    }
}
