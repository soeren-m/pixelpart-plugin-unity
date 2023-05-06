#pragma once

#include "Project.h"
#include "ParticleEngine.h"

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
		uint32_t numParticles = 0;
		pixelpart::floatd length = 0.0;

		std::vector<pixelpart::vec3d> position;
		std::vector<pixelpart::vec3d> size;
		std::vector<pixelpart::vec4d> color;
		std::vector<pixelpart::vec3d> velocity;
		std::vector<pixelpart::vec3d> force;
		std::vector<pixelpart::vec3d> direction;
		std::vector<pixelpart::floatd> index;
		std::vector<pixelpart::floatd> life;
	};

	pixelpart::ParticleData sortedParticleData;
	std::vector<uint32_t> sortKeys;

	std::unordered_map<uint32_t, ParticleTrail> trails;
};

struct PixelpartNativeEffect {
	pixelpart::Project project;
	pixelpart::ResourceDatabase projectResources;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float speed = 1.0f;

	pixelpart::ParticleEngine particleEngine;
	float timeStep = 1.0f / 60.0f;
	float simulationTime = 0.0f;

	std::vector<PixelpartMeshData> meshData;
};