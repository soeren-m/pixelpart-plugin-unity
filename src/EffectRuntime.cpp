#include "EffectRuntime.h"
#include "Common.h"
#include "Error.h"
#include "ShaderLanguage.h"
#include "ShaderLanguageURP.h"
#include "ShaderLanguageHDRP.h"
#include "pixelpart-runtime/common/Curve.h"
#include "pixelpart-runtime/common/Transform.h"
#include "pixelpart-runtime/effect/Effect.h"
#include "pixelpart-runtime/effect/Node.h"
#include "pixelpart-runtime/effect/ParticleType.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include "pixelpart-runtime/effect/ComputeGraph.h"
#include "pixelpart-runtime/effect/ShaderGraph.h"
#include "pixelpart-runtime/effect/EffectRuntimeContext.h"
#include "pixelpart-runtime/engine/MultiThreadedEffectEngine.h"
#include "pixelpart-runtime/engine/SingleThreadedEffectEngine.h"
#include "pixelpart-runtime/engine/DefaultParticleGenerator.h"
#include "pixelpart-runtime/engine/DefaultParticleModifier.h"
#include "pixelpart-runtime/engine/ParticleCollection.h"
#include "pixelpart-runtime/vertex/VertexFormat.h"
#include "pixelpart-runtime/vertex/VertexAttribute.h"
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
		effectRuntime->effectEngine = std::make_unique<pixelpart::MultiThreadedEffectEngine>(effectRuntime->effectAsset.effect(),
			std::make_shared<pixelpart::DefaultParticleGenerator>(),
			std::make_shared<pixelpart::DefaultParticleModifier>(),
			pixelpart_unity::threadPool,
			static_cast<std::uint32_t>(particleCapacity));
#else
		effectRuntime->effectEngine = std::make_unique<pixelpart::SingleThreadedEffectEngine>(effectRuntime->effectAsset.effect(),
			std::make_shared<pixelpart::DefaultParticleGenerator>(),
			std::make_shared<pixelpart::DefaultParticleModifier>(),
			static_cast<std::uint32_t>(particleCapacity));
#endif

		effectRuntime->effectAsset.effect().applyInputs();

		pixelpart::VertexFormat vertexBasedVertexFormat({
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::index, pixelpart::VertexDataGenerationMode::vertex, pixelpart::VertexDataType::type_int32, 0, 0, sizeof(int32_t)),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::position3d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 1, 0, sizeof(float) * 3),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::color, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 2, 0, sizeof(float) * 4),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::normal, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 3, 0, sizeof(float) * 3),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::texture_coord, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, 0, sizeof(float) * 2),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::velocity3d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 5, 0, sizeof(float) * 4),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::life, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 6, 0, sizeof(float) * 4),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::id, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 6, sizeof(float), sizeof(float) * 4)
			},
			pixelpart::VertexWindingOrder::cw);

		pixelpart::VertexFormat instancingVertexFormat({
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::matrix4x4_column_major, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 0, 0, sizeof(float) * 16),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::color, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 1, 0, sizeof(float) * 4),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::velocity3d, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 2, 0, sizeof(float) * 4),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::life, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 3, 0, sizeof(float)),
				pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::id, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 4, 0, sizeof(float))
			},
			pixelpart::VertexWindingOrder::cw);

		for(const pixelpart::ParticleEmissionPair& emissionPair : effectRuntime->effectAsset.effect().particleEmissionPairs()) {
			const pixelpart::ParticleType& particleType = effectRuntime->effectAsset.effect().particleTypes().at(emissionPair.typeId);

			pixelpart::VertexFormat vertexFormat;
			switch(particleType.renderer()) {
				case pixelpart::ParticleRendererType::sprite:
				case pixelpart::ParticleRendererType::trail:
					vertexFormat = vertexBasedVertexFormat;
					break;
				case pixelpart::ParticleRendererType::mesh:
					vertexFormat = instancingVertexFormat;
					break;
				default:
					break;
			}

			effectRuntime->vertexGenerators[emissionPair] = std::make_unique<pixelpart::ParticleVertexGenerator>(
				effectRuntime->effectAsset.effect(), emissionPair.emitterId, emissionPair.typeId,
				vertexFormat);
			effectRuntime->vertexBufferDimensions[emissionPair] = pixelpart::VertexDataBufferDimensions();
		}

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

		if(loop && effectRuntime->effectEngine->context().time() > pixelpart_unity::fromUnity(loopTime)) {
			effectRuntime->effectEngine->clearParticles();
			effectRuntime->effectEngine->restart();
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartRestartEffect(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::bool_t clear) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	if(clear) {
		effectRuntime->effectEngine->clearParticles();
	}

	effectRuntime->effectEngine->restart();
}

UNITY_INTERFACE_EXPORT pixelpart_unity::float_t UNITY_INTERFACE_API PixelpartGetEffectTime(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0.0f;
	}

	return pixelpart_unity::toUnity(effectRuntime->effectEngine->context().time());
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartIsEffectFinished(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
	pixelpart::float_t time = effectRuntime->effectEngine->context().time();

	for(const auto* particleEmitter : effect.sceneGraph().nodesWithType<pixelpart::ParticleEmitter>()) {
		if(!particleEmitter->primary()) {
			continue;
		}

		if(particleEmitter->active(effectRuntime->effectEngine->context()) || particleEmitter->repeat() ||
			time < particleEmitter->start() + particleEmitter->duration()) { // TODO: triggers
			return false;
		}
	}

	for(const auto& [emissionPair, particleCollection] : effectRuntime->effectEngine->state().particleCollections()) {
		if(particleCollection.count() > 0) {
			return false;
		}
	}

	return true;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartIsEffect3d(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	return effectRuntime->effectAsset.effect().is3d();
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

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartGetEffectParticleEmissionPairCount(pixelpart_unity::EffectRuntime* effectRuntime) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	return static_cast<pixelpart_unity::int_t>(effectRuntime->effectAsset.effect().particleEmissionPairs().size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetEffectParticleEmissionPairs(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::particleemissionpair_t* emissionPairs) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	std::vector<pixelpart::ParticleEmissionPair> emissionPairVector =
		effectRuntime->effectAsset.effect().particleEmissionPairs();

	for(std::size_t index = 0; index < emissionPairVector.size(); index++) {
		emissionPairs[index] = pixelpart_unity::particleemissionpair_t{
			emissionPairVector[index].emitterId.value(),
			emissionPairVector[index].typeId.value()
		};
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::uint_t UNITY_INTERFACE_API PixelpartGetEffectParticleCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId) {
	if(!effectRuntime || !effectRuntime->effectEngine) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->state().particleCollection(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId));

	return particleCollection->count();
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

	effectRuntime->effectEngine->generateParticles(
		static_cast<std::uint32_t>(count),
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId),
		pixelpart::EffectRuntimeContext());
}
}
