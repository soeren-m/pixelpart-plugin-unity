using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using UnityEngine;

namespace Pixelpart
{
    internal class PixelpartBuiltInMaterialProvider
    {
        public static PixelpartBuiltInMaterialProvider Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new PixelpartBuiltInMaterialProvider();
                }

                return instance;
            }
        }
        private static PixelpartBuiltInMaterialProvider instance;

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsBiRP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsURP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsHDRP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        public PixelpartBuiltInMaterialProvider()
        {
            AddMaterial("SpriteUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit);
            AddMaterial("SpriteUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit);
            AddMaterial("TrailUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit);
            AddMaterial("TrailUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit);
            AddMaterial("MeshUnlit", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit);
            AddMaterial("MeshUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit);
            AddMaterial("SpriteLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit);
            AddMaterial("SpriteLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit);
            AddMaterial("TrailLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit);
            AddMaterial("TrailLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit);
            AddMaterial("MeshLit", PixelpartBlendMode.Off, PixelpartLightingMode.Lit);
            AddMaterial("MeshLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit);
        }

        public PixelpartMaterialDescriptor GetMaterial(string name)
        {
            switch (PixelpartRenderPipelineDetection.DetectRenderPipeline())
            {
                case PixelpartRenderPipelineType.BuiltIn:
                    return builtInMaterialsBiRP.GetValueOrDefault(name, null);
                case PixelpartRenderPipelineType.Universal:
                    return builtInMaterialsURP.GetValueOrDefault(name, null);
                case PixelpartRenderPipelineType.HighDefinition:
                    return builtInMaterialsHDRP.GetValueOrDefault(name, null);
                default:
                    throw new InvalidOperationException("Unknown render pipeline");
            }
        }

        private void AddMaterial(string name, PixelpartBlendMode blendMode, PixelpartLightingMode lightingMode)
        {
            var parameterCount = PixelpartPlugin.PixelpartGetBuiltInMaterialParameterCount(name);
            var parameterIds = new uint[parameterCount];
            var parameterNameBuffer = new byte[parameterCount * 256];
            var parameterNameBufferLength = PixelpartPlugin.PixelpartGetBuiltInMaterialParameters(name, parameterIds, parameterNameBuffer, parameterNameBuffer.Length);
            var parameterNames = Encoding.UTF8.GetString(parameterNameBuffer, 0, parameterNameBufferLength)
                .Split(new[] { '|' }, StringSplitOptions.RemoveEmptyEntries);

            var materialNameBiRP = "Pixelpart" + name;
            var materialNameURP = "Pixelpart" + name + "URP";
            var materialNameHDRP = "Pixelpart" + name + "HDRP";

            builtInMaterialsBiRP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                "Packages/net.pixelpart.core/Runtime/Materials/" + materialNameBiRP + ".mat", materialNameBiRP,
                blendMode, lightingMode,
                parameterIds, parameterNames);
            builtInMaterialsURP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                "Packages/net.pixelpart.urp/Runtime/Materials/" + materialNameURP + ".mat", materialNameURP,
                blendMode, lightingMode,
                parameterIds, parameterNames);
            builtInMaterialsHDRP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                "Packages/net.pixelpart.hdrp/Runtime/Materials/" + materialNameHDRP + ".mat", materialNameHDRP,
                blendMode, lightingMode,
                parameterIds, parameterNames);
        }
    }
}
