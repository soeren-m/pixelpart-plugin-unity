#pragma once

#include "project/Project.h"
#include "engine/ParticleEngine.h"
#include <random>

#if defined(__CYGWIN32__)
	#define UNITY_INTERFACE_API __stdcall
	#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
	#define UNITY_INTERFACE_API __stdcall
	#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__)
	#define UNITY_INTERFACE_API
	#define UNITY_INTERFACE_EXPORT
#else
	#define UNITY_INTERFACE_API
	#define UNITY_INTERFACE_EXPORT
#endif

struct Vector2 {
	float x;
	float y;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};
struct Color {
	float r;
	float g;
	float b;
	float a;
};

struct PixelpartMeshData {
	struct ParticleTrail {
		uint32_t numParticles = 0u;
		pixelpart::float_t length = 0.0;

		std::vector<pixelpart::vec3_t> position;
		std::vector<pixelpart::vec3_t> size;
		std::vector<pixelpart::vec4_t> color;
		std::vector<pixelpart::vec3_t> velocity;
		std::vector<pixelpart::vec3_t> force;
		std::vector<pixelpart::float_t> life;

		std::vector<pixelpart::vec3_t> direction;
		std::vector<pixelpart::float_t> index;
	};

	pixelpart::ParticleData sortedParticleData;
	std::vector<uint32_t> sortKeys;

	std::unordered_map<uint32_t, ParticleTrail> trails;
};

struct PixelpartNativeEffect {
	pixelpart::Project project;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float speed = 1.0f;

	pixelpart::ParticleEngine particleEngine;
	float timeStep = 1.0f / 60.0f;
	float simulationTime = 0.0f;

	std::vector<PixelpartMeshData> meshData;

	std::mt19937 rng;
};