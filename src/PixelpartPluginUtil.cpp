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

pixelpart::vec3_t rotate2d(const pixelpart::vec3_t& p, const pixelpart::vec3_t& o, pixelpart::float_t a) {
	pixelpart::float_t s = std::sin(glm::radians(a));
	pixelpart::float_t c = std::cos(glm::radians(a));

	return pixelpart::vec3_t(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3_t rotation3d(const pixelpart::vec3_t& angle) {
	pixelpart::vec3_t rotation = glm::radians(angle);
	pixelpart::float_t cy = std::cos(rotation.y);
	pixelpart::float_t sy = std::sin(rotation.y);
	pixelpart::float_t cp = std::cos(rotation.z);
	pixelpart::float_t sp = std::sin(rotation.z);
	pixelpart::float_t cr = std::cos(rotation.x);
	pixelpart::float_t sr = std::sin(rotation.x);

	return pixelpart::mat3_t(
		pixelpart::vec3_t(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::vec3_t(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::vec3_t(sy * cp, -sp, cy * cp));
}
pixelpart::mat3_t lookAt(const pixelpart::vec3_t& direction) {
	pixelpart::vec3_t up = pixelpart::vec3_t(0.0, 1.0, 0.0);
	pixelpart::vec3_t front = glm::normalize(direction);
	pixelpart::vec3_t right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3_t(right, up, -front);
}
}