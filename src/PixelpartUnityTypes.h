#ifndef PIXELPART_UNITY_TYPES_H
#define PIXELPART_UNITY_TYPES_H

#include <cstdint>

using UnityBool = bool;
using UnityInt = int32_t;
using UnityUInt = uint32_t;
using UnityFloat = float;

struct UnityVector2 {
	float x;
	float y;
};

struct UnityVector3 {
	float x;
	float y;
	float z;
};

struct UnityVector4 {
	float x;
	float y;
	float z;
	float w;
};

struct UnityColor {
	float r;
	float g;
	float b;
	float a;
};

struct UnityMatrix4x4 {
	float data[16];
};

struct alignas(8) UnityParticleRuntimeId {
	uint32_t emitterId;
	uint32_t typeId;
};

#endif