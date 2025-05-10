#ifndef PIXELPART_TRAIL_MESH_DATA_H
#define PIXELPART_TRAIL_MESH_DATA_H

#include "pixelpart-runtime/common/Math.h"
#include <cstdint>
#include <vector>

struct PixelpartTrailMeshData {
	uint32_t particleCount = 0;
	pixelpart::float_t length = 0.0;

	std::vector<pixelpart::float3_t> position;
	std::vector<pixelpart::float3_t> size;
	std::vector<pixelpart::float4_t> color;
	std::vector<pixelpart::float3_t> velocity;
	std::vector<pixelpart::float_t> life;
	std::vector<pixelpart::float3_t> direction;
	std::vector<pixelpart::float3_t> directionToEdge;
	std::vector<pixelpart::float_t> index;
};

#endif