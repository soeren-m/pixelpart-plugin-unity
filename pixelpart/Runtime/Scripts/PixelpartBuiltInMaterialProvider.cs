using System;
using System.Collections.Generic;
using UnityEngine;

namespace Pixelpart {
public class PixelpartBuiltInMaterialProvider {
	public static PixelpartBuiltInMaterialProvider Instance {
		get {
			if(instance == null) {
				instance = new PixelpartBuiltInMaterialProvider();
			}

			return instance;
		}
	}
	private static PixelpartBuiltInMaterialProvider instance;

#if PIXELPART_USE_URP
	private const string BuiltInMaterialPath = "Packages/net.pixelpart.urp/Runtime/Materials/";
#elif PIXELPART_USE_HDRP
	private const string BuiltInMaterialPath = "Packages/net.pixelpart.hdrp/Runtime/Materials/";
#else
	private const string BuiltInMaterialPath = "Packages/net.pixelpart/Runtime/Materials/";
#endif

	private static Dictionary<string, uint> builtInMaterialParameterIds = new Dictionary<string, uint>() {
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

	private Dictionary<string, PixelpartMaterialInfo> builtInMaterials = new Dictionary<string, PixelpartMaterialInfo>();

	public PixelpartBuiltInMaterialProvider() {
		List<string> spriteUnlitParameterNames = new List<string>{
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
		List<string> spriteLitParameterNames = new List<string>{
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
		List<string> trailUnlitParameterNames = new List<string>{
			"MainTexture",
			"Emission",
			"ColorBlendMode",
			"SoftParticles",
			"SoftParticleTransition",
			"DistanceFade",
			"DistanceFadeTransition"
		};
		List<string> trailLitParameterNames = new List<string>{
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
		List<string> meshUnlitParameterNames = new List<string>{
			"MainTexture",
			"Emission",
			"ColorBlendMode"
		};
		List<string> meshUnlitAlphaParameterNames = new List<string>{
			"MainTexture",
			"Emission",
			"ColorBlendMode",
			"SoftParticles",
			"SoftParticleTransition",
			"DistanceFade",
			"DistanceFadeTransition"
		};
		List<string> meshLitParameterNames = new List<string>{
			"MainTexture",
			"Emission",
			"Roughness",
			"Metallic",
			"ColorBlendMode"
		};
		List<string> meshLitAlphaParameterNames = new List<string>{
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

	#if PIXELPART_USE_URP
		AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaURP", BlendModeType.Normal, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveURP", BlendModeType.Additive, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaURP", BlendModeType.Normal, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveURP", BlendModeType.Additive, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("MeshUnlit", "PixelpartMeshUnlitURP", BlendModeType.Off, LightingModeType.Unlit, meshUnlitParameterNames);
		AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaURP", BlendModeType.Normal, LightingModeType.Unlit, meshUnlitAlphaParameterNames);
		AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaURP", BlendModeType.Normal, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveURP", BlendModeType.Additive, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaURP", BlendModeType.Normal, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveURP", BlendModeType.Additive, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("MeshLit", "PixelpartMeshLitURP", BlendModeType.Off, LightingModeType.Lit, meshLitParameterNames);
		AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaURP", BlendModeType.Normal, LightingModeType.Lit, meshLitAlphaParameterNames);
	#elif PIXELPART_USE_HDRP
		AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlphaHDRP", BlendModeType.Normal, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditiveHDRP", BlendModeType.Additive, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlphaHDRP", BlendModeType.Normal, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditiveHDRP", BlendModeType.Additive, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("MeshUnlit", "PixelpartMeshUnlitHDRP", BlendModeType.Off, LightingModeType.Unlit, meshUnlitParameterNames);
		AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlphaHDRP", BlendModeType.Normal, LightingModeType.Unlit, meshUnlitAlphaParameterNames);
		AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlphaHDRP", BlendModeType.Normal, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditiveHDRP", BlendModeType.Additive, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlphaHDRP", BlendModeType.Normal, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditiveHDRP", BlendModeType.Additive, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("MeshLit", "PixelpartMeshLitHDRP", BlendModeType.Off, LightingModeType.Lit, meshLitParameterNames);
		AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlphaHDRP", BlendModeType.Normal, LightingModeType.Lit, meshLitAlphaParameterNames);
	#else
		AddMaterial("SpriteUnlitAlpha", "PixelpartSpriteUnlitAlpha", BlendModeType.Normal, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("SpriteUnlitAdditive", "PixelpartSpriteUnlitAdditive", BlendModeType.Additive, LightingModeType.Unlit, spriteUnlitParameterNames);
		AddMaterial("TrailUnlitAlpha", "PixelpartTrailUnlitAlpha", BlendModeType.Normal, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("TrailUnlitAdditive", "PixelpartTrailUnlitAdditive", BlendModeType.Additive, LightingModeType.Unlit, trailUnlitParameterNames);
		AddMaterial("MeshUnlit", "PixelpartMeshUnlit", BlendModeType.Off, LightingModeType.Unlit, meshUnlitParameterNames);
		AddMaterial("MeshUnlitAlpha", "PixelpartMeshUnlitAlpha", BlendModeType.Normal, LightingModeType.Unlit, meshUnlitAlphaParameterNames);
		AddMaterial("SpriteLitAlpha", "PixelpartSpriteLitAlpha", BlendModeType.Normal, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("SpriteLitAdditive", "PixelpartSpriteLitAdditive", BlendModeType.Additive, LightingModeType.Lit, spriteLitParameterNames);
		AddMaterial("TrailLitAlpha", "PixelpartTrailLitAlpha", BlendModeType.Normal, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("TrailLitAdditive", "PixelpartTrailLitAdditive", BlendModeType.Additive, LightingModeType.Lit, trailLitParameterNames);
		AddMaterial("MeshLit", "PixelpartMeshLit", BlendModeType.Off, LightingModeType.Lit, meshLitParameterNames);
		AddMaterial("MeshLitAlpha", "PixelpartMeshLitAlpha", BlendModeType.Normal, LightingModeType.Lit, meshLitAlphaParameterNames);
	#endif
	}

	public string ResolveMaterialPath(string materialId) {
		if(!builtInMaterials.ContainsKey(materialId)) {
			return "";
		}

		return builtInMaterials[materialId].MaterialPath;
	}

	public PixelpartMaterialInfo GetMaterialInfo(string materialId) {
		if(builtInMaterials.ContainsKey(materialId)) {
			return builtInMaterials[materialId];
		}

		return null;
	}

	private void AddMaterial(string name, string materialName, BlendModeType blendMode, LightingModeType lightingMode, IList<string> parameterList) {
		uint[] parameterIds = new uint[parameterList.Count];
		string[] parameterNames = new string[parameterList.Count];

		int index = 0;
		foreach(string parameterName in parameterList) {
			uint parameterId = builtInMaterialParameterIds[parameterName];

			parameterIds[index] = parameterId;
			parameterNames[index] = "_" + parameterName;
			index++;
		}

		builtInMaterials[name] = new PixelpartMaterialInfo(
			BuiltInMaterialPath + materialName + ".mat",
			blendMode, lightingMode, parameterIds, parameterNames, new string[0], new string[0]);
	}
}
}