#include "PixelpartPlugin.h"
#include "PixelpartPluginUtil.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceCount(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return static_cast<uint32_t>(nativeEffect->project.effect.resources.images.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceId(PixelpartNativeEffect* nativeEffect, uint32_t index, char* resourceIdBuffer, int32_t bufferLength) {
	if(!nativeEffect || !resourceIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0u;
	for(const auto& entry : nativeEffect->project.effect.resources.images) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			std::memcpy(resourceIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceWidth(PixelpartNativeEffect* nativeEffect, const char* resourceId) {
	if(!nativeEffect || !resourceId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->project.effect.resources.images.at(std::string(resourceId));

	return static_cast<int32_t>(image.width);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceHeight(PixelpartNativeEffect* nativeEffect, const char* resourceId) {
	if(!nativeEffect || !resourceId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->project.effect.resources.images.at(std::string(resourceId));

	return static_cast<int32_t>(image.height);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceDataSize(PixelpartNativeEffect* nativeEffect, const char* resourceId) {
	if(!nativeEffect || !resourceId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->project.effect.resources.images.at(std::string(resourceId));

	return static_cast<uint32_t>(image.data.size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetImageResourceData(PixelpartNativeEffect* nativeEffect, const char* resourceId, unsigned char* imageData) {
	if(!nativeEffect || !resourceId) {
		return;
	}

	const pixelpart::ImageResource& image = nativeEffect->project.effect.resources.images.at(std::string(resourceId));

	std::memcpy(imageData, image.data.data(), image.data.size());
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetMeshResourceCount(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return static_cast<uint32_t>(nativeEffect->project.effect.resources.meshes.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceId(PixelpartNativeEffect* nativeEffect, uint32_t index, char* resourceIdBuffer, int32_t bufferLength) {
	if(!nativeEffect || !resourceIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0u;
	for(const auto& entry : nativeEffect->project.effect.resources.meshes) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			std::memcpy(resourceIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceIndexCount(PixelpartNativeEffect* nativeEffect, const char* resourceId) {
	if(!nativeEffect || !resourceId) {
		return 0;
	}

	const pixelpart::MeshResource& mesh = nativeEffect->project.effect.resources.meshes.at(std::string(resourceId));

	return static_cast<int32_t>(mesh.faces.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetMeshResourceVertexCount(PixelpartNativeEffect* nativeEffect, const char* resourceId) {
	if(!nativeEffect || !resourceId) {
		return 0;
	}

	const pixelpart::MeshResource& mesh = nativeEffect->project.effect.resources.meshes.at(std::string(resourceId));

	return static_cast<int32_t>(mesh.positions.size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetMeshResourceVertexData(PixelpartNativeEffect* nativeEffect, const char* resourceId, int32_t* triangles, Vector3* vertices, Vector3* normals, Vector2* uv) {
	if(!nativeEffect || !resourceId) {
		return;
	}

	const pixelpart::MeshResource& mesh = nativeEffect->project.effect.resources.meshes.at(std::string(resourceId));

	for(std::size_t i = 0u; i < mesh.faces.size(); i++) {
		triangles[i] = static_cast<int32_t>(mesh.faces[i]);
	}

	for(std::size_t i = 0u; i < mesh.positions.size(); i++) {
		vertices[i] = Vector3{ mesh.positions[i].x, mesh.positions[i].y, mesh.positions[i].z };
		normals[i] = Vector3{ mesh.normals[i].x, mesh.normals[i].y, mesh.normals[i].z };
		uv[i] = Vector2{ mesh.textureCoords[i].x, mesh.textureCoords[i].y };
	}
}
}