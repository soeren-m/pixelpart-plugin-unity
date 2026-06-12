using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

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

        private static readonly Dictionary<string, uint> builtInMaterialParameterIds = new Dictionary<string, uint>()
        {
            { "MainTexture", 0 },
            { "ColorBlendMode", 10 },
            { "Emission", 20 },
            { "Roughness", 21 },
            { "Metallic", 22 },
            { "SpriteSheetRowNumber", 30 },
            { "SpriteSheetColumnNumber", 31 },
            { "SpriteSheetOrigin", 32 },
            { "SpriteAnimationNumFrames", 33 },
            { "SpriteAnimationStartFrame", 34 },
            { "SpriteAnimationDuration", 35 },
            { "SpriteAnimationLoop", 36 },
            { "SoftParticles", 40 },
            { "SoftParticleTransition", 41 },
            { "DistanceFade", 42 },
            { "DistanceFadeTransition", 43 }
        };

        private static readonly List<string> spriteUnlitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "ColorBlendMode",
            "SpriteSheetRowNumber",
            "SpriteSheetColumnNumber",
            "SpriteSheetOrigin",
            "SpriteAnimationNumFrames",
            "SpriteAnimationStartFrame",
            "SpriteAnimationDuration",
            "SpriteAnimationLoop",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };
        private static readonly List<string> spriteLitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "Roughness",
            "Metallic",
            "ColorBlendMode",
            "SpriteSheetRowNumber",
            "SpriteSheetColumnNumber",
            "SpriteSheetOrigin",
            "SpriteAnimationNumFrames",
            "SpriteAnimationStartFrame",
            "SpriteAnimationDuration",
            "SpriteAnimationLoop",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };
        private static readonly List<string> trailUnlitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "ColorBlendMode",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };
        private static readonly List<string> trailLitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "Roughness",
            "Metallic",
            "ColorBlendMode",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };
        private static readonly List<string> meshUnlitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "ColorBlendMode"
        };
        private static readonly List<string> meshUnlitAlphaParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "ColorBlendMode",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };
        private static readonly List<string> meshLitParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "Roughness",
            "Metallic",
            "ColorBlendMode"
        };
        private static readonly List<string> meshLitAlphaParameterNames = new List<string>
        {
            "MainTexture",
            "Emission",
            "Roughness",
            "Metallic",
            "ColorBlendMode",
            "SoftParticles",
            "SoftParticleTransition",
            "DistanceFade",
            "DistanceFadeTransition"
        };

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsBiRP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsURP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterialsHDRP =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        public PixelpartBuiltInMaterialProvider()
        {
            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlit", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.BuiltIn, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLit", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.BuiltIn, meshLitAlphaParameterNames);

            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlitURP", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.Universal, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLitURP", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.Universal, meshLitAlphaParameterNames);

            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlitHDRP", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, PixelpartRenderPipelineType.HighDefinition, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLitHDRP", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, PixelpartRenderPipelineType.HighDefinition, meshLitAlphaParameterNames);
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

        private void AddMaterial(string name, string materialName,
            PixelpartBlendMode blendMode, PixelpartLightingMode lightingMode,
            PixelpartRenderPipelineType renderPipeline, IList<string> parameterList)
        {
            var parameterIds = new uint[parameterList.Count];
            var parameterNames = new string[parameterList.Count];

            var index = 0;
            foreach (var parameterName in parameterList)
            {
                var parameterId = builtInMaterialParameterIds[parameterName];

                parameterIds[index] = parameterId;
                parameterNames[index] = "_" + parameterName;
                index++;
            }

            switch (renderPipeline)
            {
                case PixelpartRenderPipelineType.BuiltIn:
                    builtInMaterialsBiRP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                        "Packages/net.pixelpart.core/Runtime/Materials/" + materialName + ".mat", materialName,
                        blendMode, lightingMode,
                        parameterIds, parameterNames);
                    break;
                case PixelpartRenderPipelineType.Universal:
                    builtInMaterialsURP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                        "Packages/net.pixelpart.urp/Runtime/Materials/" + materialName + ".mat", materialName,
                        blendMode, lightingMode,
                        parameterIds, parameterNames);
                    break;
                case PixelpartRenderPipelineType.HighDefinition:
                    builtInMaterialsHDRP[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                        "Packages/net.pixelpart.hdrp/Runtime/Materials/" + materialName + ".mat", materialName,
                        blendMode, lightingMode,
                        parameterIds, parameterNames);
                    break;
                default:
                    throw new InvalidOperationException("Unknown render pipeline");
            }
        }
    }
}
