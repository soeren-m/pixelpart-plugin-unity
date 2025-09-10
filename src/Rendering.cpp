#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/Transform.h"
#include "pixelpart-runtime/common/Curve.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include "pixelpart-runtime/effect/ParticleType.h"
#include "pixelpart-runtime/effect/Effect.h"
#include "pixelpart-runtime/effect/ParticleRuntimeId.h"
#include "pixelpart-runtime/engine/ParticleCollection.h"
#include <cstdint>
#include <cstring>
#include <cmath>
#include <utility>
#include <vector>
#include <numeric>
#include <exception>
#include <algorithm>

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetSortedParticleRuntimeInstances(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t* indices) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();

	std::vector<pixelpart_unity::int_t> sortedIndices(effect.particleRuntimeIds().size());
	std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
	std::sort(sortedIndices.begin(), sortedIndices.end(), [&effect](pixelpart_unity::int_t i1, pixelpart_unity::int_t i2) {
		const pixelpart::ParticleType& pt1 = effect.particleTypes().at(effect.particleRuntimeIds()[i1].typeId);
		const pixelpart::ParticleType& pt2 = effect.particleTypes().at(effect.particleRuntimeIds()[i2].typeId);

		return pt1.layer() < pt2.layer();
	});

	std::memcpy(indices, sortedIndices.data(), sortedIndices.size() * sizeof(pixelpart_unity::int_t));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartConstructParticleGeometry(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t cameraForward, pixelpart_unity::vector3_t cameraRight, pixelpart_unity::vector3_t cameraUp, pixelpart_unity::vector3_t effectScale,
	pixelpart_unity::int_t* bufferSizes) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(!bufferSizes) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));
		if(!particleCollection) {
			return;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart::RuntimeContext runtimeContext = effectRuntime->effectEngine->runtimeContext();
		pixelpart::SceneContext sceneContext;
		sceneContext.effectScale = pixelpart_unity::fromUnity(effectScale);
		sceneContext.cameraPosition = pixelpart_unity::fromUnity(cameraPosition);
		sceneContext.cameraForward = pixelpart_unity::fromUnity(cameraForward);
		sceneContext.cameraRight = pixelpart_unity::fromUnity(cameraRight);
		sceneContext.cameraUp = pixelpart_unity::fromUnity(cameraUp);

		auto& vertexGenerator = effectRuntime->vertexGenerators.at(runtimeId);
		pixelpart::VertexDataBufferDimensions bufferDimensions = vertexGenerator->buildGeometry(
			particles, particleCount,
			runtimeContext, sceneContext);

		effectRuntime->vertexBufferDimensions[runtimeId] = bufferDimensions;

		for(std::size_t bufferSize : bufferDimensions) {
			*bufferSizes = static_cast<pixelpart_unity::int_t>(bufferSize);
			bufferSizes++;
		}
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGenerateParticleVertexData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t cameraForward, pixelpart_unity::vector3_t cameraRight, pixelpart_unity::vector3_t cameraUp, pixelpart_unity::vector3_t effectScale,
	pixelpart_unity::int_t* triangles, pixelpart_unity::vector3_t* vertices, pixelpart_unity::color_t* colors, pixelpart_unity::vector3_t* normals, pixelpart_unity::vector2_t* uv0, pixelpart_unity::vector4_t* uv1, pixelpart_unity::vector4_t* uv2) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}
	else if(!triangles || !vertices || !colors || !normals || !uv0 || !uv1 || !uv2) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return false;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
		const pixelpart::ParticleType& particleType = effect.particleTypes().at(runtimeId.typeId);

		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(runtimeId.emitterId, runtimeId.typeId);
		if(!particleCollection) {
			return false;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart::RuntimeContext runtimeContext = effectRuntime->effectEngine->runtimeContext();
		pixelpart::SceneContext sceneContext;
		sceneContext.effectScale = pixelpart_unity::fromUnity(effectScale);
		sceneContext.cameraPosition = pixelpart_unity::fromUnity(cameraPosition);
		sceneContext.cameraForward = pixelpart_unity::fromUnity(cameraForward);
		sceneContext.cameraRight = pixelpart_unity::fromUnity(cameraRight);
		sceneContext.cameraUp = pixelpart_unity::fromUnity(cameraUp);

		auto& vertexGenerator = effectRuntime->vertexGenerators.at(runtimeId);
		vertexGenerator->generateVertexData(pixelpart::VertexDataBufferCollection({
				reinterpret_cast<std::uint8_t*>(triangles),
				reinterpret_cast<std::uint8_t*>(vertices),
				reinterpret_cast<std::uint8_t*>(colors),
				reinterpret_cast<std::uint8_t*>(normals),
				reinterpret_cast<std::uint8_t*>(uv0),
				reinterpret_cast<std::uint8_t*>(uv1),
				reinterpret_cast<std::uint8_t*>(uv2)
			}),
			particles, particleCount,
			runtimeContext, sceneContext);

		if(!effect.is3d()) {
			float zOffset = -0.001f * static_cast<float>(particleType.layer());

			const auto& vertexBufferDimensions = effectRuntime->vertexBufferDimensions[runtimeId];
			for(std::size_t index = 0; index < vertexBufferDimensions.at(1); index++) {
				vertices[index].z += zOffset;
			}
		}

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

    return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGenerateParticleInstanceData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t cameraForward, pixelpart_unity::vector3_t cameraRight, pixelpart_unity::vector3_t cameraUp, pixelpart_unity::vector3_t effectScale,
	pixelpart_unity::matrix4x4_t* transforms, pixelpart_unity::vector4_t* colors, pixelpart_unity::vector4_t* velocities, pixelpart_unity::float_t* lives, pixelpart_unity::float_t* ids) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}
	else if(!transforms || !colors || !velocities || !lives || !ids) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return false;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(runtimeId.emitterId, runtimeId.typeId);
		if(!particleCollection) {
			return false;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart::RuntimeContext runtimeContext = effectRuntime->effectEngine->runtimeContext();
		pixelpart::SceneContext sceneContext;
		sceneContext.effectScale = pixelpart_unity::fromUnity(effectScale);
		sceneContext.cameraPosition = pixelpart_unity::fromUnity(cameraPosition);
		sceneContext.cameraForward = pixelpart_unity::fromUnity(cameraForward);
		sceneContext.cameraRight = pixelpart_unity::fromUnity(cameraRight);
		sceneContext.cameraUp = pixelpart_unity::fromUnity(cameraUp);

		auto& vertexGenerator = effectRuntime->vertexGenerators.at(runtimeId);
		vertexGenerator->generateVertexData(pixelpart::VertexDataBufferCollection({
				reinterpret_cast<std::uint8_t*>(transforms),
				reinterpret_cast<std::uint8_t*>(colors),
				reinterpret_cast<std::uint8_t*>(velocities),
				reinterpret_cast<std::uint8_t*>(lives),
				reinterpret_cast<std::uint8_t*>(ids)
			}),
			particles, particleCount,
			runtimeContext, sceneContext);

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}
}
