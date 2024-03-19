#pragma once

#include "PixelpartPlugin.h"

namespace util {
int32_t toUnity(pixelpart::int_t v);
float toUnity(pixelpart::float_t v);
Vector2 toUnity(const pixelpart::vec2_t& v);
Vector3 toUnity(const pixelpart::vec3_t& v);
Vector4 toUnity(const pixelpart::vec3_t& v, pixelpart::float_t w);
Vector4 toUnity(const pixelpart::vec4_t& v);
Color toUnityColor(const pixelpart::vec4_t& v);

pixelpart::int_t fromUnity(int32_t v);
pixelpart::float_t fromUnity(float v);
pixelpart::vec2_t fromUnity(const Vector2& v);
pixelpart::vec3_t fromUnity(const Vector3& v);
pixelpart::vec4_t fromUnity(const Vector4& v);
pixelpart::vec4_t fromUnity(const Color& v);
}