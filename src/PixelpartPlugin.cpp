#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"
#include "PixelpartShaderLanguage.h"
#include "PixelpartShaderLanguageURP.h"
#include "PixelpartShaderLanguageHDRP.h"
#include <locale>

#ifndef __EMSCRIPTEN__
std::shared_ptr<pixelpart::ThreadPool> PixelpartPluginEffect::threadPool;
#endif

int32_t PixelpartPluginEffect::particleCapacity = 10000;
pixelpart::ShaderGraphLanguage PixelpartPluginEffect::shaderLanguage = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage PixelpartPluginEffect::shaderLanguageURP = pixelpart::ShaderGraphLanguage();
pixelpart::ShaderGraphLanguage PixelpartPluginEffect::shaderLanguageHDRP = pixelpart::ShaderGraphLanguage();

static bool initialized = false;

extern "C" {
UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const char* data, int32_t size) {
	if(!data || size <= 0) {
		return nullptr;
	}

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

			PixelpartPluginEffect::shaderLanguage = jsonShaderLanguage;
			PixelpartPluginEffect::shaderLanguageURP = jsonShaderLanguageURP;
			PixelpartPluginEffect::shaderLanguageHDRP = jsonShaderLanguageHDRP;

			pixelpart::ShaderGraph::graphLanguage = PixelpartPluginEffect::shaderLanguage;

	#ifndef __EMSCRIPTEN__
			PixelpartPluginEffect::threadPool = std::shared_ptr<pixelpart::ThreadPool>(
				new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
	#endif
		}
		catch(...) {
			// Do nothing
		}
	}

	try {
		PixelpartPluginEffect* internalEffect = new PixelpartPluginEffect();
		internalEffect->project = pixelpart::deserialize(data, static_cast<std::size_t>(size));

#ifndef __EMSCRIPTEN__
		internalEffect->particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(new pixelpart::CPUParticleEngine(
			internalEffect->project.effect, PixelpartPluginEffect::particleCapacity, PixelpartPluginEffect::threadPool));
#else
		internalEffect->particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(new pixelpart::CPUParticleEngine(
			internalEffect->project.effect, PixelpartPluginEffect::particleCapacity));
#endif

		pixelpart::refreshEffectProperties(internalEffect->project.effect);

		internalEffect->meshData.resize(internalEffect->project.effect.particleTypes.getCount());

		return internalEffect;
	}
	catch(...) {
		return nullptr;
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartDeleteEffect(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return;
	}

	delete internalEffect;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateEffect(PixelpartPluginEffect* internalEffect, float dt) {
	if(!internalEffect) {
		return;
	}

	if(internalEffect->playing) {
		internalEffect->simulationTime += glm::clamp(dt, 0.0f, 1.0f) * internalEffect->speed;

		while(internalEffect->simulationTime > internalEffect->timeStep) {
			internalEffect->simulationTime -= internalEffect->timeStep;
			internalEffect->particleEngine->step(internalEffect->timeStep);

			if(internalEffect->loop) {
				if(internalEffect->particleEngine->getTime() > internalEffect->loopTime) {
					internalEffect->particleEngine->restart(false);
				}
			}
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPlayEffect(PixelpartPluginEffect* internalEffect, bool state) {
	if(!internalEffect) {
		return;
	}

	internalEffect->playing = state;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartRestartEffect(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return;
	}

	internalEffect->particleEngine->restart(true);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartResetEffect(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return;
	}

	internalEffect->particleEngine->restart(false);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartIsEffect3D(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return false;
	}

	return internalEffect->project.effect.is3d;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectLoop(PixelpartPluginEffect* internalEffect, bool loop) {
	if(!internalEffect) {
		return;
	}

	internalEffect->loop = loop;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectLoopTime(PixelpartPluginEffect* internalEffect, float loopTime) {
	if(!internalEffect) {
		return;
	}

	internalEffect->loopTime = std::max(loopTime, 0.0f);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectSpeed(PixelpartPluginEffect* internalEffect, float speed) {
	if(!internalEffect) {
		return;
	}

	internalEffect->speed = std::max(speed, 0.0f);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectTimeStep(PixelpartPluginEffect* internalEffect, float timeStep) {
	if(!internalEffect) {
		return;
	}

	internalEffect->timeStep = std::max(timeStep, 0.001f);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartIsEffectPlaying(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return false;
	}

	return internalEffect->playing;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetEffectLoop(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return false;
	}

	return internalEffect->loop;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectLoopTime(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0.0f;
	}

	return internalEffect->loopTime;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectSpeed(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0.0f;
	}

	return internalEffect->speed;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectTimeStep(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0.0f;
	}

	return internalEffect->timeStep;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectTime(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0.0f;
	}

	return internalEffect->particleEngine->getTime();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticleEmitters(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return internalEffect->project.effect.particleEmitters.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticleTypes(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return internalEffect->project.effect.particleTypes.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumForceFields(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return internalEffect->project.effect.forceFields.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumColliders(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return internalEffect->project.effect.colliders.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticles(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex) {
	if(!internalEffect || particleTypeIndex >= internalEffect->project.effect.particleTypes.getCount()) {
		return 0;
	}

	return internalEffect->particleEngine->getNumParticles(particleTypeIndex);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetNumEffectInputs(PixelpartPluginEffect* internalEffect) {
	if(!internalEffect) {
		return 0;
	}

	return static_cast<int32_t>(internalEffect->project.effect.inputs.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetEffectInputs(PixelpartPluginEffect* internalEffect, uint32_t* ids, char* names, int32_t bufferSizeNames) {
	if(!internalEffect || ids == nullptr || names == nullptr || bufferSizeNames < 2) {
		return 0;
	}

	int32_t index = 0;
	std::string namesString;
	for(const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry : internalEffect->project.effect.inputs) {
		ids[index++] = static_cast<uint32_t>(entry.first);
		namesString += entry.second.name;
		namesString += " ";
	}

	if(!namesString.empty()) {
		namesString.pop_back();
	}

	int32_t lengthNames = std::min(static_cast<int32_t>(namesString.size()), bufferSizeNames - 1);
	std::strncpy(names, namesString.c_str(), lengthNames);
	names[lengthNames] = '\0';

	return lengthNames;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputBool(PixelpartPluginEffect* internalEffect, uint32_t inputId, bool value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Bool(value);

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputInt(PixelpartPluginEffect* internalEffect, uint32_t inputId, int32_t value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Int(util::fromUnity(value));

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat(PixelpartPluginEffect* internalEffect, uint32_t inputId, float value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float(util::fromUnity(value));

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat2(PixelpartPluginEffect* internalEffect, uint32_t inputId, Vector2 value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float2(util::fromUnity(value));

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat3(PixelpartPluginEffect* internalEffect, uint32_t inputId, Vector3 value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float3(util::fromUnity(value));

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectInputFloat4(PixelpartPluginEffect* internalEffect, uint32_t inputId, Vector4 value) {
	if(!internalEffect) {
		return;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float4(util::fromUnity(value));

	pixelpart::refreshEffectProperties(internalEffect->project.effect);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetEffectInputBool(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return false;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return false;
	}

	return inputIt->second.value.toBool();
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetEffectInputInt(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return 0;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return 0;
	}

	return util::toUnity(inputIt->second.value.toInt());
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectInputFloat(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return 0.0f;
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return 0.0f;
	}

	return util::toUnity(inputIt->second.value.toFloat());
}

UNITY_INTERFACE_EXPORT Vector2 UNITY_INTERFACE_API PixelpartGetEffectInputFloat2(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return Vector2{ 0.0f, 0.0f };
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return Vector2{ 0.0f, 0.0f };
	}

	return util::toUnity(inputIt->second.value.toFloat2());
}

UNITY_INTERFACE_EXPORT Vector3 UNITY_INTERFACE_API PixelpartGetEffectInputFloat3(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	return util::toUnity(inputIt->second.value.toFloat3());
}

UNITY_INTERFACE_EXPORT Vector4 UNITY_INTERFACE_API PixelpartGetEffectInputFloat4(PixelpartPluginEffect* internalEffect, uint32_t inputId) {
	if(!internalEffect) {
		return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	pixelpart::EffectInputCollection::iterator inputIt = internalEffect->project.effect.inputs.find(static_cast<pixelpart::id_t>(inputId));
	if(inputIt == internalEffect->project.effect.inputs.end()) {
		return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return util::toUnity(inputIt->second.value.toFloat4());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpawnParticles(PixelpartPluginEffect* internalEffect, uint32_t particleTypeId, int32_t count) {
	if(!internalEffect || count <= 0) {
		return;
	}

	internalEffect->particleEngine->spawnParticles(particleTypeId, static_cast<uint32_t>(count));
}
}