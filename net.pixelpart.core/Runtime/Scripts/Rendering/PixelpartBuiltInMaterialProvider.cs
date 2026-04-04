using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Pixelpart
{
    internal class PixelpartBuiltInMaterialProvider
    {
#if PIXELPART_USE_URP
        private const string builtInMaterialPath = "Packages/net.pixelpart.urp/Runtime/Materials/";
#elif PIXELPART_USE_HDRP
        private const string builtInMaterialPath = "Packages/net.pixelpart.hdrp/Runtime/Materials/";
#else
        private const string builtInMaterialPath = "Packages/net.pixelpart.core/Runtime/Materials/";
#endif

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

        public IReadOnlyDictionary<string, PixelpartMaterialDescriptor> BuiltInMaterials =>
            new ReadOnlyDictionary<string, PixelpartMaterialDescriptor>(builtInMaterials);
        private readonly Dictionary<string, PixelpartMaterialDescriptor> builtInMaterials =
            new Dictionary<string, PixelpartMaterialDescriptor>();

        public PixelpartBuiltInMaterialProvider()
        {
#if PIXELPART_USE_URP
            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlitURP", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveURP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLitURP", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaURP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, meshLitAlphaParameterNames);
#elif PIXELPART_USE_HDRP
            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlitHDRP", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveHDRP", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLitHDRP", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaHDRP", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, meshLitAlphaParameterNames);
#else
            AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, spriteUnlitParameterNames);
            AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Unlit, trailUnlitParameterNames);
            AddMaterial("MeshUnlit", "PixelpartMeshUnlit", PixelpartBlendMode.Off, PixelpartLightingMode.Unlit, meshUnlitParameterNames);
            AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Unlit, meshUnlitAlphaParameterNames);
            AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, spriteLitParameterNames);
            AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditive", PixelpartBlendMode.Additive, PixelpartLightingMode.Lit, trailLitParameterNames);
            AddMaterial("MeshLit", "PixelpartMeshLit", PixelpartBlendMode.Off, PixelpartLightingMode.Lit, meshLitParameterNames);
            AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlpha", PixelpartBlendMode.Alpha, PixelpartLightingMode.Lit, meshLitAlphaParameterNames);
#endif
        }

        private void AddMaterial(string name, string materialName, PixelpartBlendMode blendMode, PixelpartLightingMode lightingMode, IList<string> parameterList)
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

            builtInMaterials[name] = PixelpartMaterialDescriptor.CreateDescriptorForBuiltInMaterial(
                builtInMaterialPath + materialName + ".mat", materialName,
                blendMode, lightingMode,
                parameterIds, parameterNames);
        }
    }
}
