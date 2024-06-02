#pragma once

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

struct Matrix4x4 {
	float data[16];
};