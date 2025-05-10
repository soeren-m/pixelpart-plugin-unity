#include "PixelpartUtil.h"
#include <cmath>

namespace internal {
UnityInt toUnity(pixelpart::int_t v) {
	return static_cast<UnityInt>(v);
}
UnityFloat toUnity(pixelpart::float_t v) {
	return static_cast<UnityFloat>(v);
}
UnityVector2 toUnity(const pixelpart::float2_t& v) {
	return UnityVector2{
		static_cast<UnityFloat>(v.x),
		static_cast<UnityFloat>(v.y)
	};
}
UnityVector3 toUnity(const pixelpart::float3_t& v) {
	return UnityVector3{
		static_cast<UnityFloat>(v.x),
		static_cast<UnityFloat>(v.y),
		static_cast<UnityFloat>(v.z)
	};
}
UnityVector4 toUnity(const pixelpart::float3_t& v, pixelpart::float_t w) {
	return UnityVector4{
		static_cast<UnityFloat>(v.x),
		static_cast<UnityFloat>(v.y),
		static_cast<UnityFloat>(v.z),
		static_cast<UnityFloat>(w)
	};
}
UnityVector4 toUnity(const pixelpart::float4_t& v) {
	return UnityVector4{
		static_cast<UnityFloat>(v.x),
		static_cast<UnityFloat>(v.y),
		static_cast<UnityFloat>(v.z),
		static_cast<UnityFloat>(v.w)
	};
}
UnityColor toUnityColor(const pixelpart::float4_t& v) {
	return UnityColor{
		static_cast<UnityFloat>(v.x),
		static_cast<UnityFloat>(v.y),
		static_cast<UnityFloat>(v.z),
		static_cast<UnityFloat>(v.w)
	};
}

pixelpart::int_t fromUnity(UnityInt v) {
	return static_cast<pixelpart::int_t>(v);
}
pixelpart::float_t fromUnity(UnityFloat v) {
	return static_cast<pixelpart::float_t>(v);
}
pixelpart::float2_t fromUnity(const UnityVector2& v) {
	return pixelpart::float2_t(v.x, v.y);
}
pixelpart::float3_t fromUnity(const UnityVector3& v) {
	return pixelpart::float3_t(v.x, v.y, v.z);
}
pixelpart::float4_t fromUnity(const UnityVector4& v) {
	return pixelpart::float4_t(v.x, v.y, v.z, v.w);
}
pixelpart::float4_t fromUnity(const UnityColor& v) {
	return pixelpart::float4_t(v.r, v.g, v.b, v.a);
}

pixelpart::float3_t rotate2d(const pixelpart::float3_t& p, const pixelpart::float3_t& o, pixelpart::float_t a) {
	pixelpart::float_t s = std::sin(glm::radians(a));
	pixelpart::float_t c = std::cos(glm::radians(a));

	return pixelpart::float3_t(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3_t rotation3d(const pixelpart::float3_t& angle) {
	pixelpart::float3_t rotation = glm::radians(angle);
	pixelpart::float_t cy = std::cos(rotation.y);
	pixelpart::float_t sy = std::sin(rotation.y);
	pixelpart::float_t cp = std::cos(rotation.z);
	pixelpart::float_t sp = std::sin(rotation.z);
	pixelpart::float_t cr = std::cos(rotation.x);
	pixelpart::float_t sr = std::sin(rotation.x);

	return pixelpart::mat3_t(
		pixelpart::float3_t(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::float3_t(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::float3_t(sy * cp, -sp, cy * cp));
}
pixelpart::mat3_t lookAt(const pixelpart::float3_t& direction) {
	pixelpart::float3_t up = pixelpart::float3_t(0.0, 1.0, 0.0);
	pixelpart::float3_t front = glm::normalize(direction);
	pixelpart::float3_t right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3_t(right, up, -front);
}
}