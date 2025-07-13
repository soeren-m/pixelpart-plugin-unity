using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEditor;

namespace Pixelpart
{
    [InitializeOnLoad]
    internal static class PixelpartPluginSwitch
    {
#if UNITY_2023_2_OR_NEWER
        static readonly string activeWebGLPluginPath = "WebGL/3.1.38/libpixelpart.a";
#elif UNITY_2022_2_OR_NEWER
        static readonly string activeWebGLPluginPath = "WebGL/3.1.8/libpixelpart.a";
#else
        static readonly string activeWebGLPluginPath = "WebGL/2.0.19/libpixelpart.a";
#endif

        static PixelpartPluginSwitch()
        {
            var webglPluginPaths = new string[]
            {
                "Packages/net.pixelpart/Runtime/Plugins/WebGL/3.1.38/libpixelpart.a",
                "Packages/net.pixelpart/Runtime/Plugins/WebGL/3.1.8/libpixelpart.a",
                "Packages/net.pixelpart/Runtime/Plugins/WebGL/2.0.19/libpixelpart.a"
            };

            foreach (var pluginPath in webglPluginPaths)
            {
                var importer = PluginImporter.GetAtPath(pluginPath) as PluginImporter;
                if (importer == null)
                {
                    Debug.LogError($"[Pixelpart] Failed to find WebGL plugin at \"{pluginPath}\"");
                    continue;
                }

                importer.SetIncludeInBuildDelegate(IncludeWebGLPluginInBuild);
            }
        }

        static bool IncludeWebGLPluginInBuild(string path)
        {
            return path.Contains(activeWebGLPluginPath);
        }
    }
}