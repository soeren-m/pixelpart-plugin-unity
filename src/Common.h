#ifndef PIXELPART_UNITY_COMMON_H
#define PIXELPART_UNITY_COMMON_H

#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/common/Math.h"
#include <cstdint>

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

namespace pixelpart_unity {
using bool_t = bool;
using int_t = std::int32_t;
using uint_t = std::uint32_t;
using float_t = float;
using char_t = char;

struct vector2_t {
	float x;
	float y;
};

struct vector3_t {
	float x;
	float y;
	float z;
};

struct vector4_t {
	float x;
	float y;
	float z;
	float w;
};

struct color_t {
	float r;
	float g;
	float b;
	float a;
};

struct matrix4x4_t {
	float data[16];
};

struct alignas(8) particleruntimeid_t {
	std::uint32_t emitterId;
	std::uint32_t typeId;
};

int_t toUnity(pixelpart::int_t v);
float_t toUnity(pixelpart::float_t v);
vector2_t toUnity(const pixelpart::float2_t& v);
vector3_t toUnity(const pixelpart::float3_t& v);
vector4_t toUnity(const pixelpart::float3_t& v, pixelpart::float_t w);
vector4_t toUnity(const pixelpart::float4_t& v);
color_t toUnityColor(const pixelpart::float4_t& v);

pixelpart::int_t fromUnity(int_t v);
pixelpart::float_t fromUnity(float_t v);
pixelpart::float2_t fromUnity(const vector2_t& v);
pixelpart::float3_t fromUnity(const vector3_t& v);
pixelpart::float4_t fromUnity(const vector4_t& v);
pixelpart::float4_t fromUnity(const color_t& v);
}

#endif