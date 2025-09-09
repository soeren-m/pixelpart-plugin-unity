#include "EffectRuntime.h"
#include "Common.h"
#include "Error.h"
#include "ShaderLanguage.h"
#include "ShaderLanguageURP.h"
#include "ShaderLanguageHDRP.h"
#include "pixelpart-runtime/common/Curve.h"
#include "pixelpart-runtime/common/Transform.h"
#include "pixelpart-runtime/effect/Node.h"
#include "pixelpart-runtime/computegraph/ComputeGraph.h"
#include "pixelpart-runtime/shadergraph/ShaderGraph.h"
#include "pixelpart-runtime/engine/MultiThreadedEffectEngine.h"
#include "pixelpart-runtime/engine/SingleThreadedEffectEngine.h"
#include "pixelpart-runtime/json/json.hpp"
#include <cstdint>
#include <cstring>
#include <vector>
#include <locale>
#include <exception>
#include <algorithm>

namespace pixelpart_unity {
pixelpart::ShaderGraphLanguage shaderLanguage = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage shaderLanguageURP = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage shaderLanguageHDRP = pixelpart::ShaderGraphLanguage();

std::mt19937 rng;
#ifdef PIXELPART_RUNTIME_MULTITHREADING
std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif
}

extern "C" {
UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const pixelpart_unity::char_t* data, pixelpart_unity::int_t size, pixelpart_unity::int_t particleCapacity) {
	if(!data || size <= 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return nullptr;
	}

	static bool initialized = false;

	if(!initialized) {
		initialized = true;

		std::locale::global(std::locale::classic());

		try {
			pixelpart::ComputeGraph::nodeFactory.registerBuiltInNodes();

			nlohmann::ordered_json jsonShaderLanguage = nlohmann::json::parse(
				PixelpartShaderLanguage_json,
				PixelpartShaderLanguage_json + PixelpartShaderLanguage_json_size);
			nlohmann::ordered_json jsonShaderLanguageURP = nlohmann::json::parse(
				PixelpartShaderLanguageURP_json,
				PixelpartShaderLanguageURP_json + PixelpartShaderLanguageURP_json_size);
			nlohmann::ordered_json jsonShaderLanguageHDRP = nlohmann::json::parse(
				PixelpartShaderLanguageHDRP_json,
				PixelpartShaderLanguageHDRP_json + PixelpartShaderLanguageHDRP_json_size);

			pixelpart_unity::shaderLanguage = jsonShaderLanguage;
			pixelpart_unity::shaderLanguageURP = jsonShaderLanguageURP;
			pixelpart_unity::shaderLanguageHDRP = jsonShaderLanguageHDRP;

			pixelpart::ShaderGraph::graphLanguage = pixelpart_unity::shaderLanguage;

#ifdef PIXELPART_RUNTIME_MULTITHREADING
			pixelpart_unity::threadPool = std::shared_ptr<pixelpart::ThreadPool>(
				new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
#endif
		}
		catch(const std::exception& e) {
			pixelpart_unity::lastError = std::string(e.what());
			return nullptr;
		}
	}

	try {
		pixelpart_unity::EffectRuntime* effectRuntime = new pixelpart_unity::EffectRuntime();
		effectRuntime->effectAsset = pixelpart::deserializeEffectAsset(data, static_cast<std::size_t>(size));

		particleCapacity = std::max(particleCapacity, 1);

#ifdef PIXELPART_RUNTIME_MULTITHREADING
		effectRuntime->effectEngine = std::unique_ptr<pixelpart::MultiThreadedEffectEngine>(new pixelpart::MultiThreadedEffectEngine(
			effectRuntime->effectAsset.effect(), static_cast<std::uint32_t>(particleCapacity), pixelpart_unity::threadPool));
#else
		effectRuntime->effectEngine = std::unique_ptr<pixelpart::SingleThreadedEffectEngine>(new pixelpart::SingleThreadedEffectEngine(
			effectRuntime->effectAsset.effect(), static_cast<std::uint32_t>(particleCapacity)));
#endif

		effectRuntime->effectAsset.effect().applyInputs();

		return effectRuntime;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartDeleteEffect(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	delete effectRuntime;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectTransform(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::matrix4x4_t transformMatrix, pixelpart_unity::vector3_t scale) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	glm::mat4 floatTransformMatrix;
	std::memcpy(&floatTransformMatrix[0][0], transformMatrix.data, sizeof(float) * 16);

	pixelpart::Transform transform(floatTransformMatrix);
	pixelpart::float3_t cappedEffectScale = glm::max(pixelpart_unity::fromUnity(scale), pixelpart::float3_t(0.0001));

	for(const std::unique_ptr<pixelpart::Node>& node : effectRuntime->effectAsset.effect().sceneGraph().nodes()) {
		if(node->parentId()) {
			continue;
		}

		node->position().keyframes({ pixelpart::Curve<pixelpart::float3_t>::Point{ 0.0,
			transform.position() / cappedEffectScale
		} });
		node->rotation().keyframes({ pixelpart::Curve<pixelpart::float3_t>::Point{ 0.0,
			transform.rotation()
		} });
		node->scale().keyframes({ pixelpart::Curve<pixelpart::float3_t>::Point{ 0.0,
			transform.scale()
		} });
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAdvanceEffect(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::float_t dt, pixelpart_unity::bool_t loop, pixelpart_unity::float_t loopTime, pixelpart_unity::float_t speed, pixelpart_unity::float_t timeStep) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	dt = std::max(dt, 0.0f);
	loopTime = std::max(loopTime, 0.0f);
	speed = std::max(speed, 0.0f);
	timeStep = std::max(timeStep * speed, 0.001f);

	effectRuntime->simulationTime += pixelpart_unity::fromUnity(dt) * pixelpart_unity::fromUnity(speed);

	while(effectRuntime->simulationTime > pixelpart_unity::fromUnity(timeStep)) {
		effectRuntime->simulationTime -= pixelpart_unity::fromUnity(timeStep);
		effectRuntime->effectEngine->advance(pixelpart_unity::fromUnity(timeStep));

		if(loop && effectRuntime->effectEngine->runtimeContext().time() > pixelpart_unity::fromUnity(loopTime)) {
			effectRuntime->effectEngine->restart(false);
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartRestartEffect(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::bool_t reset) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	effectRuntime->effectEngine->restart(reset);
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartIsEffect3d(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	return effectRuntime->effectAsset.effect().is3d();
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartGetEffectTime(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	return pixelpart_unity::toUnity(effectRuntime->effectEngine->runtimeContext().time());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectNodeCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().sceneGraph().count());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectParticleTypeCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().particleTypes().count());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectParticleRuntimeInstanceCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().particleRuntimeIds().size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetEffectParticleRuntimeInstances(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::particleruntimeid_t* runtimeIds) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	std::vector<pixelpart::ParticleRuntimeId> runtimeIdVector =
		effectRuntime->effectAsset.effect().particleRuntimeIds();

	for(std::size_t index = 0; index < runtimeIdVector.size(); index++) {
		runtimeIds[index] = pixelpart_unity::particleruntimeid_t{
			runtimeIdVector[index].emitterId.value(),
			runtimeIdVector[index].typeId.value()
		};
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartGetEffectParticleCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return effectRuntime->effectEngine->particleCount(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpawnParticles(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t count) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(count <= 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	effectRuntime->effectEngine->spawnParticles(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId),
		static_cast<std::uint32_t>(count));
}
}
