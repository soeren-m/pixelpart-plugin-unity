#include "PixelpartPluginUtil.h"

namespace util {
int32_t toUnity(pixelpart::int_t v) {
	return static_cast<int32_t>(v);
}
float toUnity(pixelpart::float_t v) {
	return static_cast<float>(v);
}
Vector2 toUnity(const pixelpart::vec2_t& v) {
	return Vector2{
		static_cast<float>(v.x),
		static_cast<float>(v.y)
	};
}
Vector3 toUnity(const pixelpart::vec3_t& v) {
	return Vector3{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}
Vector4 toUnity(const pixelpart::vec3_t& v, pixelpart::float_t w) {
	return Vector4{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(w)
	};
}
Vector4 toUnity(const pixelpart::vec4_t& v) {
	return Vector4{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(v.w)
	};
}
Color toUnityColor(const pixelpart::vec4_t& v) {
	return Color{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(v.w)
	};
}

pixelpart::int_t fromUnity(int32_t v) {
	return static_cast<pixelpart::int_t>(v);
}
pixelpart::float_t fromUnity(float v) {
	return static_cast<pixelpart::float_t>(v);
}
pixelpart::vec2_t fromUnity(const Vector2& v) {
	return pixelpart::vec2_t(v.x, v.y);
}
pixelpart::vec3_t fromUnity(const Vector3& v) {
	return pixelpart::vec3_t(v.x, v.y, v.z);
}
pixelpart::vec4_t fromUnity(const Vector4& v) {
	return pixelpart::vec4_t(v.x, v.y, v.z, v.w);
}
pixelpart::vec4_t fromUnity(const Color& v) {
	return pixelpart::vec4_t(v.r, v.g, v.b, v.a);
}
}