#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/effect/ImageResource.h"
#include "pixelpart-runtime/effect/MeshResource.h"
#include "pixelpart-runtime/effect/MaterialResource.h"
#include <cstdint>
#include <cstring>
#include <string>

extern "C" {
UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetImageResourceCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityUInt>(effectRuntime->effectAsset.effect().resources().images().size());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetImageResourceId(PixelpartEffectRuntime* effectRuntime, UnityUInt index, char* resourceIdBuffer, UnityInt bufferLength) {
	if(!effectRuntime || !resourceIdBuffer) {
		return 0;
	}

	uint32_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().images()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<UnityInt>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetImageResourceWidth(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<UnityInt>(image.width());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetImageResourceHeight(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<UnityInt>(image.height());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetImageResourceChannels(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<UnityInt>(image.channels());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetImageResourceColorSpace(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return static_cast<UnityInt>(pixelpart::ColorSpace::linear);
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<UnityInt>(image.colorSpace());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ColorSpace::linear);
	}
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetImageResourceDataSize(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<UnityUInt>(image.data().size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetImageResourceData(PixelpartEffectRuntime* effectRuntime, const char* resourceId, uint8_t* imageData) {
	if(!effectRuntime || !resourceId) {
		return;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		std::memcpy(imageData, image.data().data(), image.data().size());
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetMeshResourceCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityUInt>(effectRuntime->effectAsset.effect().resources().meshes().size());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMeshResourceId(PixelpartEffectRuntime* effectRuntime, UnityUInt index, char* resourceIdBuffer, UnityInt bufferLength) {
	if(!effectRuntime || !resourceIdBuffer) {
		return 0;
	}

	uint32_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().meshes()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<UnityInt>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMeshResourceIndexCount(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh =
			effectRuntime->effectAsset.effect().resources().meshes().at(std::string(resourceId));

		return static_cast<UnityInt>(mesh.faces().size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMeshResourceVertexCount(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh =
			effectRuntime->effectAsset.effect().resources().meshes().at(std::string(resourceId));

		return static_cast<UnityInt>(mesh.positions().size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetMeshResourceVertexData(PixelpartEffectRuntime* effectRuntime, const char* resourceId, UnityInt* triangles, UnityVector3* vertices, UnityVector3* normals, UnityVector2* uv) {
	if(!effectRuntime || !resourceId || !triangles || !vertices || !normals || !uv) {
		return;
	}

	try {
		const pixelpart::MeshResource& mesh =
			effectRuntime->effectAsset.effect().resources().meshes().at(std::string(resourceId));

		const auto& meshFaces = mesh.faces();
		const auto& meshPositions = mesh.positions();
		const auto& meshNormals = mesh.normals();
		const auto& meshTextureCoords = mesh.textureCoords();

		for(std::size_t i = 0; i < meshFaces.size(); i += 3) {
			triangles[i + 0] = static_cast<UnityInt>(meshFaces[i + 0]);
			triangles[i + 1] = static_cast<UnityInt>(meshFaces[i + 2]);
			triangles[i + 2] = static_cast<UnityInt>(meshFaces[i + 1]);
		}

		for(std::size_t i = 0; i < meshPositions.size(); i++) {
			vertices[i] = UnityVector3{
				meshPositions[i].x,
				meshPositions[i].y,
				meshPositions[i].z
			};
		}

		for(std::size_t i = 0; i < meshNormals.size(); i++) {
			normals[i] = UnityVector3{
				meshNormals[i].x,
				meshNormals[i].y,
				meshNormals[i].z
			};
		}

		for(std::size_t i = 0; i < meshTextureCoords.size(); i++) {
			uv[i] = UnityVector2{
				meshTextureCoords[i].x,
				meshTextureCoords[i].y
			};
		}
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetMaterialResourceCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityUInt>(effectRuntime->effectAsset.effect().resources().materials().size());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMaterialResourceId(PixelpartEffectRuntime* effectRuntime, UnityUInt index, char* resourceIdBuffer, UnityInt bufferLength) {
	if(!effectRuntime || !resourceIdBuffer) {
		return 0;
	}

	uint32_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().materials()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<UnityInt>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMaterialResourceBlendMode(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource =
			effectRuntime->effectAsset.effect().resources().materials().at(std::string(resourceId));

		return static_cast<UnityInt>(materialResource.blendMode());
	}
	catch(...) {
		return -1;
	}
}


UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetMaterialResourceLightingMode(PixelpartEffectRuntime* effectRuntime, const char* resourceId) {
	if(!effectRuntime || !resourceId) {
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource =
			effectRuntime->effectAsset.effect().resources().materials().at(std::string(resourceId));

		return static_cast<UnityInt>(materialResource.lightingMode());
	}
	catch(...) {
		return -1;
	}
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartBuildMaterialShader(PixelpartEffectRuntime* effectRuntime, const char* resourceId, UnityInt renderPipeline,
	char* bufferMainCode, char* bufferParameterCode, char* bufferParameterNames, uint32_t* bufferParameterIds, char* bufferTextureResourceIds, char* bufferSamplerNames,
	UnityInt* lengthMainCode, UnityInt* lengthParameterCode, UnityInt* lengthParameterNames, UnityInt* lengthParameterIds, UnityInt* lengthTextureResourceIds, UnityInt* lengthSamplerNames,
	UnityInt bufferSizeMainCode, UnityInt bufferSizeParameterCode, UnityInt bufferSizeParameterNames, UnityInt bufferSizeParameterIds, UnityInt bufferSizeTextureResourceIds, UnityInt bufferSizeSamplerNames) {
	if(!effectRuntime || !resourceId ||
		!bufferMainCode || !bufferParameterCode || !bufferParameterNames || !bufferParameterIds || !bufferTextureResourceIds || !bufferSamplerNames ||
		!lengthMainCode || !lengthParameterCode || !lengthParameterNames || !lengthParameterIds || !lengthTextureResourceIds || !lengthSamplerNames ||
		bufferSizeMainCode < 2 || bufferSizeParameterCode < 2 || bufferSizeParameterNames < 2 || bufferSizeParameterIds < 2 || bufferSizeTextureResourceIds < 2 || bufferSizeSamplerNames < 2) {
		return false;
	}

	std::string materialId = std::string(resourceId);
	if(effectRuntime->effectAsset.effect().resources().materials().count(materialId) == 0u) {
		return false;
	}

	const pixelpart::MaterialResource& materialResource =
		effectRuntime->effectAsset.effect().resources().materials().at(materialId);

	try {
		switch(renderPipeline) {
			case 1:
				pixelpart::ShaderGraph::graphLanguage = PixelpartEffectRuntime::shaderLanguageURP;
				break;
			case 2:
				pixelpart::ShaderGraph::graphLanguage = PixelpartEffectRuntime::shaderLanguageHDRP;
				break;
			default:
				pixelpart::ShaderGraph::graphLanguage = PixelpartEffectRuntime::shaderLanguage;
				break;
		}

		pixelpart::ShaderGraph::BuildResult buildResult;
		materialResource.shaderGraph().build(buildResult);

		UnityInt parameterIndex = 0;
		std::string parameterNamesString;
		for(const auto& parameterEntry : buildResult.parameterNames) {
			if(parameterIndex >= bufferSizeParameterIds) {
				break;
			}

			bufferParameterIds[parameterIndex++] = static_cast<uint32_t>(parameterEntry.first);
			parameterNamesString += parameterEntry.second;
			parameterNamesString += ' ';
		}

		if(!parameterNamesString.empty()) {
			parameterNamesString.pop_back();
		}

		*lengthParameterIds = std::min(bufferSizeParameterIds, static_cast<UnityInt>(buildResult.parameterNames.size()));

		std::string textureResourceIdsString;
		std::string samplerNamesString;

		for(std::size_t samplerIndex = 0; samplerIndex < buildResult.textureResourceIds.size(); samplerIndex++) {
			textureResourceIdsString += buildResult.textureResourceIds[samplerIndex];
			textureResourceIdsString += ' ';

			samplerNamesString += pixelpart::ShaderGraph::graphLanguage.textureSamplers[samplerIndex];
			samplerNamesString += ' ';
		}

		if(!textureResourceIdsString.empty()) {
			textureResourceIdsString.pop_back();
		}
		if(!samplerNamesString.empty()) {
			samplerNamesString.pop_back();
		}

		*lengthMainCode = std::min(static_cast<UnityInt>(buildResult.mainCode.size()), bufferSizeMainCode - 1);
		std::memcpy(bufferMainCode, buildResult.mainCode.c_str(), *lengthMainCode);
		bufferMainCode[*lengthMainCode] = '\0';

		*lengthParameterCode = std::min(static_cast<UnityInt>(buildResult.parameterCode.size()), bufferSizeParameterCode - 1);
		std::memcpy(bufferParameterCode, buildResult.parameterCode.c_str(), *lengthParameterCode);
		bufferParameterCode[*lengthParameterCode] = '\0';

		*lengthParameterNames = std::min(static_cast<UnityInt>(parameterNamesString.size()), bufferSizeParameterNames - 1);
		std::memcpy(bufferParameterNames, parameterNamesString.c_str(), *lengthParameterNames);
		bufferParameterNames[*lengthParameterNames] = '\0';

		*lengthTextureResourceIds = std::min(static_cast<UnityInt>(textureResourceIdsString.size()), bufferSizeTextureResourceIds - 1);
		std::memcpy(bufferTextureResourceIds, textureResourceIdsString.c_str(), *lengthTextureResourceIds);
		bufferTextureResourceIds[*lengthTextureResourceIds] = '\0';

		*lengthSamplerNames = std::min(static_cast<UnityInt>(samplerNamesString.size()), bufferSizeSamplerNames - 1);
		std::memcpy(bufferSamplerNames, samplerNamesString.c_str(), *lengthSamplerNames);
		bufferSamplerNames[*lengthSamplerNames] = '\0';

		return true;
	}
	catch(const std::exception& e) {
		*lengthMainCode = std::min(static_cast<UnityInt>(std::strlen(e.what())), bufferSizeMainCode - 1);
		std::memcpy(bufferMainCode, e.what(), *lengthMainCode);
		bufferMainCode[*lengthMainCode] = '\0';

		return false;
	}
	catch(...) {
		return false;
	}

	return false;
}
}