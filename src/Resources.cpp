#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/effect/ImageResource.h"
#include "pixelpart-runtime/effect/MeshResource.h"
#include "pixelpart-runtime/effect/MaterialResource.h"
#include <cstring>
#include <string>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().resources().images().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t index, pixelpart_unity::char_t* resourceIdBuffer, pixelpart_unity::int_t bufferLength) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceIdBuffer) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	pixelpart_unity::int_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().images()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<pixelpart_unity::int_t>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceWidth(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(image.width());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceHeight(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(image.height());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceChannels(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(image.channels());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetImageResourceColorSpace(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ColorSpace::linear);
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ColorSpace::linear);
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(image.colorSpace());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ColorSpace::linear);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartGetImageResourceDataSize(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		return static_cast<pixelpart_unity::uint_t>(image.data().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetImageResourceData(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId, uint8_t* imageData) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		const pixelpart::ImageResource& image =
			effectRuntime->effectAsset.effect().resources().images().at(std::string(resourceId));

		std::memcpy(imageData, image.data().data(), image.data().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMeshResourceCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().resources().meshes().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMeshResourceId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t index, pixelpart_unity::char_t* resourceIdBuffer, pixelpart_unity::int_t bufferLength) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceIdBuffer) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	pixelpart_unity::int_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().meshes()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<pixelpart_unity::int_t>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMeshResourceIndexCount(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh =
			effectRuntime->effectAsset.effect().resources().meshes().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(mesh.faces().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMeshResourceVertexCount(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh =
			effectRuntime->effectAsset.effect().resources().meshes().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(mesh.positions().size());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetMeshResourceVertexData(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId, pixelpart_unity::int_t* triangles, pixelpart_unity::vector3_t* vertices, pixelpart_unity::vector3_t* normals, pixelpart_unity::vector2_t* uv) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(!resourceId || !triangles || !vertices || !normals || !uv) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
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
			triangles[i + 0] = static_cast<pixelpart_unity::int_t>(meshFaces[i + 0]);
			triangles[i + 1] = static_cast<pixelpart_unity::int_t>(meshFaces[i + 2]);
			triangles[i + 2] = static_cast<pixelpart_unity::int_t>(meshFaces[i + 1]);
		}

		for(std::size_t i = 0; i < meshPositions.size(); i++) {
			vertices[i] = pixelpart_unity::vector3_t{
				meshPositions[i].x,
				meshPositions[i].y,
				meshPositions[i].z
			};
		}

		for(std::size_t i = 0; i < meshNormals.size(); i++) {
			normals[i] = pixelpart_unity::vector3_t{
				meshNormals[i].x,
				meshNormals[i].y,
				meshNormals[i].z
			};
		}

		for(std::size_t i = 0; i < meshTextureCoords.size(); i++) {
			uv[i] = pixelpart_unity::vector2_t{
				meshTextureCoords[i].x,
				meshTextureCoords[i].y
			};
		}
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMaterialResourceCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().resources().materials().size());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMaterialResourceId(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t index, pixelpart_unity::char_t* resourceIdBuffer, pixelpart_unity::int_t bufferLength) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}
	else if(!resourceIdBuffer) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return 0;
	}

	pixelpart_unity::int_t currentIndex = 0;
	for(const auto& entry : effectRuntime->effectAsset.effect().resources().materials()) {
		if(currentIndex == index) {
			if(entry.first.size() > static_cast<std::size_t>(bufferLength)) {
				return 0;
			}

			std::memcpy(resourceIdBuffer, entry.first.c_str(), entry.first.size());

			return static_cast<pixelpart_unity::int_t>(entry.first.size());
		}

		currentIndex++;
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMaterialResourceBlendMode(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return -1;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource =
			effectRuntime->effectAsset.effect().resources().materials().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(materialResource.blendMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return -1;
}


UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetMaterialResourceLightingMode(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return -1;
	}
	else if(!resourceId) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource =
			effectRuntime->effectAsset.effect().resources().materials().at(std::string(resourceId));

		return static_cast<pixelpart_unity::int_t>(materialResource.lightingMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return -1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartBuildMaterialShader(pixelpart_unity::EffectRuntime* effectRuntime, const pixelpart_unity::char_t* resourceId, pixelpart_unity::int_t renderPipeline,
	pixelpart_unity::char_t* bufferMainCode, pixelpart_unity::char_t* bufferParameterCode, pixelpart_unity::char_t* bufferParameterNames, pixelpart_unity::uint_t* bufferParameterIds, pixelpart_unity::char_t* bufferTextureResourceIds, pixelpart_unity::char_t* bufferSamplerNames,
	pixelpart_unity::int_t* lengthMainCode, pixelpart_unity::int_t* lengthParameterCode, pixelpart_unity::int_t* lengthParameterNames, pixelpart_unity::int_t* lengthParameterIds, pixelpart_unity::int_t* lengthTextureResourceIds, pixelpart_unity::int_t* lengthSamplerNames,
	pixelpart_unity::int_t bufferSizeMainCode, pixelpart_unity::int_t bufferSizeParameterCode, pixelpart_unity::int_t bufferSizeParameterNames, pixelpart_unity::int_t bufferSizeParameterIds, pixelpart_unity::int_t bufferSizeTextureResourceIds, pixelpart_unity::int_t bufferSizeSamplerNames) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}
	else if(!resourceId ||
		!bufferMainCode || !bufferParameterCode || !bufferParameterNames || !bufferParameterIds || !bufferTextureResourceIds || !bufferSamplerNames ||
		!lengthMainCode || !lengthParameterCode || !lengthParameterNames || !lengthParameterIds || !lengthTextureResourceIds || !lengthSamplerNames ||
		bufferSizeMainCode < 2 || bufferSizeParameterCode < 2 || bufferSizeParameterNames < 2 || bufferSizeParameterIds < 2 || bufferSizeTextureResourceIds < 2 || bufferSizeSamplerNames < 2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return false;
	}

	std::string materialId = std::string(resourceId);
	if(effectRuntime->effectAsset.effect().resources().materials().count(materialId) == 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return false;
	}

	const pixelpart::MaterialResource& materialResource =
		effectRuntime->effectAsset.effect().resources().materials().at(materialId);

	try {
		switch(renderPipeline) {
			case 1:
				pixelpart::ShaderGraph::graphLanguage = pixelpart_unity::shaderLanguageURP;
				break;
			case 2:
				pixelpart::ShaderGraph::graphLanguage = pixelpart_unity::shaderLanguageHDRP;
				break;
			default:
				pixelpart::ShaderGraph::graphLanguage = pixelpart_unity::shaderLanguage;
				break;
		}

		pixelpart::ShaderGraph::BuildResult buildResult;
		materialResource.shaderGraph().build(buildResult);

		pixelpart_unity::int_t parameterIndex = 0;
		std::string parameterNamesString;
		for(const auto& parameterEntry : buildResult.parameterNames) {
			if(parameterIndex >= bufferSizeParameterIds) {
				break;
			}

			bufferParameterIds[parameterIndex++] = parameterEntry.first.value();
			parameterNamesString += parameterEntry.second;
			parameterNamesString += ' ';
		}

		if(!parameterNamesString.empty()) {
			parameterNamesString.pop_back();
		}

		*lengthParameterIds = std::min(bufferSizeParameterIds, static_cast<pixelpart_unity::int_t>(buildResult.parameterNames.size()));

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

		*lengthMainCode = std::min(static_cast<pixelpart_unity::int_t>(buildResult.mainCode.size()), bufferSizeMainCode - 1);
		std::memcpy(bufferMainCode, buildResult.mainCode.c_str(), *lengthMainCode);
		bufferMainCode[*lengthMainCode] = '\0';

		*lengthParameterCode = std::min(static_cast<pixelpart_unity::int_t>(buildResult.parameterCode.size()), bufferSizeParameterCode - 1);
		std::memcpy(bufferParameterCode, buildResult.parameterCode.c_str(), *lengthParameterCode);
		bufferParameterCode[*lengthParameterCode] = '\0';

		*lengthParameterNames = std::min(static_cast<pixelpart_unity::int_t>(parameterNamesString.size()), bufferSizeParameterNames - 1);
		std::memcpy(bufferParameterNames, parameterNamesString.c_str(), *lengthParameterNames);
		bufferParameterNames[*lengthParameterNames] = '\0';

		*lengthTextureResourceIds = std::min(static_cast<pixelpart_unity::int_t>(textureResourceIdsString.size()), bufferSizeTextureResourceIds - 1);
		std::memcpy(bufferTextureResourceIds, textureResourceIdsString.c_str(), *lengthTextureResourceIds);
		bufferTextureResourceIds[*lengthTextureResourceIds] = '\0';

		*lengthSamplerNames = std::min(static_cast<pixelpart_unity::int_t>(samplerNamesString.size()), bufferSizeSamplerNames - 1);
		std::memcpy(bufferSamplerNames, samplerNamesString.c_str(), *lengthSamplerNames);
		bufferSamplerNames[*lengthSamplerNames] = '\0';

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}
}