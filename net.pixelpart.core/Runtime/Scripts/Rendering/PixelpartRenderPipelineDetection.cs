using System;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart
{
    internal static class PixelpartRenderPipelineDetection
    {
        public static PixelpartRenderPipelineType DetectRenderPipeline()
        {
            var rpAsset = GraphicsSettings.currentRenderPipeline;
            if (rpAsset == null)
            {
                return PixelpartRenderPipelineType.BuiltIn;
            }

            var typeName = rpAsset.GetType().Name;
            if (typeName.Contains("Universal"))
            {
                return PixelpartRenderPipelineType.Universal;
            }
            else if (typeName.Contains("HD"))
            {
                return PixelpartRenderPipelineType.HighDefinition;
            }

            return PixelpartRenderPipelineType.BuiltIn;
        }
    }
}
