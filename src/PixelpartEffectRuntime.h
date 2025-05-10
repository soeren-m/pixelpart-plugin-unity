#ifndef PIXELPART_EFFECT_RUNTIME_H
#define PIXELPART_EFFECT_RUNTIME_H

#include "PixelpartParticleMeshData.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/ParticleRuntimeId.h"
#include "pixelpart-runtime/asset/EffectAsset.h"
#include "pixelpart-runtime/engine/MultiThreadedEffectEngine.h"
#include "pixelpart-runtime/engine/SingleThreadedEffectEngine.h"
#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <random>

struct PixelpartEffectRuntime {
	static bool initialized;
	static uint32_t particleCapacity;
	static pixelpart::ShaderGraphLanguage shaderLanguage;
	static pixelpart::ShaderGraphLanguage shaderLanguageURP;
	static pixelpart::ShaderGraphLanguage shaderLanguageHDRP;
#ifdef PIXELPART_RUNTIME_MULTITHREADING
	static std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif

	pixelpart::EffectAsset effectAsset;

	std::unique_ptr<pixelpart::EffectEngine> effectEngine;
	float simulationTime = 0.0f;

	std::unordered_map<pixelpart::ParticleRuntimeId, PixelpartParticleMeshData> meshData;

	std::mt19937 rng;
};

#endif