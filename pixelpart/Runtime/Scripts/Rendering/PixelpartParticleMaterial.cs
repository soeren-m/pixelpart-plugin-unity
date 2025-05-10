using System;
using UnityEngine;

namespace Pixelpart {
internal class PixelpartParticleMaterial {
	public enum MaterialParameterType {
		Int = 0,
		Float = 1,
		Float2 = 2,
		Float3 = 3,
		Float4 = 4,
		Bool = 5,
		Enum = 6,
		Color = 7,
		Curve = 8,
		Gradient = 9,
		ResourceImage = 10
	}

	public Material Material { get; }

	public string MaterialId { get; }

	private readonly IntPtr internalEffect;

	private readonly uint particleEmitterId;

	private readonly uint particleTypeId;

	private readonly PixelpartGraphicsResourceProvider graphicsResourceProvider;

	private readonly PixelpartMaterialInfo materialInformation;

	public PixelpartParticleMaterial(IntPtr effectRuntimePtr, uint emitterId, uint typeId, Material baseMaterial, PixelpartMaterialInfo materialInfo, PixelpartGraphicsResourceProvider resourceProvider) {
		internalEffect = effectRuntimePtr;
		particleEmitterId = emitterId;
		particleTypeId = typeId;
		graphicsResourceProvider = resourceProvider;

		var materialIdBuffer = new byte[256];
		var materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(internalEffect, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
		MaterialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);

		materialInformation = materialInfo;
		Material = new Material(baseMaterial);

		var builtIn = Plugin.PixelpartParticleTypeIsMaterialBuiltIn(internalEffect, particleTypeId);

		if(!builtIn) {
			for(var samplerIndex = 0; samplerIndex < materialInformation.TextureResourceIds.Length; samplerIndex++) {
				var samplerName = materialInformation.SamplerNames[samplerIndex];
				var resourceId = materialInformation.TextureResourceIds[samplerIndex];

				Texture2D texture = null;
				if(graphicsResourceProvider.Textures.TryGetValue(resourceId, out texture)) {
					Material.SetTexture(samplerName, texture);
				}
				else {
					Debug.LogError("[Pixelpart] Cannot find texture '" + resourceId + "'");
				}
			}
		}
	}

	public void ApplyParameters() {
		var effectTime = Plugin.PixelpartGetEffectTime(internalEffect);
		var objectTime = Plugin.PixelpartNodeGetLocalTime(internalEffect, particleEmitterId);

		Material.SetFloat("_EffectTime", effectTime);
		Material.SetFloat("_ObjectTime", objectTime);

		var parameterCount = Plugin.PixelpartParticleTypeGetMaterialParameterCount(internalEffect, particleTypeId);

		var parameterIds = new uint[parameterCount];
		Plugin.PixelpartParticleTypeGetMaterialParameterIds(internalEffect, particleTypeId, parameterIds);

		foreach(var parameterId in parameterIds) {
			ApplyParameter(parameterId);
		}
	}

	private void ApplyParameter(uint parameterId) {
		var parameterName = materialInformation.GetParameterName(parameterId);
		if(parameterName == null) {
			return;
		}

		var parameterType = (MaterialParameterType)Plugin.PixelpartParticleTypeGetMaterialParameterType(internalEffect, particleTypeId, parameterId);

		switch(parameterType) {
			case MaterialParameterType.Int:
			case MaterialParameterType.Enum: {
				Material.SetInt(parameterName,
					Plugin.PixelpartParticleTypeGetMaterialParameterValueInt(internalEffect, particleTypeId, parameterId));
				break;
			}

			case MaterialParameterType.Float: {
				Material.SetFloat(parameterName,
					Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat(internalEffect, particleTypeId, parameterId));
				break;
			}

			case MaterialParameterType.Float2:
			case MaterialParameterType.Float3:
			case MaterialParameterType.Float4:
			case MaterialParameterType.Color: {
				Material.SetVector(parameterName,
					Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat4(internalEffect, particleTypeId, parameterId));
				break;
			}

			case MaterialParameterType.Bool: {
				Material.SetInt(parameterName,
					Plugin.PixelpartParticleTypeGetMaterialParameterValueBool(internalEffect, particleTypeId, parameterId) ? 1 : 0);
				break;
			}

			case MaterialParameterType.ResourceImage: {
				var resourceIdBuffer = new byte[256];
				var resourceIdLength = Plugin.PixelpartParticleTypeGetMaterialParameterValueResourceId(internalEffect, particleTypeId, parameterId, resourceIdBuffer, resourceIdBuffer.Length);
				var imageResourceId = System.Text.Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

				if(graphicsResourceProvider.Textures.TryGetValue(imageResourceId, out Texture2D texture)) {
					Material.SetTexture(parameterName, texture);
				}
				else {
					Debug.LogError("[Pixelpart] Cannot find texture '" + imageResourceId + "'");
				}

				break;
			}

			default: {
				break;
			}
		}
	}
}
}