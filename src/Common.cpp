#include "Common.h"

namespace pixelpart_unity {
int_t toUnity(pixelpart::int_t v) {
	return static_cast<int_t>(v);
}
float_t toUnity(pixelpart::float_t v) {
	return static_cast<float_t>(v);
}
vector2_t toUnity(const pixelpart::float2_t& v) {
	return vector2_t{
		static_cast<float_t>(v.x),
		static_cast<float_t>(v.y)
	};
}
vector3_t toUnity(const pixelpart::float3_t& v) {
	return vector3_t{
		static_cast<float_t>(v.x),
		static_cast<float_t>(v.y),
		static_cast<float_t>(v.z)
	};
}
vector4_t toUnity(const pixelpart::float3_t& v, pixelpart::float_t w) {
	return vector4_t{
		static_cast<float_t>(v.x),
		static_cast<float_t>(v.y),
		static_cast<float_t>(v.z),
		static_cast<float_t>(w)
	};
}
vector4_t toUnity(const pixelpart::float4_t& v) {
	return vector4_t{
		static_cast<float_t>(v.x),
		static_cast<float_t>(v.y),
		static_cast<float_t>(v.z),
		static_cast<float_t>(v.w)
	};
}
color_t toUnityColor(const pixelpart::float4_t& v) {
	return color_t{
		static_cast<float_t>(v.x),
		static_cast<float_t>(v.y),
		static_cast<float_t>(v.z),
		static_cast<float_t>(v.w)
	};
}

pixelpart::int_t fromUnity(int_t v) {
	return static_cast<pixelpart::int_t>(v);
}
pixelpart::float_t fromUnity(float_t v) {
	return static_cast<pixelpart::float_t>(v);
}
pixelpart::float2_t fromUnity(const vector2_t& v) {
	return pixelpart::float2_t(v.x, v.y);
}
pixelpart::float3_t fromUnity(const vector3_t& v) {
	return pixelpart::float3_t(v.x, v.y, v.z);
}
pixelpart::float4_t fromUnity(const vector4_t& v) {
	return pixelpart::float4_t(v.x, v.y, v.z, v.w);
}
pixelpart::float4_t fromUnity(const color_t& v) {
	return pixelpart::float4_t(v.r, v.g, v.b, v.a);
}
}
