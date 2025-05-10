#ifndef PIXELPART_PARTICLE_MESH_DATA_H
#define PIXELPART_PARTICLE_MESH_DATA_H

#include "PixelpartTrailMeshData.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/engine/ParticleCollection.h"
#include <cstdint>
#include <vector>
#include <unordered_map>

struct PixelpartParticleMeshData {
	pixelpart::ParticleCollection sortedParticleCollection;
	std::vector<uint32_t> sortKeys;

	std::unordered_map<uint32_t, PixelpartTrailMeshData> trails;
};

#endif