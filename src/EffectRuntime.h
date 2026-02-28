#ifndef PIXELPART_UNITY_EFFECT_RUNTIME_H
#define PIXELPART_UNITY_EFFECT_RUNTIME_H

#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/ParticleEmissionPair.h"
#include "pixelpart-runtime/effect/ShaderGraphSpecification.h"
#include "pixelpart-runtime/asset/EffectAsset.h"
#include "pixelpart-runtime/engine/EffectEngine.h"
#include "pixelpart-runtime/vertex/ParticleVertexGenerator.h"
#include "pixelpart-runtime/vertex/VertexDataBufferDimensions.h"
#include <memory>
#include <unordered_map>
#include <random>

namespace pixelpart_unity {
extern pixelpart::ShaderGraphSpecification shaderGraphSpecification;
extern pixelpart::ShaderGraphSpecification shaderGraphSpecificationURP;
extern pixelpart::ShaderGraphSpecification shaderGraphSpecificationHDRP;

extern std::shared_ptr<pixelpart::ThreadPool> threadPool;
extern std::mt19937 rng;

struct EffectRuntime {
	pixelpart::EffectAsset effectAsset;
	std::unique_ptr<pixelpart::EffectEngine> effectEngine;
	pixelpart::float_t simulationTime = 0.0;

	std::unordered_map<pixelpart::ParticleEmissionPair,
		std::unique_ptr<pixelpart::ParticleVertexGenerator>> vertexGenerators;
	std::unordered_map<pixelpart::ParticleEmissionPair,
		pixelpart::VertexDataBufferDimensions> vertexBufferDimensions;
};
}

#endif
