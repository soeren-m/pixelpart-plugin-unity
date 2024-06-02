#pragma once

#include "PixelpartPluginCommon.h"
#include "project/Project.h"
#include "engine/CPUParticleEngine.h"
#include <random>

struct PixelpartPluginEffect {
	struct ParticleTrail {
		uint32_t numParticles = 0u;
		pixelpart::float_t length = 0.0;

		std::vector<pixelpart::vec3_t> position;
		std::vector<pixelpart::vec3_t> size;
		std::vector<pixelpart::vec4_t> color;
		std::vector<pixelpart::vec3_t> velocity;
		std::vector<pixelpart::float_t> life;
		std::vector<pixelpart::vec3_t> direction;
		std::vector<pixelpart::vec3_t> directionToEdge;
		std::vector<pixelpart::float_t> index;
	};

	struct MeshData {
		pixelpart::ParticleContainer sortedParticleContainer;
		std::vector<uint32_t> sortKeys;

		std::unordered_map<uint32_t, ParticleTrail> trails;
	};

#ifndef __EMSCRIPTEN__
	static std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif

	static int32_t particleCapacity;
	static pixelpart::ShaderGraphLanguage shaderLanguage;
	static pixelpart::ShaderGraphLanguage shaderLanguageURP;
	static pixelpart::ShaderGraphLanguage shaderLanguageHDRP;

	pixelpart::Project project;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float speed = 1.0f;

	std::unique_ptr<pixelpart::CPUParticleEngine> particleEngine;
	float timeStep = 1.0f / 60.0f;
	float simulationTime = 0.0f;

	std::vector<MeshData> meshData;

	std::mt19937 rng;
};