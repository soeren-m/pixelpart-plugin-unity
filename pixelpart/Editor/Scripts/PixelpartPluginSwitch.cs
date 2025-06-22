using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEditor;

[InitializeOnLoad]
internal static class PixelpartPluginSwitch
{
#if UNITY_2023_2_OR_NEWER
    static readonly string libraryPath = "WebGL/3.1.38/libpixelpart.a";
#elif UNITY_2022_2_OR_NEWER
    static readonly string libraryPath = "WebGL/3.1.8/libpixelpart.a";
#elif UNITY_2021_2_OR_NEWER
    static readonly string libraryPath = "WebGL/2.0.19/libpixelpart.a";
#else
    static readonly string libraryPath = "WebGL/libpixelpart.a";
#endif

    static readonly string tempFilePath = "Temp/PixelpartPluginSwitch";

    static PixelpartPluginSwitch()
    {
        if (System.IO.File.Exists(tempFilePath))
        {
            return;
        }

        var pluginImporters = PluginImporter.GetAllImporters()
            .Where(importer => importer.assetPath.Contains("WebGL") && importer.assetPath.Contains("libpixelpart.a"))
            .ToList();

        if (pluginImporters.Count > 0)
        {
            var result = new List<string>();
            foreach (var importer in pluginImporters)
            {
                var enabled = importer.assetPath.Contains(libraryPath);
                importer.SetCompatibleWithPlatform(BuildTarget.WebGL, enabled);

                result.Add(importer.assetPath + " " + enabled.ToString());
            }

            File.WriteAllLines(tempFilePath, result.ToArray());
        }
    }
}