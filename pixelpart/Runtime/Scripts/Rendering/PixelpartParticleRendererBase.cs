using System;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
public class PixelpartParticleRendererBase {
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

	protected readonly IntPtr internalEffect;
	protected readonly uint particleTypeIndex;
	protected readonly uint particleTypeId;
	protected readonly PixelpartGraphicsResourceProvider graphicsResourceProvider;

	protected readonly Material material;
	protected readonly string materialId;
	protected readonly PixelpartMaterialInfo materialInformation;

	public PixelpartParticleRendererBase(IntPtr effectPtr, uint ptypeIndex, Material baseMaterial, PixelpartMaterialInfo materialInfo, PixelpartGraphicsResourceProvider resourceProvider) {
		internalEffect = effectPtr;
		particleTypeIndex = ptypeIndex;
		particleTypeId = Plugin.PixelpartFindParticleTypeByIndex(internalEffect, (uint)particleTypeIndex);
		graphicsResourceProvider = resourceProvider;

		byte[] materialIdBuffer = new byte[256];
		int materialIdLength = Plugin.PixelpartParticleTypeGetMaterialId(internalEffect, particleTypeId, materialIdBuffer, materialIdBuffer.Length);
		materialId = System.Text.Encoding.UTF8.GetString(materialIdBuffer, 0, materialIdLength);

		materialInformation = materialInfo;
		material = new Material(baseMaterial);

		bool materialBuiltIn = Plugin.PixelpartParticleTypeIsMaterialBuiltIn(internalEffect, particleTypeId);

		if(!materialBuiltIn) {
			for(int samplerIndex = 0; samplerIndex < materialInformation.TextureResourceIds.Length; samplerIndex++) {
				string samplerName = materialInformation.SamplerNames[samplerIndex];
				string resourceId = materialInformation.TextureResourceIds[samplerIndex];

				Texture2D texture = null;
				if(graphicsResourceProvider.Textures.TryGetValue(resourceId, out texture)) {
					material.SetTexture(samplerName, texture);
				}
				else {
					Debug.LogError("[Pixelpart] Cannot find texture '" + resourceId + "'");
				}
			}
		}
	}

	protected void ApplyMaterialParameters() {
		uint particleEmitterId = Plugin.PixelpartParticleTypeGetParentId(internalEffect, particleTypeId);
		float effectTime = Plugin.PixelpartGetEffectTime(internalEffect);
		float objectTime = Plugin.PixelpartParticleEmitterGetLocalTime(internalEffect, particleEmitterId);

		material.SetFloat("_EffectTime", effectTime);
		material.SetFloat("_ObjectTime", objectTime);

		int numParameters = Plugin.PixelpartParticleTypeGetNumMaterialParameters(internalEffect, particleTypeId);

		uint[] parameterIds = new uint[numParameters];
		Plugin.PixelpartParticleTypeGetMaterialParameterIds(internalEffect, particleTypeId, parameterIds);

		for(int parameterIndex = 0; parameterIndex < numParameters; parameterIndex++) {
			uint parameterId = parameterIds[parameterIndex];
			string parameterName = materialInformation.GetParameterName(parameterId);
			if(parameterName == null) {
				continue;
			}

			MaterialParameterType parameterType = (MaterialParameterType)Plugin.PixelpartParticleTypeGetMaterialParameterType(internalEffect, particleTypeId, parameterId);

			switch(parameterType) {
				case MaterialParameterType.Int:
				case MaterialParameterType.Enum: {
					material.SetInt(parameterName, Plugin.PixelpartParticleTypeGetMaterialParameterValueInt(internalEffect, particleTypeId, parameterId));
					break;
				}

				case MaterialParameterType.Float: {
					material.SetFloat(parameterName, Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat(internalEffect, particleTypeId, parameterId));
					break;
				}

				case MaterialParameterType.Float2:
				case MaterialParameterType.Float3:
				case MaterialParameterType.Float4:
				case MaterialParameterType.Color: {
					material.SetVector(parameterName, Plugin.PixelpartParticleTypeGetMaterialParameterValueFloat4(internalEffect, particleTypeId, parameterId));
					break;
				}

				case MaterialParameterType.Bool: {
					material.SetInt(parameterName, Plugin.PixelpartParticleTypeGetMaterialParameterValueBool(internalEffect, particleTypeId, parameterId) ? 1 : 0);
					break;
				}

				case MaterialParameterType.ResourceImage: {
					byte[] resourceIdBuffer = new byte[256];
					int resourceIdLength = Plugin.PixelpartParticleTypeGetMaterialParameterValueResourceId(internalEffect, particleTypeId, parameterId, resourceIdBuffer, resourceIdBuffer.Length);
					string imageResourceId = System.Text.Encoding.UTF8.GetString(resourceIdBuffer, 0, resourceIdLength);

					Texture2D texture = null;
					if(graphicsResourceProvider.Textures.TryGetValue(imageResourceId, out texture)) {
						material.SetTexture(parameterName, texture);
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
}