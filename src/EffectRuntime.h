#ifndef PIXELPART_UNITY_EFFECT_RUNTIME_H
#define PIXELPART_UNITY_EFFECT_RUNTIME_H

#include "ParticleMeshData.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/ParticleRuntimeId.h"
#include "pixelpart-runtime/asset/EffectAsset.h"
#include "pixelpart-runtime/engine/EffectEngine.h"
#include <memory>
#include <unordered_map>
#include <random>

namespace pixelpart_unity {
extern pixelpart::ShaderGraphLanguage shaderLanguage;
extern pixelpart::ShaderGraphLanguage shaderLanguageURP;
extern pixelpart::ShaderGraphLanguage shaderLanguageHDRP;

extern std::mt19937 rng;
#ifdef PIXELPART_RUNTIME_MULTITHREADING
extern std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif

struct EffectRuntime {
	pixelpart::EffectAsset effectAsset;
	std::unique_ptr<pixelpart::EffectEngine> effectEngine;
	pixelpart::float_t simulationTime = 0.0;

	std::unordered_map<pixelpart::ParticleRuntimeId, ParticleMeshData> meshData;
};
}

#endif