#pragma once

#include "Project.h"
#include "ParticleEngine.h"
#include "RenderUtil.h"

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

	pixelpart::ParticleMeshBuildInfo* particleMeshBuildInfo = nullptr;
	pixelpart::SpriteMeshBuildInfo* spriteMeshBuildInfo = nullptr;
};