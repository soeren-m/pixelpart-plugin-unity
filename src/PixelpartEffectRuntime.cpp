#include "PixelpartEffectRuntime.h"
#include "PixelpartPluginCommon.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartShaderLanguage.h"
#include "PixelpartShaderLanguageURP.h"
#include "PixelpartShaderLanguageHDRP.h"
#include "pixelpart-runtime/computegraph/ComputeGraph.h"
#include "pixelpart-runtime/shadergraph/ShaderGraph.h"
#include "pixelpart-runtime/json/json.hpp"
#include <cstring>
#include <locale>
#include <algorithm>

bool PixelpartEffectRuntime::initialized = false;
uint32_t PixelpartEffectRuntime::particleCapacity = 10000;
pixelpart::ShaderGraphLanguage PixelpartEffectRuntime::shaderLanguage = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage PixelpartEffectRuntime::shaderLanguageURP = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage PixelpartEffectRuntime::shaderLanguageHDRP = pixelpart::ShaderGraphLanguage();
#ifdef PIXELPART_RUNTIME_MULTITHREADING
std::shared_ptr<pixelpart::ThreadPool> PixelpartEffectRuntime::threadPool;
#endif

extern "C" {
UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const char* data, UnityInt size) {
	if(!data || size <= 0) {
		return nullptr;
	}

	if(!PixelpartEffectRuntime::initialized) {
		PixelpartEffectRuntime::initialized = true;

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

			PixelpartEffectRuntime::shaderLanguage = jsonShaderLanguage;
			PixelpartEffectRuntime::shaderLanguageURP = jsonShaderLanguageURP;
			PixelpartEffectRuntime::shaderLanguageHDRP = jsonShaderLanguageHDRP;

			pixelpart::ShaderGraph::graphLanguage = PixelpartEffectRuntime::shaderLanguage;

	#ifdef PIXELPART_RUNTIME_MULTITHREADING
			PixelpartEffectRuntime::threadPool = std::shared_ptr<pixelpart::ThreadPool>(
				new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
	#endif
		}
		catch(...) {
			// Do nothing
		}
	}

	try {
		PixelpartEffectRuntime* effectRuntime = new PixelpartEffectRuntime();
		effectRuntime->effectAsset = pixelpart::deserializeEffectAsset(data, static_cast<std::size_t>(size));

#ifdef PIXELPART_RUNTIME_MULTITHREADING
		effectRuntime->effectEngine = std::unique_ptr<pixelpart::MultiThreadedEffectEngine>(new pixelpart::MultiThreadedEffectEngine(
			effectRuntime->effectAsset.effect(), PixelpartEffectRuntime::particleCapacity, PixelpartEffectRuntime::threadPool));
#else
		effectRuntime->effectEngine = std::unique_ptr<pixelpart::SingleThreadedEffectEngine>(new pixelpart::SingleThreadedEffectEngine(
			effectRuntime->effectAsset.effect(), PixelpartEffectRuntime::particleCapacity));
#endif

		effectRuntime->effectAsset.effect().applyInputs();

		return effectRuntime;
	}
	catch(...) {
		return nullptr;
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartDeleteEffect(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return;
	}

	delete effectRuntime;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartAdvanceEffect(PixelpartEffectRuntime* effectRuntime, UnityFloat dt, UnityBool loop, UnityFloat loopTime, UnityFloat speed, UnityFloat timeStep) {
	if(!effectRuntime) {
		return;
	}

	dt = std::max(dt, 0.0f);
	loopTime = std::max(loopTime, 0.0f);
	speed = std::max(speed, 0.0f);
	timeStep = std::max(timeStep * speed, 0.001f);

	effectRuntime->simulationTime += dt * speed;

	while(effectRuntime->simulationTime > timeStep) {
		effectRuntime->simulationTime -= timeStep;
		effectRuntime->effectEngine->advance(timeStep);

		if(loop && effectRuntime->effectEngine->runtimeContext().time() > loopTime) {
			effectRuntime->effectEngine->restart(false);
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartRestartEffect(PixelpartEffectRuntime* effectRuntime, UnityBool reset) {
	if(!effectRuntime) {
		return;
	}

	effectRuntime->effectEngine->restart(reset);
}

UNITY_INTERFACE_EXPORT UnityBool UNITY_INTERFACE_API PixelpartIsEffect3d(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return false;
	}

	return effectRuntime->effectAsset.effect().is3d();
}

UNITY_INTERFACE_EXPORT UnityFloat UNITY_INTERFACE_API PixelpartGetEffectTime(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0.0f;
	}

	return effectRuntime->effectEngine->runtimeContext().time();
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetEffectNodeCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityInt>(effectRuntime->effectAsset.effect().sceneGraph().count());
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartGetEffectParticleTypeCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityInt>(effectRuntime->effectAsset.effect().particleTypes().count());
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetEffectParticleRuntimeInstanceCount(PixelpartEffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		return 0;
	}

	return static_cast<UnityUInt>(effectRuntime->effectAsset.effect().particleRuntimeIds().size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetEffectParticleRuntimeInstances(PixelpartEffectRuntime* effectRuntime, UnityParticleRuntimeId* runtimeIds) {
	if(!effectRuntime) {
		return;
	}

	std::vector<pixelpart::ParticleRuntimeId> runtimeIdVector =
		effectRuntime->effectAsset.effect().particleRuntimeIds();

	for(std::size_t index = 0; index < runtimeIdVector.size(); index++) {
		runtimeIds[index] = UnityParticleRuntimeId{
			runtimeIdVector[index].emitterId.value(),
			runtimeIdVector[index].typeId.value()
		};
	}
}

UNITY_INTERFACE_EXPORT UnityUInt UNITY_INTERFACE_API PixelpartGetEffectParticleCount(PixelpartEffectRuntime* effectRuntime, UnityUInt particleEmitterId, UnityUInt particleTypeId) {
	if(!effectRuntime) {
		return 0;
	}

	return effectRuntime->effectEngine->particleCount(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpawnParticles(PixelpartEffectRuntime* effectRuntime, UnityUInt particleEmitterId, UnityUInt particleTypeId, UnityInt count) {
	if(!effectRuntime || count <= 0) {
		return;
	}

	effectRuntime->effectEngine->spawnParticles(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId),
		static_cast<uint32_t>(count));
}
}