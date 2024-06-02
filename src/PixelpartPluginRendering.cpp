#include "PixelpartPluginEffect.h"
#include "PixelpartPluginUtil.h"
#include "glm/glm/gtx/norm.hpp"
#include "glm/glm/gtx/vector_angle.hpp"

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetParticleTypesSortedForRendering(PixelpartPluginEffect* internalEffect, uint32_t* indices) {
	if(!internalEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = internalEffect->project.effect.particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	for(uint32_t i = 0u; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPrepareParticleSpriteVertexData(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex, int32_t* numTriangles, int32_t* numVertices) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsIndex(particleTypeIndex) || !numTriangles || !numVertices) {
		return;
	}

	if(particleTypeIndex > internalEffect->meshData.size()) {
		internalEffect->meshData.resize(particleTypeIndex + 1u);
	}

	uint32_t numParticles = internalEffect->particleEngine->getNumParticles(particleTypeIndex);
	*numTriangles = static_cast<int32_t>(numParticles) * 6 / 3;
	*numVertices = static_cast<int32_t>(numParticles) * 4;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPrepareParticleTrailVertexData(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex, int32_t* numTriangles, int32_t* numVertices) {
	const pixelpart::float_t epsilon = 0.000001;

	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsIndex(particleTypeIndex) || !numTriangles || !numVertices) {
		return;
	}

	if(particleTypeIndex > internalEffect->meshData.size()) {
		internalEffect->meshData.resize(particleTypeIndex + 1u);
	}

	const pixelpart::ParticleType& particleType = internalEffect->project.effect.particleTypes.getByIndex(particleTypeIndex);
	if(!internalEffect->project.effect.particleEmitters.contains(particleType.parentId)) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = internalEffect->project.effect.particleEmitters.get(particleType.parentId);
	pixelpart::ParticleReadPtr particles = internalEffect->particleEngine->getParticles(particleTypeIndex);
	uint32_t numParticles = internalEffect->particleEngine->getNumParticles(particleTypeIndex);

	*numTriangles = 0;
	*numVertices = 0;

	if(numParticles < 2u) {
		return;
	}

	std::vector<uint32_t> sortKeys(numParticles);
	std::iota(sortKeys.begin(), sortKeys.end(), 0u);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	PixelpartPluginEffect::MeshData& meshData = internalEffect->meshData[particleTypeIndex];
	std::unordered_map<uint32_t, PixelpartPluginEffect::ParticleTrail>& trails = meshData.trails;

	for(auto& entry : trails) {
		entry.second.numParticles = 0u;
	}

	PixelpartPluginEffect::ParticleTrail* currentTrail = nullptr;
	for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
		particleParentId = particles.parentId[sortKeys[p]];

		if(particleParentId != trailId) {
			trailId = particleParentId;
			currentTrail = &trails[trailId];
		}

		currentTrail->numParticles++;
	}

	for(auto it = trails.begin(); it != trails.end(); ) {
		if(it->second.numParticles == 0u) {
			it = trails.erase(it);
		}
		else {
			it++;
		}
	}

	currentTrail = nullptr;

	switch(particleType.trailRendererSettings.smoothingMethod) {
		case pixelpart::ParticleTrailRendererSettings::SmoothingMethod::spline: {
			for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					currentTrail = &trails.at(trailId);

					std::vector<pixelpart::float_t> relativeParticlePosition(currentTrail->numParticles);
					relativeParticlePosition[0u] = 0.0;

					currentTrail->length = 0.0;
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1u]]);
						relativeParticlePosition[q] = currentTrail->length;
					}
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						relativeParticlePosition[q] /= currentTrail->length;
					}

					pixelpart::Curve<pixelpart::vec3_t> positionCurve(pixelpart::CurveInterpolation::spline);
					positionCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3_t> sizeCurve(pixelpart::CurveInterpolation::spline);
					sizeCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec4_t> colorCurve(pixelpart::CurveInterpolation::spline);
					colorCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3_t> velocityCurve(pixelpart::CurveInterpolation::spline);
					velocityCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::float_t> lifeCurve(pixelpart::CurveInterpolation::spline);
					lifeCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life, sortKeys.data() + p, currentTrail->numParticles);

					currentTrail->position = positionCurve.getCache();
					currentTrail->size = sizeCurve.getCache();
					currentTrail->color = colorCurve.getCache();
					currentTrail->velocity = velocityCurve.getCache();
					currentTrail->life = lifeCurve.getCache();
					currentTrail->direction.resize(particleType.trailRendererSettings.numSmoothingSegments);
					currentTrail->directionToEdge.resize(particleType.trailRendererSettings.numSmoothingSegments);
					currentTrail->index.resize(particleType.trailRendererSettings.numSmoothingSegments);
				}
			}

			break;
		}

		default: {
			for(uint32_t p = 0u, q = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++, q++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					q = 0u;

					currentTrail = &trails.at(trailId);
					currentTrail->position.resize(currentTrail->numParticles);
					currentTrail->size.resize(currentTrail->numParticles);
					currentTrail->color.resize(currentTrail->numParticles);
					currentTrail->velocity.resize(currentTrail->numParticles);
					currentTrail->life.resize(currentTrail->numParticles);
					currentTrail->direction.resize(currentTrail->numParticles);
					currentTrail->directionToEdge.resize(currentTrail->numParticles);
					currentTrail->index.resize(currentTrail->numParticles);
				}

				currentTrail->position[q] = particles.globalPosition[particleIndex];
				currentTrail->size[q] = particles.size[particleIndex];
				currentTrail->color[q] = particles.color[particleIndex];
				currentTrail->velocity[q] = particles.velocity[particleIndex];
				currentTrail->life[q] = particles.life[particleIndex];
			}

			break;
		}
	}

	for(auto& entry : trails) {
		PixelpartPluginEffect::ParticleTrail& trail = entry.second;
		trail.length = 0.0;

		if(trail.position.size() < 2u) {
			continue;
		}

		std::size_t last = trail.position.size() - 1u;
		for(std::size_t i = 0u; i < last; i++) {
			pixelpart::vec3_t toNext = trail.position[i + 1u] - trail.position[i];
			pixelpart::float_t distance = std::max(glm::length(toNext), epsilon);

			trail.direction[i] = toNext / distance;
			trail.index[i] = trail.length;
			trail.length += distance;
		}

		trail.direction[last] = trail.direction[last - 1u];
		trail.index[last] = trail.length;

		for(std::size_t i = last; i > 0u; i--) {
			pixelpart::vec3_t toEdge = trail.direction[i] - trail.direction[i - 1u];
			pixelpart::float_t toEdgeLength = glm::length(toEdge);
			if(toEdgeLength < epsilon) {
				trail.directionToEdge[i] = pixelpart::vec3_t(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.directionToEdge[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.directionToEdge[0u] = trail.directionToEdge[1u];

		for(pixelpart::float_t& index : trail.index) {
			index /= trail.length;
		}

		*numTriangles += (static_cast<int32_t>(trail.position.size()) - 1) * 12 / 3;
		*numVertices += (static_cast<int32_t>(trail.position.size()) - 1) * 5;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetParticleSpriteVertexData(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
	int32_t* triangles, Vector3* vertices, Color* colors, Vector3* normals, Vector2* uv0, Vector4* uv1, Vector4* uv2) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsIndex(particleTypeIndex)) {
		return;
	}

	const pixelpart::Effect& effect = internalEffect->project.effect;
	const pixelpart::ParticleType& particleType = effect.particleTypes.getByIndex(particleTypeIndex);
	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::ParticleReadPtr particles = internalEffect->particleEngine->getParticles(particleTypeIndex);
	PixelpartPluginEffect::MeshData& meshData = internalEffect->meshData[particleTypeIndex];
	uint32_t numParticles = internalEffect->particleEngine->getNumParticles(particleTypeIndex);
	pixelpart::float_t alpha = std::fmod(internalEffect->particleEngine->getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	pixelpart::vec3_t effectScale = util::fromUnity(scale);
	pixelpart::vec3_t cameraRight = util::fromUnity(viewRight);
	pixelpart::vec3_t cameraUp = util::fromUnity(viewUp);

	for(int32_t p = 0; p < static_cast<int32_t>(numParticles); p++) {
		triangles[p * 6 + 0] = p * 4 + 0;
		triangles[p * 6 + 1] = p * 4 + 1;
		triangles[p * 6 + 2] = p * 4 + 3;
		triangles[p * 6 + 3] = p * 4 + 1;
		triangles[p * 6 + 4] = p * 4 + 2;
		triangles[p * 6 + 5] = p * 4 + 3;
	}

	pixelpart::ParticleReadPtr particleRenderData = particles;

	if(effect.is3d) {
		if(particleType.spriteRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none && numParticles > 1u) {
			if(numParticles > meshData.sortedParticleContainer.getParticleCapacity()) {
				meshData.sortedParticleContainer = pixelpart::ParticleContainer(numParticles);
				meshData.sortKeys.resize(numParticles);
			}

			std::iota(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles, 0);

			switch(particleType.spriteRendererSettings.sortCriterion) {
				case pixelpart::ParticleSortCriterion::age: {
#ifndef __EMSCRIPTEN__
					if(PixelpartPluginEffect::threadPool) {
						util::parallelInsertionSort(*PixelpartPluginEffect::threadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
							[&particles](uint32_t i, uint32_t j) {
								return particles.id[i] < particles.id[j];
							});

						break;
					}
#endif

					util::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
						[&particles](uint32_t i, uint32_t j) {
							return particles.id[i] < particles.id[j];
						});

					break;
				}

				case pixelpart::ParticleSortCriterion::distance: {
					pixelpart::vec3_t cameraPos = pixelpart::vec3_t(cameraPosition.x, cameraPosition.y, cameraPosition.z);

#ifndef __EMSCRIPTEN__
					if(PixelpartPluginEffect::threadPool) {
						util::parallelInsertionSort(*PixelpartPluginEffect::threadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
							[&particles, cameraPos](uint32_t i, uint32_t j) {
								return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
							});

						break;
					}
#endif

					util::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
						[&particles, cameraPos](uint32_t i, uint32_t j) {
							return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
						});

					break;
				}
			}

			pixelpart::ParticleWritePtr sortedParticleData = meshData.sortedParticleContainer.getParticleWritePtr();

			for(uint32_t i = 0u; i < numParticles; i++) {
				uint32_t j = meshData.sortKeys[i];

				sortedParticleData.id[i] = particles.id[j];
				sortedParticleData.life[i] = particles.life[j];
				sortedParticleData.globalPosition[i] = particles.globalPosition[j];
				sortedParticleData.velocity[i] = particles.velocity[j];
				sortedParticleData.force[i] = particles.force[j];
				sortedParticleData.rotation[i] = particles.rotation[j];
				sortedParticleData.size[i] = particles.size[j];
				sortedParticleData.color[i] = particles.color[j];
			}

			particleRenderData = meshData.sortedParticleContainer.getParticleReadPtr();
		}

		for(uint32_t p = 0u; p < numParticles; p++) {
			pixelpart::mat3_t rotationMatrix = util::rotation3d(particleRenderData.rotation[p]);
			pixelpart::vec3_t pivot = particleType.pivot.get() * particleRenderData.size[p];
			pixelpart::vec3_t position[4] = {
				(rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
				(rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
				(rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
				(rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale };

			pixelpart::vec3_t normal = rotationMatrix * pixelpart::vec3_t(0.0, 0.0, -1.0);

			switch(particleType.alignmentMode) {
				case pixelpart::AlignmentMode::camera: {
					position[0] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[0].x + cameraUp * position[0].y;
					position[1] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[1].x + cameraUp * position[1].y;
					position[2] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[2].x + cameraUp * position[2].y;
					position[3] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[3].x + cameraUp * position[3].y;
					break;
				}
				case pixelpart::AlignmentMode::motion: {
					pixelpart::mat3_t lookAtMatrix = util::lookAt(particleRenderData.velocity[p] * effectScale);
					position[0] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[0];
					position[1] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[1];
					position[2] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[2];
					position[3] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[3];
					break;
				}
				case pixelpart::AlignmentMode::emission: {
					pixelpart::mat3_t lookAtMatrix = util::lookAt(particleEmitter.position.get(alpha) - particleRenderData.globalPosition[p]);
					position[0] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[0];
					position[1] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[1];
					position[2] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[2];
					position[3] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[3];
					break;
				}
				case pixelpart::AlignmentMode::emitter: {
					pixelpart::mat3_t lookAtMatrix = util::rotation3d(particleEmitter.orientation.get(alpha));
					position[0] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[0];
					position[1] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[1];
					position[2] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[2];
					position[3] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[3];
					break;
				}
				default: {
					position[0] = particleRenderData.globalPosition[p] * effectScale + position[0];
					position[1] = particleRenderData.globalPosition[p] * effectScale + position[1];
					position[2] = particleRenderData.globalPosition[p] * effectScale + position[2];
					position[3] = particleRenderData.globalPosition[p] * effectScale + position[3];
					break;
				}
			}

			vertices[p * 4 + 0] = util::toUnity(position[0]);
			vertices[p * 4 + 1] = util::toUnity(position[1]);
			vertices[p * 4 + 2] = util::toUnity(position[2]);
			vertices[p * 4 + 3] = util::toUnity(position[3]);

			normals[p * 4 + 0] = util::toUnity(normal);
			normals[p * 4 + 1] = util::toUnity(normal);
			normals[p * 4 + 2] = util::toUnity(normal);
			normals[p * 4 + 3] = util::toUnity(normal);
		}
	}
	else {
		pixelpart::float_t zOffset = -0.001 * static_cast<pixelpart::float_t>(particleType.layer);

		Vector3 normal = Vector3{ 0.0f, 0.0f, -1.0f };

		for(uint32_t p = 0u; p < numParticles; p++) {
			pixelpart::vec3_t position[4];
			pixelpart::vec3_t pivot = particleType.pivot.get() * particles.size[p];

			switch(particleType.alignmentMode) {
				case pixelpart::AlignmentMode::motion: {
					pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::vec2_t(0.0, 1.0), (particles.velocity[p] != pixelpart::vec3_t(0.0))
						? pixelpart::vec2_t(glm::normalize(particles.velocity[p]))
						: pixelpart::vec2_t(0.0, 1.0)));
					position[0] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[1] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[2] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[3] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					break;
				}
				case pixelpart::AlignmentMode::emission: {
					pixelpart::vec3_t emissionDirection = particleEmitter.position.get(alpha) - particles.globalPosition[p];
					pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::vec2_t(0.0, 1.0), (emissionDirection != pixelpart::vec3_t(0.0))
							? pixelpart::vec2_t(glm::normalize(emissionDirection))
							: pixelpart::vec2_t(0.0, 1.0)));
					position[0] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[1] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[2] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					position[3] = particles.globalPosition[p] + util::rotate2d(pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
					break;
				}
				case pixelpart::AlignmentMode::emitter: {
					pixelpart::mat3_t rotationMatrix = util::rotation3d(particles.rotation[p]);
					pixelpart::mat3_t lookAtMatrix = util::rotation3d(particleEmitter.orientation.get(alpha));
					position[0] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[1] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[2] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[3] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
					break;
				}
				default: {
					pixelpart::mat3_t rotationMatrix = util::rotation3d(particles.rotation[p]);
					position[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
					position[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
					break;
				}
			}

			position[0] *= effectScale;
			position[1] *= effectScale;
			position[2] *= effectScale;
			position[3] *= effectScale;
			position[0].z += zOffset;
			position[1].z += zOffset;
			position[2].z += zOffset;
			position[3].z += zOffset;

			vertices[p * 4 + 0] = util::toUnity(position[0]);
			vertices[p * 4 + 1] = util::toUnity(position[1]);
			vertices[p * 4 + 2] = util::toUnity(position[2]);
			vertices[p * 4 + 3] = util::toUnity(position[3]);

			normals[p * 4 + 0] = normal;
			normals[p * 4 + 1] = normal;
			normals[p * 4 + 2] = normal;
			normals[p * 4 + 3] = normal;
		}
	}

	for(uint32_t p = 0u; p < numParticles; p++) {
		colors[p * 4 + 0] = util::toUnityColor(particleRenderData.color[p]);
		colors[p * 4 + 1] = util::toUnityColor(particleRenderData.color[p]);
		colors[p * 4 + 2] = util::toUnityColor(particleRenderData.color[p]);
		colors[p * 4 + 3] = util::toUnityColor(particleRenderData.color[p]);
	}

	for(uint32_t p = 0u; p < numParticles; p++) {
		uv0[p * 4 + 0] = Vector2{ 0.0f, 0.0f };
		uv0[p * 4 + 1] = Vector2{ 1.0f, 0.0f };
		uv0[p * 4 + 2] = Vector2{ 1.0f, 1.0f };
		uv0[p * 4 + 3] = Vector2{ 0.0f, 1.0f };
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		uv1[p * 4 + 0] = util::toUnity(particleRenderData.velocity[p], 0.0);
		uv1[p * 4 + 1] = util::toUnity(particleRenderData.velocity[p], 0.0);
		uv1[p * 4 + 2] = util::toUnity(particleRenderData.velocity[p], 0.0);
		uv1[p * 4 + 3] = util::toUnity(particleRenderData.velocity[p], 0.0);
	}

	for(uint32_t p = 0u; p < numParticles; p++) {
		uv2[p * 4 + 0] = Vector4{ util::toUnity(particleRenderData.life[p]), static_cast<float>(particleRenderData.id[p]), 0.0f, 0.0f };
		uv2[p * 4 + 1] = Vector4{ util::toUnity(particleRenderData.life[p]), static_cast<float>(particleRenderData.id[p]), 0.0f, 0.0f };
		uv2[p * 4 + 2] = Vector4{ util::toUnity(particleRenderData.life[p]), static_cast<float>(particleRenderData.id[p]), 0.0f, 0.0f };
		uv2[p * 4 + 3] = Vector4{ util::toUnity(particleRenderData.life[p]), static_cast<float>(particleRenderData.id[p]), 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetParticleTrailVertexData(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
	int32_t* triangles, Vector3* vertices, Color* colors, Vector3* normals, Vector2* uv0, Vector4* uv1, Vector4* uv2) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsIndex(particleTypeIndex)) {
		return false;
	}

	const pixelpart::Effect& effect = internalEffect->project.effect;
	const pixelpart::ParticleType& particleType = effect.particleTypes.getByIndex(particleTypeIndex);
	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::ParticleReadPtr particles = internalEffect->particleEngine->getParticles(particleTypeIndex);
	PixelpartPluginEffect::MeshData& meshData = internalEffect->meshData[particleTypeIndex];
	uint32_t numParticles = internalEffect->particleEngine->getNumParticles(particleTypeIndex);
	pixelpart::float_t alpha = std::fmod(internalEffect->particleEngine->getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	pixelpart::vec3_t effectScale = util::fromUnity(scale);
	pixelpart::vec3_t cameraPos = util::fromUnity(cameraPosition);
	pixelpart::vec3_t cameraRight = util::fromUnity(viewRight);
	pixelpart::vec3_t cameraUp = util::fromUnity(viewUp);

	if(numParticles < 2u) {
		return true;
	}

	int32_t elementIndex = 0;
	int32_t vertexIndex = 0;

	pixelpart::float_t zOffset = !effect.is3d
		? static_cast<pixelpart::float_t>(particleType.layer) * -0.001
		: 0.0;

	for(const auto& entry : meshData.trails) {
		const PixelpartPluginEffect::ParticleTrail& trail = entry.second;
		if(trail.position.size() < 2u) {
			continue;
		}

		int32_t numTrailSegments = static_cast<int32_t>(trail.position.size()) - 1;

		for(int32_t p = 0; p < numTrailSegments; p++) {
			triangles[elementIndex + p * 12 + 0] = vertexIndex + p * 5 + 0;
			triangles[elementIndex + p * 12 + 1] = vertexIndex + p * 5 + 1;
			triangles[elementIndex + p * 12 + 2] = vertexIndex + p * 5 + 4;
			triangles[elementIndex + p * 12 + 3] = vertexIndex + p * 5 + 2;
			triangles[elementIndex + p * 12 + 4] = vertexIndex + p * 5 + 0;
			triangles[elementIndex + p * 12 + 5] = vertexIndex + p * 5 + 4;
			triangles[elementIndex + p * 12 + 6] = vertexIndex + p * 5 + 1;
			triangles[elementIndex + p * 12 + 7] = vertexIndex + p * 5 + 3;
			triangles[elementIndex + p * 12 + 8] = vertexIndex + p * 5 + 4;
			triangles[elementIndex + p * 12 + 9] = vertexIndex + p * 5 + 3;
			triangles[elementIndex + p * 12 + 10] = vertexIndex + p * 5 + 2;
			triangles[elementIndex + p * 12 + 11] = vertexIndex + p * 5 + 4;
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			pixelpart::vec3_t startToEdgeDirection = trail.directionToEdge[p];
			pixelpart::vec3_t endToEdgeDirection = trail.directionToEdge[p + 1];

			if(particleType.alignmentMode == pixelpart::AlignmentMode::camera) {
				pixelpart::vec3_t cameraToStart = glm::normalize(cameraPos - trail.position[p]);
				pixelpart::vec3_t cameraToEnd = glm::normalize(cameraPos - trail.position[p + 1]);
				startToEdgeDirection = glm::normalize(glm::cross(cameraToStart, trail.direction[p]));
				endToEdgeDirection = glm::normalize(glm::cross(cameraToEnd, trail.direction[p + 1]));
			}

			pixelpart::vec3_t startToEdge = startToEdgeDirection * std::max(trail.size[p].x, std::max(trail.size[p].y, trail.size[p].z)) * 0.5;
			pixelpart::vec3_t endToEdge = endToEdgeDirection * std::max(trail.size[p + 1].x, std::max(trail.size[p + 1].y, trail.size[p + 1].z)) * 0.5;

			pixelpart::vec3_t vertexPositions[5] = {
				(trail.position[p] + startToEdge) * effectScale + pixelpart::vec3_t(0.0, 0.0, zOffset),
				(trail.position[p] - startToEdge) * effectScale + pixelpart::vec3_t(0.0, 0.0, zOffset),
				(trail.position[p + 1] + endToEdge) * effectScale + pixelpart::vec3_t(0.0, 0.0, zOffset),
				(trail.position[p + 1] - endToEdge) * effectScale + pixelpart::vec3_t(0.0, 0.0, zOffset),
				(trail.position[p] + trail.position[p + 1]) * 0.5 * effectScale + pixelpart::vec3_t(0.0, 0.0, zOffset)
			};

			vertices[vertexIndex + p * 5 + 0] = util::toUnity(vertexPositions[0]);
			vertices[vertexIndex + p * 5 + 1] = util::toUnity(vertexPositions[1]);
			vertices[vertexIndex + p * 5 + 2] = util::toUnity(vertexPositions[2]);
			vertices[vertexIndex + p * 5 + 3] = util::toUnity(vertexPositions[3]);
			vertices[vertexIndex + p * 5 + 4] = util::toUnity(vertexPositions[4]);

			pixelpart::vec3_t vertexNormalStart = glm::cross(trail.direction[p], startToEdgeDirection);
			pixelpart::vec3_t vertexNormalEnd = glm::cross(trail.direction[p + 1], endToEdgeDirection);

			normals[vertexIndex + p * 5 + 0] = util::toUnity(vertexNormalStart);
			normals[vertexIndex + p * 5 + 1] = util::toUnity(vertexNormalStart);
			normals[vertexIndex + p * 5 + 2] = util::toUnity(vertexNormalEnd);
			normals[vertexIndex + p * 5 + 3] = util::toUnity(vertexNormalEnd);
			normals[vertexIndex + p * 5 + 4] = util::toUnity((vertexNormalStart + vertexNormalEnd) * 0.5);
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			colors[vertexIndex + p * 5 + 0] = util::toUnityColor(trail.color[p]);
			colors[vertexIndex + p * 5 + 1] = util::toUnityColor(trail.color[p]);
			colors[vertexIndex + p * 5 + 2] = util::toUnityColor(trail.color[p + 1]);
			colors[vertexIndex + p * 5 + 3] = util::toUnityColor(trail.color[p + 1]);
			colors[vertexIndex + p * 5 + 4] = util::toUnityColor((trail.color[p] + trail.color[p + 1]) * 0.5);
		}

		switch(particleType.trailRendererSettings.textureRotation) {
			case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					uv0[vertexIndex + p * 5 + 0] = Vector2{ util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					uv0[vertexIndex + p * 5 + 1] = Vector2{ util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
					uv0[vertexIndex + p * 5 + 2] = Vector2{ util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					uv0[vertexIndex + p * 5 + 3] = Vector2{ util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
					uv0[vertexIndex + p * 5 + 4] = Vector2{ util::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor), 0.5f };
				}
				break;
			case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					uv0[vertexIndex + p * 5 + 0] = Vector2{ 1.0f, 1.0f - util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 1] = Vector2{ 0.0f, 1.0f - util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 2] = Vector2{ 1.0f, 1.0f - util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 3] = Vector2{ 0.0f, 1.0f - util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 4] = Vector2{ 0.5f, 1.0f - util::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor) };
				}
				break;
			case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					uv0[vertexIndex + p * 5 + 0] = Vector2{ 1.0f - util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
					uv0[vertexIndex + p * 5 + 1] = Vector2{ 1.0f - util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					uv0[vertexIndex + p * 5 + 2] = Vector2{ 1.0f - util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
					uv0[vertexIndex + p * 5 + 3] = Vector2{ 1.0f - util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					uv0[vertexIndex + p * 5 + 4] = Vector2{ 1.0f - util::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor), 0.5f };
				}
				break;
			default:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					uv0[vertexIndex + p * 5 + 0] = Vector2{ 0.0f, util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 1] = Vector2{ 1.0f, util::toUnity(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 2] = Vector2{ 0.0f, util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 3] = Vector2{ 1.0f, util::toUnity(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					uv0[vertexIndex + p * 5 + 4] = Vector2{ 0.5f, util::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor) };
				}
				break;
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			uv1[vertexIndex + p * 5 + 0] = util::toUnity(trail.velocity[p], 0.0);
			uv1[vertexIndex + p * 5 + 1] = util::toUnity(trail.velocity[p], 0.0);
			uv1[vertexIndex + p * 5 + 2] = util::toUnity(trail.velocity[p + 1], 0.0);
			uv1[vertexIndex + p * 5 + 3] = util::toUnity(trail.velocity[p + 1], 0.0);
			uv1[vertexIndex + p * 5 + 4] = util::toUnity((trail.velocity[p] + trail.velocity[p + 1]) * 0.5, 0.0);
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			uv2[vertexIndex + p * 5 + 0] = Vector4{ util::toUnity(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
			uv2[vertexIndex + p * 5 + 1] = Vector4{ util::toUnity(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
			uv2[vertexIndex + p * 5 + 2] = Vector4{ util::toUnity(trail.life[p + 1]), static_cast<float>(entry.first), 0.0f, 0.0f };
			uv2[vertexIndex + p * 5 + 3] = Vector4{ util::toUnity(trail.life[p + 1]), static_cast<float>(entry.first), 0.0f, 0.0f };
			uv2[vertexIndex + p * 5 + 4] = Vector4{ util::toUnity(trail.life[p] + trail.life[p + 1]) * 0.5f, static_cast<float>(entry.first), 0.0f, 0.0f };
		}

		elementIndex += numTrailSegments * 12;
		vertexIndex += numTrailSegments * 5;
	}

	return true;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetParticleMeshInstanceData(PixelpartPluginEffect* internalEffect, uint32_t particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
	Matrix4x4* transforms, Vector4* colors, Vector4* velocities, float* lives, float* ids) {
	if(!internalEffect || !internalEffect->project.effect.particleTypes.containsIndex(particleTypeIndex)) {
		return false;
	}

	const pixelpart::Effect& effect = internalEffect->project.effect;
	const pixelpart::ParticleType& particleType = effect.particleTypes.getByIndex(particleTypeIndex);
	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::ParticleReadPtr particles = internalEffect->particleEngine->getParticles(particleTypeIndex);
	PixelpartPluginEffect::MeshData& meshData = internalEffect->meshData[particleTypeIndex];
	uint32_t numParticles = internalEffect->particleEngine->getNumParticles(particleTypeIndex);
	pixelpart::float_t alpha = std::fmod(internalEffect->particleEngine->getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	pixelpart::vec3_t effectScale = pixelpart::vec3_t(scale.x, scale.y, scale.z);
	pixelpart::vec3_t cameraRight = pixelpart::vec3_t(viewRight.x, viewRight.y, viewRight.z);
	pixelpart::vec3_t cameraUp = pixelpart::vec3_t(viewUp.x, viewUp.y, viewUp.z);

	pixelpart::ParticleReadPtr particleRenderData = particles;

	if(effect.is3d) {
		if(particleType.spriteRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none && numParticles > 1u) {
			if(numParticles > meshData.sortedParticleContainer.getParticleCapacity()) {
				meshData.sortedParticleContainer = pixelpart::ParticleContainer(numParticles);
				meshData.sortKeys.resize(numParticles);
			}

			std::iota(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles, 0);

			switch(particleType.spriteRendererSettings.sortCriterion) {
				case pixelpart::ParticleSortCriterion::age: {
#ifndef __EMSCRIPTEN__
					if(PixelpartPluginEffect::threadPool) {
						util::parallelInsertionSort(*PixelpartPluginEffect::threadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
							[&particles](uint32_t i, uint32_t j) {
								return particles.id[i] < particles.id[j];
							});

						break;
					}
#endif

					util::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
						[&particles](uint32_t i, uint32_t j) {
							return particles.id[i] < particles.id[j];
						});

					break;
				}

				case pixelpart::ParticleSortCriterion::distance: {
					pixelpart::vec3_t cameraPos = pixelpart::vec3_t(cameraPosition.x, cameraPosition.y, cameraPosition.z);

#ifndef __EMSCRIPTEN__
					if(PixelpartPluginEffect::threadPool) {
						util::parallelInsertionSort(*PixelpartPluginEffect::threadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
							[&particles, cameraPos](uint32_t i, uint32_t j) {
								return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
							});

						break;
					}
#endif

					util::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
						[&particles, cameraPos](uint32_t i, uint32_t j) {
							return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
						});

					break;
				}
			}

			pixelpart::ParticleWritePtr sortedParticleData = meshData.sortedParticleContainer.getParticleWritePtr();

			for(uint32_t i = 0u; i < numParticles; i++) {
				uint32_t j = meshData.sortKeys[i];

				sortedParticleData.id[i] = particles.id[j];
				sortedParticleData.life[i] = particles.life[j];
				sortedParticleData.globalPosition[i] = particles.globalPosition[j];
				sortedParticleData.velocity[i] = particles.velocity[j];
				sortedParticleData.force[i] = particles.force[j];
				sortedParticleData.rotation[i] = particles.rotation[j];
				sortedParticleData.size[i] = particles.size[j];
				sortedParticleData.color[i] = particles.color[j];
			}

			particleRenderData = meshData.sortedParticleContainer.getParticleReadPtr();
		}
	}

	for(uint32_t p = 0u; p < numParticles; p++) {
		pixelpart::mat4_t globalScaleMatrix = glm::scale(pixelpart::vec3_t(effectScale));

		pixelpart::mat4_t alignmentMatrix = pixelpart::mat4_t(1.0);
		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::motion:
				alignmentMatrix = pixelpart::mat4_t(util::lookAt(particleRenderData.velocity[p]));
				break;
			case pixelpart::AlignmentMode::emission:
				alignmentMatrix = pixelpart::mat4_t(util::lookAt(particleEmitter.position.get(alpha) - particleRenderData.globalPosition[p]));
				break;
			case pixelpart::AlignmentMode::emitter:
				alignmentMatrix = pixelpart::mat4_t(util::rotation3d(particleEmitter.orientation.get(alpha)));
				break;
			default:
				break;
		}

		pixelpart::vec3_t pivot = particleType.pivot.get() * particleRenderData.size[p];
		pixelpart::mat4_t rotationMatrix =
			glm::translate(pivot) *
			pixelpart::mat4_t(util::rotation3d(particleRenderData.rotation[p])) *
			glm::translate(-pivot);

		pixelpart::mat4_t translationMatrix = glm::translate(particleRenderData.globalPosition[p]);
		pixelpart::mat4_t scaleMatrix = glm::scale(particleRenderData.size[p]);

		glm::mat4 transformationMatrix =
			globalScaleMatrix * translationMatrix * alignmentMatrix * rotationMatrix * scaleMatrix;

		std::memcpy(transforms[p].data, &transformationMatrix[0][0], sizeof(float) * 16);

		colors[p] = util::toUnity(particleRenderData.color[p]);
		velocities[p] = util::toUnity(particleRenderData.velocity[p], 0.0);
		lives[p] = util::toUnity(particleRenderData.life[p]);
		ids[p] = static_cast<float>(particleRenderData.id[p]);
	}

	return true;
}
}