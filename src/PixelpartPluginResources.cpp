#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceCount(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return static_cast<uint32_t>(internalEffect->project.effect.resources.images.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceId(PixelpartPluginEffect* internalEffect, uint32_t index, char* resourceIdBuffer, int32_t bufferLength) {
	if(!internalEffect || !resourceIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0u;
	for(const auto& entry : internalEffect->project.effect.resources.images) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			std::memcpy(resourceIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceWidth(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image = internalEffect->project.effect.resources.images.at(std::string(resourceId));

		return static_cast<int32_t>(image.width);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceHeight(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image = internalEffect->project.effect.resources.images.at(std::string(resourceId));

		return static_cast<int32_t>(image.height);
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceDataSize(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::ImageResource& image = internalEffect->project.effect.resources.images.at(std::string(resourceId));

		return static_cast<uint32_t>(image.data.size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetImageResourceData(PixelpartPluginEffect* internalEffect, const char* resourceId, unsigned char* imageData) {
	if(!internalEffect || !resourceId) {
		return;
	}

	try {
		const pixelpart::ImageResource& image = internalEffect->project.effect.resources.images.at(std::string(resourceId));

		std::memcpy(imageData, image.data.data(), image.data.size());
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetMeshResourceCount(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return static_cast<uint32_t>(internalEffect->project.effect.resources.meshes.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceId(PixelpartPluginEffect* internalEffect, uint32_t index, char* resourceIdBuffer, int32_t bufferLength) {
	if(!internalEffect || !resourceIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0u;
	for(const auto& entry : internalEffect->project.effect.resources.meshes) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			std::memcpy(resourceIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceIndexCount(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh = internalEffect->project.effect.resources.meshes.at(std::string(resourceId));

		return static_cast<int32_t>(mesh.faces.size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceVertexCount(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId) {
		return 0;
	}

	try {
		const pixelpart::MeshResource& mesh = internalEffect->project.effect.resources.meshes.at(std::string(resourceId));

		return static_cast<int32_t>(mesh.positions.size());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetMeshResourceVertexData(PixelpartPluginEffect* internalEffect, const char* resourceId, int32_t* triangles, Vector3* vertices, Vector3* normals, Vector2* uv) {
	if(!internalEffect || !resourceId) {
		return;
	}

	try {
		const pixelpart::MeshResource& mesh = internalEffect->project.effect.resources.meshes.at(std::string(resourceId));

		/*for(std::size_t i = 0u; i < mesh.faces.size(); i++) {
			triangles[i] = static_cast<int32_t>(mesh.faces[i]);
		}*/
		for(std::size_t i = 0u; i < mesh.faces.size(); i += 3u) {
			triangles[i + 0u] = static_cast<int32_t>(mesh.faces[i + 0u]);
			triangles[i + 1u] = static_cast<int32_t>(mesh.faces[i + 2u]);
			triangles[i + 2u] = static_cast<int32_t>(mesh.faces[i + 1u]);
		}

		for(std::size_t i = 0u; i < mesh.positions.size(); i++) {
			vertices[i] = Vector3{ mesh.positions[i].x, mesh.positions[i].y, mesh.positions[i].z };
			normals[i] = Vector3{ mesh.normals[i].x, mesh.normals[i].y, mesh.normals[i].z };
			uv[i] = Vector2{ mesh.textureCoords[i].x, mesh.textureCoords[i].y };
		}
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetMaterialResourceCount(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return static_cast<uint32_t>(internalEffect->project.effect.resources.materials.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMaterialResourceId(PixelpartPluginEffect* internalEffect, uint32_t index, char* resourceIdBuffer, int32_t bufferLength) {
	if(!internalEffect || !resourceIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0u;
	for(const auto& entry : internalEffect->project.effect.resources.materials) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			std::memcpy(resourceIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMaterialResourceBlendMode(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId || internalEffect->project.effect.resources.materials.count(std::string(resourceId)) == 0u) {
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource = internalEffect->project.effect.resources.materials.at(std::string(resourceId));

		return static_cast<int32_t>(materialResource.blendMode);
	}
	catch(...) {
		return -1;
	}
}


UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMaterialResourceLightingMode(PixelpartPluginEffect* internalEffect, const char* resourceId) {
	if(!internalEffect || !resourceId || internalEffect->project.effect.resources.materials.count(std::string(resourceId)) == 0u) {
		return -1;
	}

	try {
		const pixelpart::MaterialResource& materialResource = internalEffect->project.effect.resources.materials.at(std::string(resourceId));

		return static_cast<int32_t>(materialResource.lightingMode);
	}
	catch(...) {
		return -1;
	}
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartBuildMaterialShader(PixelpartPluginEffect* internalEffect, const char* resourceId, int32_t renderPipeline,
	char* bufferMainCode, char* bufferParameterCode, char* bufferParameterNames, uint32_t* bufferParameterIds, char* bufferTextureResourceIds, char* bufferSamplerNames,
	int32_t* lengthMainCode, int32_t* lengthParameterCode, int32_t* lengthParameterNames, int32_t* lengthParameterIds, int32_t* lengthTextureResourceIds, int32_t* lengthSamplerNames,
	int32_t bufferSizeMainCode, int32_t bufferSizeParameterCode, int32_t bufferSizeParameterNames, int32_t bufferSizeParameterIds, int32_t bufferSizeTextureResourceIds, int32_t bufferSizeSamplerNames) {
	if(!internalEffect || !resourceId ||
		!bufferMainCode || !bufferParameterCode || !bufferParameterNames || !bufferParameterIds || !bufferTextureResourceIds || !bufferSamplerNames ||
		!lengthMainCode || !lengthParameterCode || !lengthParameterNames || !lengthParameterIds || !lengthTextureResourceIds || !lengthSamplerNames ||
		bufferSizeMainCode < 2 || bufferSizeParameterCode < 2 || bufferSizeParameterNames < 2 || bufferSizeParameterIds < 2 || bufferSizeTextureResourceIds < 2 || bufferSizeSamplerNames < 2) {
		return false;
	}

	std::string materialId = std::string(resourceId);
	if(internalEffect->project.effect.resources.materials.count(materialId) == 0u) {
		return false;
	}

	const pixelpart::MaterialResource& materialResource = internalEffect->project.effect.resources.materials.at(materialId);

	try {
		switch(renderPipeline) {
			case 1:
				pixelpart::ShaderGraph::graphLanguage = PixelpartPluginEffect::shaderLanguageURP;
				break;
			case 2:
				pixelpart::ShaderGraph::graphLanguage = PixelpartPluginEffect::shaderLanguageHDRP;
				break;
			default:
				pixelpart::ShaderGraph::graphLanguage = PixelpartPluginEffect::shaderLanguage;
				break;
		}

		pixelpart::ShaderGraph::BuildResult buildResult;
		materialResource.shaderGraph.build(buildResult);

		int32_t parameterIndex = 0;
		std::string parameterNamesString;
		for(const std::pair<pixelpart::id_t, std::string>& parameterEntry : buildResult.parameterNames) {
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

		*lengthParameterIds = std::min(bufferSizeParameterIds, static_cast<int32_t>(buildResult.parameterNames.size()));

		std::string textureResourceIdsString;
		std::string samplerNamesString;

		for(std::size_t samplerIndex = 0u; samplerIndex < buildResult.textureResourceIds.size(); samplerIndex++) {
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

		*lengthMainCode = std::min(static_cast<int32_t>(buildResult.mainCode.size()), bufferSizeMainCode - 1);
		std::strncpy(bufferMainCode, buildResult.mainCode.c_str(), *lengthMainCode);
		bufferMainCode[*lengthMainCode] = '\0';

		*lengthParameterCode = std::min(static_cast<int32_t>(buildResult.parameterCode.size()), bufferSizeParameterCode - 1);
		std::strncpy(bufferParameterCode, buildResult.parameterCode.c_str(), *lengthParameterCode);
		bufferParameterCode[*lengthParameterCode] = '\0';

		*lengthParameterNames = std::min(static_cast<int32_t>(parameterNamesString.size()), bufferSizeParameterNames - 1);
		std::strncpy(bufferParameterNames, parameterNamesString.c_str(), *lengthParameterNames);
		bufferParameterNames[*lengthParameterNames] = '\0';

		*lengthTextureResourceIds = std::min(static_cast<int32_t>(textureResourceIdsString.size()), bufferSizeTextureResourceIds - 1);
		std::strncpy(bufferTextureResourceIds, textureResourceIdsString.c_str(), *lengthTextureResourceIds);
		bufferTextureResourceIds[*lengthTextureResourceIds] = '\0';

		*lengthSamplerNames = std::min(static_cast<int32_t>(samplerNamesString.size()), bufferSizeSamplerNames - 1);
		std::strncpy(bufferSamplerNames, samplerNamesString.c_str(), *lengthSamplerNames);
		bufferSamplerNames[*lengthSamplerNames] = '\0';

		return true;
	}
	catch(const std::exception& e) {
		*lengthMainCode = std::min(static_cast<int32_t>(std::strlen(e.what())), bufferSizeMainCode - 1);
		std::strncpy(bufferMainCode, e.what(), *lengthMainCode);
		bufferMainCode[*lengthMainCode] = '\0';

		return false;
	}

	return false;
}
}