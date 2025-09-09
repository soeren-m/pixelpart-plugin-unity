#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "ParticleMeshData.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/Transform.h"
#include "pixelpart-runtime/common/Curve.h"
#include "pixelpart-runtime/common/Id.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include "pixelpart-runtime/effect/ParticleType.h"
#include "pixelpart-runtime/effect/Effect.h"
#include "pixelpart-runtime/effect/ParticleRuntimeId.h"
#include "pixelpart-runtime/engine/ParticleCollection.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "pixelpart-runtime/glm/gtx/norm.hpp"
#include "pixelpart-runtime/glm/gtx/vector_angle.hpp"
#include <cstdint>
#include <cstring>
#include <cmath>
#include <utility>
#include <vector>
#include <numeric>
#include <exception>
#include <algorithm>

namespace pixelpart_unity {
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

template <typename T>
std::vector<typename pixelpart::Curve<T>::Point> orderedPoints(const pixelpart::float_t* positionList, const T* valueList, const std::uint32_t* order, std::uint32_t count) {
	std::vector<typename pixelpart::Curve<T>::Point> result;
	result.resize(count);
	for(uint32_t index = 0; index < count; index++) {
		result[index] = typename pixelpart::Curve<T>::Point{ positionList[index], valueList[order[index]] };
	}

	return result;
}

template<typename It, typename Comp>
std::vector<typename It::value_type> merge(It begin, It mid, It end, Comp comp) {
	std::vector<typename It::value_type> result;
	result.reserve(std::distance(begin, end));

	It left = begin;
	It right = mid;

	while(left != mid && right != end) {
		result.push_back(comp(*left, *right)
			? *left++
			: *right++);
	}

	result.insert(result.end(), left, mid);
	result.insert(result.end(), right, end);

	return result;
}

template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void insertionSort(It begin, It end, Comp comp = Comp()) {
	std::iter_swap(begin, std::min_element(begin, end, comp));

	for(It i = begin; ++i < end; begin = i) {
		for(It j = i; comp(*j, *begin); --j, --begin) {
			std::iter_swap(begin, j);
		}
	}
}

#ifdef PIXELPART_RUNTIME_MULTITHREADING
template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void parallelInsertionSort(pixelpart::ThreadPool& threadPool, std::size_t minWorkSize, It begin, It end, Comp comp = Comp()) {
	std::size_t length = std::distance(begin, end);
	if(length < 2) {
		return;
	}

	std::size_t threadCount = std::min(length / std::max(minWorkSize, static_cast<std::size_t>(1)), threadPool.threadCount());
	if(threadCount <= 1) {
		insertionSort(begin, end, comp);
		return;
	}

	std::vector<std::pair<std::size_t, std::size_t>> workgroups;
	std::size_t threadCountWithLargerLoad = length % threadCount;
	std::size_t workSize = length / threadCount;
	std::size_t workgroupStart = 0;

	for(std::size_t t = 0; t < threadCount; t++) {
		std::size_t workgroupSize = (t < threadCount - threadCountWithLargerLoad) ? workSize : workSize + 1;
		std::size_t workgroupEnd = workgroupStart + workgroupSize;

		threadPool.enqueue(static_cast<std::uint32_t>(t),
			insertionSort<It, Comp>,
			std::next(begin, workgroupStart),
			std::next(begin, workgroupEnd),
			comp);

		workgroups.emplace_back(std::make_pair(workgroupStart, workgroupEnd));
		workgroupStart += workgroupSize;
	}

	for(std::size_t t = 0; t < threadCount; t++) {
		threadPool.wait(static_cast<std::uint32_t>(t));
	}

	for(std::size_t t = 1; t < threadCount; t++) {
		auto&& result = merge(
			begin,
			std::next(begin, workgroups[t].first),
			std::next(begin, workgroups[t].second),
			comp);

		std::move(result.cbegin(), result.cend(), begin);
	}
}
#endif
}

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetSortedParticleRuntimeInstances(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::int_t* indices) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();

	std::vector<pixelpart_unity::int_t> sortedIndices(effect.particleRuntimeIds().size());
	std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
	std::sort(sortedIndices.begin(), sortedIndices.end(), [&effect](pixelpart_unity::int_t i1, pixelpart_unity::int_t i2) {
		const pixelpart::ParticleType& pt1 = effect.particleTypes().at(effect.particleRuntimeIds()[i1].typeId);
		const pixelpart::ParticleType& pt2 = effect.particleTypes().at(effect.particleRuntimeIds()[i2].typeId);

		return pt1.layer() < pt2.layer();
	});

	std::memcpy(indices, sortedIndices.data(), sortedIndices.size() * sizeof(pixelpart_unity::int_t));
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPrepareParticleSpriteVertexData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t* triangleCount, pixelpart_unity::int_t* vertexCount) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(!triangleCount || !vertexCount) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	std::uint32_t particleCount = effectRuntime->effectEngine->particleCount(
		pixelpart::id_t(particleEmitterId),
		pixelpart::id_t(particleTypeId));

	*triangleCount = static_cast<pixelpart_unity::int_t>(particleCount * 6 / 3);
	*vertexCount = static_cast<pixelpart_unity::int_t>(particleCount * 4);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPrepareParticleTrailVertexData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId, pixelpart_unity::int_t* triangleCount, pixelpart_unity::int_t* vertexCount) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(!triangleCount || !vertexCount) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		const pixelpart::float_t epsilon = 0.000001;

		*triangleCount = 0;
		*vertexCount = 0;

		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
		const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(pixelpart::id_t(particleEmitterId));
		const pixelpart::ParticleType& particleType = effect.particleTypes().at(pixelpart::id_t(particleTypeId));
		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		if(!particleCollection || particleCollection->count() < 2) {
			return;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		std::vector<std::uint32_t> sortKeys(particleCount);
		std::iota(sortKeys.begin(), sortKeys.end(), 0);
		std::sort(sortKeys.begin(), sortKeys.end(), [&particles](std::uint32_t p0, std::uint32_t p1) {
			return (particles.parentId[p0] != particles.parentId[p1])
				? particles.parentId[p0] > particles.parentId[p1]
				: particles.id[p0] > particles.id[p1];
		});

		pixelpart_unity::ParticleMeshData& meshData = effectRuntime->meshData[runtimeId];
		std::unordered_map<std::uint32_t, pixelpart_unity::TrailMeshData>& trails = meshData.trails;

		for(auto& entry : trails) {
			entry.second.particleCount = 0;
		}

		pixelpart_unity::TrailMeshData* currentTrail = nullptr;
		for(std::uint32_t p = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++) {
			particleParentId = particles.parentId[sortKeys[p]];

			if(particleParentId != trailId) {
				trailId = particleParentId;
				currentTrail = &trails[trailId];
			}

			currentTrail->particleCount++;
		}

		for(auto it = trails.begin(); it != trails.end(); ) {
			if(it->second.particleCount == 0) {
				it = trails.erase(it);
			}
			else {
				it++;
			}
		}

		currentTrail = nullptr;

		switch(particleType.trailRendererSettings().smoothingMethod) {
			case pixelpart::ParticleTrailRendererSettings::SmoothingMethod::spline: {
				for(std::uint32_t p = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++) {
					std::uint32_t particleIndex = sortKeys[p];
					particleParentId = particles.parentId[particleIndex];

					if(particleParentId != trailId) {
						trailId = particleParentId;
						currentTrail = &trails.at(trailId);

						std::vector<pixelpart::float_t> relativeParticlePosition(currentTrail->particleCount);
						relativeParticlePosition[0] = 0.0;

						currentTrail->length = 0.0;
						for(std::uint32_t q = 1; q < currentTrail->particleCount; q++) {
							currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1]]);
							relativeParticlePosition[q] = currentTrail->length;
						}
						for(std::uint32_t q = 1; q < currentTrail->particleCount; q++) {
							relativeParticlePosition[q] /= currentTrail->length;
						}

						pixelpart::Curve<pixelpart::float3_t> positionCurve(pixelpart::CurveInterpolation::spline);
						positionCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
						positionCurve.points(pixelpart_unity::orderedPoints(relativeParticlePosition.data(), particles.globalPosition, sortKeys.data() + p, currentTrail->particleCount));

						pixelpart::Curve<pixelpart::float3_t> sizeCurve(pixelpart::CurveInterpolation::spline);
						sizeCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
						sizeCurve.points(pixelpart_unity::orderedPoints(relativeParticlePosition.data(), particles.size, sortKeys.data() + p, currentTrail->particleCount));

						pixelpart::Curve<pixelpart::float4_t> colorCurve(pixelpart::CurveInterpolation::spline);
						colorCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
						colorCurve.points(pixelpart_unity::orderedPoints(relativeParticlePosition.data(), particles.color, sortKeys.data() + p, currentTrail->particleCount));

						pixelpart::Curve<pixelpart::float3_t> velocityCurve(pixelpart::CurveInterpolation::spline);
						velocityCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
						velocityCurve.points(pixelpart_unity::orderedPoints(relativeParticlePosition.data(), particles.velocity, sortKeys.data() + p, currentTrail->particleCount));

						pixelpart::Curve<pixelpart::float_t> lifeCurve(pixelpart::CurveInterpolation::spline);
						lifeCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
						lifeCurve.points(pixelpart_unity::orderedPoints(relativeParticlePosition.data(), particles.life, sortKeys.data() + p, currentTrail->particleCount));

						currentTrail->position = positionCurve.cache();
						currentTrail->size = sizeCurve.cache();
						currentTrail->color = colorCurve.cache();
						currentTrail->velocity = velocityCurve.cache();
						currentTrail->life = lifeCurve.cache();
						currentTrail->direction.resize(particleType.trailRendererSettings().smoothingSegmentCount);
						currentTrail->directionToEdge.resize(particleType.trailRendererSettings().smoothingSegmentCount);
						currentTrail->index.resize(particleType.trailRendererSettings().smoothingSegmentCount);
					}
				}

				break;
			}

			default: {
				for(std::uint32_t p = 0, q = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++, q++) {
					std::uint32_t particleIndex = sortKeys[p];
					particleParentId = particles.parentId[particleIndex];

					if(particleParentId != trailId) {
						trailId = particleParentId;
						q = 0;

						currentTrail = &trails.at(trailId);
						currentTrail->position.resize(currentTrail->particleCount);
						currentTrail->size.resize(currentTrail->particleCount);
						currentTrail->color.resize(currentTrail->particleCount);
						currentTrail->velocity.resize(currentTrail->particleCount);
						currentTrail->life.resize(currentTrail->particleCount);
						currentTrail->direction.resize(currentTrail->particleCount);
						currentTrail->directionToEdge.resize(currentTrail->particleCount);
						currentTrail->index.resize(currentTrail->particleCount);
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
			pixelpart_unity::TrailMeshData& trail = entry.second;
			trail.length = 0.0;

			if(trail.position.size() < 2) {
				continue;
			}

			std::size_t last = trail.position.size() - 1;
			for(std::size_t i = 0; i < last; i++) {
				pixelpart::float3_t toNext = trail.position[i + 1] - trail.position[i];
				pixelpart::float_t distance = std::max(glm::length(toNext), epsilon);

				trail.direction[i] = toNext / distance;
				trail.index[i] = trail.length;
				trail.length += distance;
			}

			trail.direction[last] = trail.direction[last - 1];
			trail.index[last] = trail.length;

			for(std::size_t i = last; i > 0; i--) {
				pixelpart::float3_t toEdge = trail.direction[i] - trail.direction[i - 1];
				pixelpart::float_t toEdgeLength = glm::length(toEdge);
				if(toEdgeLength < epsilon) {
					trail.directionToEdge[i] = pixelpart::float3_t(
						-trail.direction[i].y,
						trail.direction[i].x,
						trail.direction[i].z);
				}
				else {
					trail.directionToEdge[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::float3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
						? +toEdge / toEdgeLength
						: -toEdge / toEdgeLength;
				}
			}

			trail.directionToEdge[0] = trail.directionToEdge[1];

			for(pixelpart::float_t& index : trail.index) {
				index /= trail.length;
			}

			*triangleCount += (static_cast<pixelpart_unity::int_t>(trail.position.size()) - 1) * 12 / 3;
			*vertexCount += (static_cast<pixelpart_unity::int_t>(trail.position.size()) - 1) * 5;
		}
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGetParticleSpriteVertexData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t viewRight, pixelpart_unity::vector3_t viewUp, pixelpart_unity::vector3_t effectSize,
	pixelpart_unity::int_t* triangles, pixelpart_unity::vector3_t* vertices, pixelpart_unity::color_t* colors, pixelpart_unity::vector3_t* normals, pixelpart_unity::vector2_t* uv0, pixelpart_unity::vector4_t* uv1, pixelpart_unity::vector4_t* uv2) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
		const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(pixelpart::id_t(particleEmitterId));
		const pixelpart::ParticleType& particleType = effect.particleTypes().at(pixelpart::id_t(particleTypeId));
		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		if(!particleCollection) {
			return false;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart_unity::ParticleMeshData& meshData = effectRuntime->meshData[runtimeId];

		pixelpart::float3_t effectScale = pixelpart_unity::fromUnity(effectSize);
		pixelpart::float3_t cameraRight = pixelpart_unity::fromUnity(viewRight);
		pixelpart::float3_t cameraUp = pixelpart_unity::fromUnity(viewUp);

		pixelpart::Transform emitterTransform = effect.sceneGraph().globalTransform(particleEmitter.id(), effectRuntime->effectEngine->runtimeContext());
		pixelpart::float3_t emitterPosition = emitterTransform.position();
		pixelpart::mat3_t emitterRotationMatrix = pixelpart_unity::rotation3d(emitterTransform.rotation());

		for(std::int32_t p = 0; p < static_cast<std::int32_t>(particleCount); p++) {
			triangles[p * 6 + 0] = p * 4 + 0;
			triangles[p * 6 + 1] = p * 4 + 1;
			triangles[p * 6 + 2] = p * 4 + 3;
			triangles[p * 6 + 3] = p * 4 + 1;
			triangles[p * 6 + 4] = p * 4 + 2;
			triangles[p * 6 + 5] = p * 4 + 3;
		}

		pixelpart::ParticleCollection::ReadPtr particleRenderData = particles;

		if(effect.is3d()) {
			if(particleType.spriteRendererSettings().sortCriterion != pixelpart::ParticleSortCriterion::none && particleCount > 1) {
				if(particleCount > meshData.sortedParticleCollection.capacity()) {
					meshData.sortedParticleCollection = pixelpart::ParticleCollection(particleCount);
					meshData.sortKeys.resize(particleCount);
				}

				std::iota(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount, 0);

				switch(particleType.spriteRendererSettings().sortCriterion) {
					case pixelpart::ParticleSortCriterion::age: {
#ifdef PIXELPART_RUNTIME_MULTITHREADING
						if(pixelpart_unity::threadPool) {
							pixelpart_unity::parallelInsertionSort(*pixelpart_unity::threadPool, 64, meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
								[&particles](std::uint32_t i, std::uint32_t j) {
									return particles.id[i] < particles.id[j];
								});

							break;
						}
#endif

						pixelpart_unity::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
							[&particles](std::uint32_t i, std::uint32_t j) {
								return particles.id[i] < particles.id[j];
							});

						break;
					}

					case pixelpart::ParticleSortCriterion::distance: {
						pixelpart::float3_t cameraPos = pixelpart::float3_t(cameraPosition.x, cameraPosition.y, cameraPosition.z);

#ifdef PIXELPART_RUNTIME_MULTITHREADING
						if(pixelpart_unity::threadPool) {
							pixelpart_unity::parallelInsertionSort(*pixelpart_unity::threadPool, 64, meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
								[&particles, cameraPos](std::uint32_t i, std::uint32_t j) {
									return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
								});

							break;
						}
#endif

						pixelpart_unity::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
							[&particles, cameraPos](std::uint32_t i, std::uint32_t j) {
								return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
							});

						break;
					}

					default: {
						break;
					}
				}

				pixelpart::ParticleCollection::WritePtr sortedParticleData = meshData.sortedParticleCollection.writePtr();

				for(std::uint32_t i = 0; i < particleCount; i++) {
					std::uint32_t j = meshData.sortKeys[i];

					sortedParticleData.id[i] = particles.id[j];
					sortedParticleData.life[i] = particles.life[j];
					sortedParticleData.globalPosition[i] = particles.globalPosition[j];
					sortedParticleData.velocity[i] = particles.velocity[j];
					sortedParticleData.force[i] = particles.force[j];
					sortedParticleData.rotation[i] = particles.rotation[j];
					sortedParticleData.size[i] = particles.size[j];
					sortedParticleData.color[i] = particles.color[j];
				}

				particleRenderData = meshData.sortedParticleCollection.readPtr();
			}

			for(std::uint32_t p = 0; p < particleCount; p++) {
				pixelpart::mat3_t rotationMatrix = pixelpart_unity::rotation3d(particleRenderData.rotation[p]);
				pixelpart::float3_t pivot = particleType.pivot().value() * particleRenderData.size[p];
				pixelpart::float3_t position[4] = {
					(rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot) * effectScale
				};

				switch(particleType.alignmentMode()) {
					case pixelpart::AlignmentMode::camera: {
						position[0] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[0].x + cameraUp * position[0].y;
						position[1] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[1].x + cameraUp * position[1].y;
						position[2] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[2].x + cameraUp * position[2].y;
						position[3] = particleRenderData.globalPosition[p] * effectScale + cameraRight * position[3].x + cameraUp * position[3].y;
						break;
					}
					case pixelpart::AlignmentMode::motion: {
						pixelpart::mat3_t lookAtMatrix = pixelpart_unity::lookAt(particleRenderData.velocity[p] * effectScale);
						position[0] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[0];
						position[1] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[1];
						position[2] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[2];
						position[3] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[3];
						break;
					}
					case pixelpart::AlignmentMode::emission: {
						pixelpart::mat3_t lookAtMatrix = pixelpart_unity::lookAt(emitterPosition - particleRenderData.globalPosition[p]);
						position[0] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[0];
						position[1] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[1];
						position[2] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[2];
						position[3] = particleRenderData.globalPosition[p] * effectScale + lookAtMatrix * position[3];
						break;
					}
					case pixelpart::AlignmentMode::emitter: {
						position[0] = particleRenderData.globalPosition[p] * effectScale + emitterRotationMatrix * position[0];
						position[1] = particleRenderData.globalPosition[p] * effectScale + emitterRotationMatrix * position[1];
						position[2] = particleRenderData.globalPosition[p] * effectScale + emitterRotationMatrix * position[2];
						position[3] = particleRenderData.globalPosition[p] * effectScale + emitterRotationMatrix * position[3];
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

				vertices[p * 4 + 0] = pixelpart_unity::toUnity(position[0]);
				vertices[p * 4 + 1] = pixelpart_unity::toUnity(position[1]);
				vertices[p * 4 + 2] = pixelpart_unity::toUnity(position[2]);
				vertices[p * 4 + 3] = pixelpart_unity::toUnity(position[3]);

				pixelpart_unity::vector3_t normal = pixelpart_unity::toUnity(rotationMatrix * pixelpart::float3_t(0.0, 0.0, -1.0));
				normals[p * 4 + 0] = normal;
				normals[p * 4 + 1] = normal;
				normals[p * 4 + 2] = normal;
				normals[p * 4 + 3] = normal;
			}
		}
		else {
			pixelpart::float_t zOffset = -0.001 * static_cast<pixelpart::float_t>(particleType.layer());

			pixelpart_unity::vector3_t normal = pixelpart_unity::vector3_t{ 0.0f, 0.0f, -1.0f };

			for(std::uint32_t p = 0; p < particleCount; p++) {
				pixelpart::float3_t position[4];
				pixelpart::float3_t pivot = particleType.pivot().value() * particles.size[p];

				switch(particleType.alignmentMode()) {
					case pixelpart::AlignmentMode::motion: {
						pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::float2_t(0.0, 1.0), (particles.velocity[p] != pixelpart::float3_t(0.0))
							? pixelpart::float2_t(glm::normalize(particles.velocity[p]))
							: pixelpart::float2_t(0.0, 1.0)));
						position[0] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[1] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[2] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[3] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						break;
					}
					case pixelpart::AlignmentMode::emission: {
						pixelpart::float3_t emissionDirection = emitterPosition - particles.globalPosition[p];
						pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::float2_t(0.0, 1.0), (emissionDirection != pixelpart::float3_t(0.0))
							? pixelpart::float2_t(glm::normalize(emissionDirection))
							: pixelpart::float2_t(0.0, 1.0)));
						position[0] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[1] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[2] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						position[3] = particles.globalPosition[p] + pixelpart_unity::rotate2d(pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p], pivot, particles.rotation[p].x + angle);
						break;
					}
					case pixelpart::AlignmentMode::emitter: {
						pixelpart::mat3_t rotationMatrix = pixelpart_unity::rotation3d(particles.rotation[p]);
						position[0] = particles.globalPosition[p] + emitterRotationMatrix * (rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[1] = particles.globalPosition[p] + emitterRotationMatrix * (rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[2] = particles.globalPosition[p] + emitterRotationMatrix * (rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[3] = particles.globalPosition[p] + emitterRotationMatrix * (rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
						break;
					}
					default: {
						pixelpart::mat3_t rotationMatrix = pixelpart_unity::rotation3d(particles.rotation[p]);
						position[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
						position[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p] - pivot) + pivot);
						break;
					}
				}

				vertices[p * 4 + 0] = pixelpart_unity::toUnity(position[0] * effectScale + zOffset);
				vertices[p * 4 + 1] = pixelpart_unity::toUnity(position[1] * effectScale + zOffset);
				vertices[p * 4 + 2] = pixelpart_unity::toUnity(position[2] * effectScale + zOffset);
				vertices[p * 4 + 3] = pixelpart_unity::toUnity(position[3] * effectScale + zOffset);

				normals[p * 4 + 0] = normal;
				normals[p * 4 + 1] = normal;
				normals[p * 4 + 2] = normal;
				normals[p * 4 + 3] = normal;
			}
		}

		for(std::uint32_t p = 0; p < particleCount; p++) {
			colors[p * 4 + 0] = pixelpart_unity::toUnityColor(particleRenderData.color[p]);
			colors[p * 4 + 1] = pixelpart_unity::toUnityColor(particleRenderData.color[p]);
			colors[p * 4 + 2] = pixelpart_unity::toUnityColor(particleRenderData.color[p]);
			colors[p * 4 + 3] = pixelpart_unity::toUnityColor(particleRenderData.color[p]);
		}

		for(std::uint32_t p = 0; p < particleCount; p++) {
			uv0[p * 4 + 0] = pixelpart_unity::vector2_t{ 0.0f, 0.0f };
			uv0[p * 4 + 1] = pixelpart_unity::vector2_t{ 1.0f, 0.0f };
			uv0[p * 4 + 2] = pixelpart_unity::vector2_t{ 1.0f, 1.0f };
			uv0[p * 4 + 3] = pixelpart_unity::vector2_t{ 0.0f, 1.0f };
		}

		for(std::uint32_t p = 0; p < particleCount; p++) {
			uv1[p * 4 + 0] = pixelpart_unity::toUnity(particleRenderData.velocity[p], 0.0);
			uv1[p * 4 + 1] = pixelpart_unity::toUnity(particleRenderData.velocity[p], 0.0);
			uv1[p * 4 + 2] = pixelpart_unity::toUnity(particleRenderData.velocity[p], 0.0);
			uv1[p * 4 + 3] = pixelpart_unity::toUnity(particleRenderData.velocity[p], 0.0);
		}

		for(std::uint32_t p = 0; p < particleCount; p++) {
			uv2[p * 4 + 0] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(particleRenderData.life[p]), static_cast<pixelpart_unity::float_t>(particleRenderData.id[p]), 0.0f, 0.0f };
			uv2[p * 4 + 1] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(particleRenderData.life[p]), static_cast<pixelpart_unity::float_t>(particleRenderData.id[p]), 0.0f, 0.0f };
			uv2[p * 4 + 2] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(particleRenderData.life[p]), static_cast<pixelpart_unity::float_t>(particleRenderData.id[p]), 0.0f, 0.0f };
			uv2[p * 4 + 3] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(particleRenderData.life[p]), static_cast<pixelpart_unity::float_t>(particleRenderData.id[p]), 0.0f, 0.0f };
		}

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

    return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGetParticleTrailVertexData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t viewRight, pixelpart_unity::vector3_t viewUp, pixelpart_unity::vector3_t effectSize,
	pixelpart_unity::int_t* triangles, pixelpart_unity::vector3_t* vertices, pixelpart_unity::color_t* colors, pixelpart_unity::vector3_t* normals, pixelpart_unity::vector2_t* uv0, pixelpart_unity::vector4_t* uv1, pixelpart_unity::vector4_t* uv2) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
		const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(pixelpart::id_t(particleEmitterId));
		const pixelpart::ParticleType& particleType = effect.particleTypes().at(pixelpart::id_t(particleTypeId));
		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		if(!particleCollection || particleCollection->count() < 2) {
			return false;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart_unity::ParticleMeshData& meshData = effectRuntime->meshData[runtimeId];

		pixelpart::float3_t effectScale = pixelpart_unity::fromUnity(effectSize);
		pixelpart::float3_t cameraPos = pixelpart_unity::fromUnity(cameraPosition);
		pixelpart::float3_t cameraRight = pixelpart_unity::fromUnity(viewRight);
		pixelpart::float3_t cameraUp = pixelpart_unity::fromUnity(viewUp);

		std::int32_t elementIndex = 0;
		std::int32_t vertexIndex = 0;

		pixelpart::float_t zOffset = !effect.is3d()
			? static_cast<pixelpart::float_t>(particleType.layer()) * -0.001
			: 0.0;

		for(const auto& entry : meshData.trails) {
			const pixelpart_unity::TrailMeshData& trail = entry.second;
			if(trail.position.size() < 2) {
				continue;
			}

			std::int32_t trailSegmentCount = static_cast<std::int32_t>(trail.position.size()) - 1;

			for(std::int32_t p = 0; p < trailSegmentCount; p++) {
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

			for(std::int32_t p = 0; p < trailSegmentCount; p++) {
				pixelpart::float3_t startToEdgeDirection = trail.directionToEdge[p];
				pixelpart::float3_t endToEdgeDirection = trail.directionToEdge[p + 1];

				if(particleType.alignmentMode() == pixelpart::AlignmentMode::camera) {
					pixelpart::float3_t cameraToStart = glm::normalize(cameraPos - trail.position[p]);
					pixelpart::float3_t cameraToEnd = glm::normalize(cameraPos - trail.position[p + 1]);
					startToEdgeDirection = glm::normalize(glm::cross(cameraToStart, trail.direction[p]));
					endToEdgeDirection = glm::normalize(glm::cross(cameraToEnd, trail.direction[p + 1]));
				}

				pixelpart::float3_t startToEdge = startToEdgeDirection * std::max(trail.size[p].x, std::max(trail.size[p].y, trail.size[p].z)) * 0.5;
				pixelpart::float3_t endToEdge = endToEdgeDirection * std::max(trail.size[p + 1].x, std::max(trail.size[p + 1].y, trail.size[p + 1].z)) * 0.5;

				pixelpart::float3_t vertexPositions[5] = {
					(trail.position[p] + startToEdge) * effectScale + pixelpart::float3_t(0.0, 0.0, zOffset),
					(trail.position[p] - startToEdge) * effectScale + pixelpart::float3_t(0.0, 0.0, zOffset),
					(trail.position[p + 1] + endToEdge) * effectScale + pixelpart::float3_t(0.0, 0.0, zOffset),
					(trail.position[p + 1] - endToEdge) * effectScale + pixelpart::float3_t(0.0, 0.0, zOffset),
					(trail.position[p] + trail.position[p + 1]) * 0.5 * effectScale + pixelpart::float3_t(0.0, 0.0, zOffset)
				};

				vertices[vertexIndex + p * 5 + 0] = pixelpart_unity::toUnity(vertexPositions[0]);
				vertices[vertexIndex + p * 5 + 1] = pixelpart_unity::toUnity(vertexPositions[1]);
				vertices[vertexIndex + p * 5 + 2] = pixelpart_unity::toUnity(vertexPositions[2]);
				vertices[vertexIndex + p * 5 + 3] = pixelpart_unity::toUnity(vertexPositions[3]);
				vertices[vertexIndex + p * 5 + 4] = pixelpart_unity::toUnity(vertexPositions[4]);

				pixelpart::float3_t vertexNormalStart = glm::cross(trail.direction[p], startToEdgeDirection);
				pixelpart::float3_t vertexNormalEnd = glm::cross(trail.direction[p + 1], endToEdgeDirection);

				normals[vertexIndex + p * 5 + 0] = pixelpart_unity::toUnity(vertexNormalStart);
				normals[vertexIndex + p * 5 + 1] = pixelpart_unity::toUnity(vertexNormalStart);
				normals[vertexIndex + p * 5 + 2] = pixelpart_unity::toUnity(vertexNormalEnd);
				normals[vertexIndex + p * 5 + 3] = pixelpart_unity::toUnity(vertexNormalEnd);
				normals[vertexIndex + p * 5 + 4] = pixelpart_unity::toUnity((vertexNormalStart + vertexNormalEnd) * 0.5);
			}

			for(std::int32_t p = 0; p < trailSegmentCount; p++) {
				colors[vertexIndex + p * 5 + 0] = pixelpart_unity::toUnityColor(trail.color[p]);
				colors[vertexIndex + p * 5 + 1] = pixelpart_unity::toUnityColor(trail.color[p]);
				colors[vertexIndex + p * 5 + 2] = pixelpart_unity::toUnityColor(trail.color[p + 1]);
				colors[vertexIndex + p * 5 + 3] = pixelpart_unity::toUnityColor(trail.color[p + 1]);
				colors[vertexIndex + p * 5 + 4] = pixelpart_unity::toUnityColor((trail.color[p] + trail.color[p + 1]) * 0.5);
			}

			switch(particleType.trailRendererSettings().textureRotation) {
				case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
					for(std::int32_t p = 0; p < trailSegmentCount; p++) {
						uv0[vertexIndex + p * 5 + 0] = pixelpart_unity::vector2_t{ pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor), 1.0f };
						uv0[vertexIndex + p * 5 + 1] = pixelpart_unity::vector2_t{ pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor), 0.0f };
						uv0[vertexIndex + p * 5 + 2] = pixelpart_unity::vector2_t{ pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor), 1.0f };
						uv0[vertexIndex + p * 5 + 3] = pixelpart_unity::vector2_t{ pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor), 0.0f };
						uv0[vertexIndex + p * 5 + 4] = pixelpart_unity::vector2_t{ pixelpart_unity::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor), 0.5f };
					}
					break;
				case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
					for(std::int32_t p = 0; p < trailSegmentCount; p++) {
						uv0[vertexIndex + p * 5 + 0] = pixelpart_unity::vector2_t{ 1.0f, 1.0f - pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 1] = pixelpart_unity::vector2_t{ 0.0f, 1.0f - pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 2] = pixelpart_unity::vector2_t{ 1.0f, 1.0f - pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 3] = pixelpart_unity::vector2_t{ 0.0f, 1.0f - pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 4] = pixelpart_unity::vector2_t{ 0.5f, 1.0f - pixelpart_unity::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor) };
					}
					break;
				case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
					for(std::int32_t p = 0; p < trailSegmentCount; p++) {
						uv0[vertexIndex + p * 5 + 0] = pixelpart_unity::vector2_t{ 1.0f - pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor), 0.0f };
						uv0[vertexIndex + p * 5 + 1] = pixelpart_unity::vector2_t{ 1.0f - pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor), 1.0f };
						uv0[vertexIndex + p * 5 + 2] = pixelpart_unity::vector2_t{ 1.0f - pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor), 0.0f };
						uv0[vertexIndex + p * 5 + 3] = pixelpart_unity::vector2_t{ 1.0f - pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor), 1.0f };
						uv0[vertexIndex + p * 5 + 4] = pixelpart_unity::vector2_t{ 1.0f - pixelpart_unity::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor), 0.5f };
					}
					break;
				default:
					for(std::int32_t p = 0; p < trailSegmentCount; p++) {
						uv0[vertexIndex + p * 5 + 0] = pixelpart_unity::vector2_t{ 0.0f, pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 1] = pixelpart_unity::vector2_t{ 1.0f, pixelpart_unity::toUnity(trail.index[p] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 2] = pixelpart_unity::vector2_t{ 0.0f, pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 3] = pixelpart_unity::vector2_t{ 1.0f, pixelpart_unity::toUnity(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor) };
						uv0[vertexIndex + p * 5 + 4] = pixelpart_unity::vector2_t{ 0.5f, pixelpart_unity::toUnity((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor) };
					}
					break;
			}

			for(std::int32_t p = 0; p < trailSegmentCount; p++) {
				uv1[vertexIndex + p * 5 + 0] = pixelpart_unity::toUnity(trail.velocity[p], 0.0);
				uv1[vertexIndex + p * 5 + 1] = pixelpart_unity::toUnity(trail.velocity[p], 0.0);
				uv1[vertexIndex + p * 5 + 2] = pixelpart_unity::toUnity(trail.velocity[p + 1], 0.0);
				uv1[vertexIndex + p * 5 + 3] = pixelpart_unity::toUnity(trail.velocity[p + 1], 0.0);
				uv1[vertexIndex + p * 5 + 4] = pixelpart_unity::toUnity((trail.velocity[p] + trail.velocity[p + 1]) * 0.5, 0.0);
			}

			for(std::int32_t p = 0; p < trailSegmentCount; p++) {
				uv2[vertexIndex + p * 5 + 0] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(trail.life[p]), static_cast<pixelpart_unity::float_t>(entry.first), 0.0f, 0.0f };
				uv2[vertexIndex + p * 5 + 1] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(trail.life[p]), static_cast<pixelpart_unity::float_t>(entry.first), 0.0f, 0.0f };
				uv2[vertexIndex + p * 5 + 2] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(trail.life[p + 1]), static_cast<pixelpart_unity::float_t>(entry.first), 0.0f, 0.0f };
				uv2[vertexIndex + p * 5 + 3] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(trail.life[p + 1]), static_cast<pixelpart_unity::float_t>(entry.first), 0.0f, 0.0f };
				uv2[vertexIndex + p * 5 + 4] = pixelpart_unity::vector4_t{ pixelpart_unity::toUnity(trail.life[p] + trail.life[p + 1]) * 0.5f, static_cast<pixelpart_unity::float_t>(entry.first), 0.0f, 0.0f };
			}

			elementIndex += trailSegmentCount * 12;
			vertexIndex += trailSegmentCount * 5;
		}

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::bool_t UNITY_INTERFACE_API PixelpartGetParticleMeshInstanceData(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t particleEmitterId, pixelpart_unity::uint_t particleTypeId,
	pixelpart_unity::vector3_t cameraPosition, pixelpart_unity::vector3_t viewRight, pixelpart_unity::vector3_t viewUp, pixelpart_unity::vector3_t effectSize,
	pixelpart_unity::matrix4x4_t* transforms, pixelpart_unity::vector4_t* colors, pixelpart_unity::vector4_t* velocities, pixelpart_unity::float_t* lives, pixelpart_unity::float_t* ids) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return false;
	}

	try {
		pixelpart::ParticleRuntimeId runtimeId = pixelpart::ParticleRuntimeId(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		const pixelpart::Effect& effect = effectRuntime->effectAsset.effect();
		const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(pixelpart::id_t(particleEmitterId));
		const pixelpart::ParticleType& particleType = effect.particleTypes().at(pixelpart::id_t(particleTypeId));
		const pixelpart::ParticleCollection* particleCollection = effectRuntime->effectEngine->particles(
			pixelpart::id_t(particleEmitterId), pixelpart::id_t(particleTypeId));

		if(!particleCollection) {
			return false;
		}

		pixelpart::ParticleCollection::ReadPtr particles = particleCollection->readPtr();
		std::uint32_t particleCount = particleCollection->count();

		pixelpart_unity::ParticleMeshData& meshData = effectRuntime->meshData[runtimeId];

		pixelpart::float3_t effectScale = pixelpart_unity::fromUnity(effectSize);
		pixelpart::float3_t cameraPos = pixelpart_unity::fromUnity(cameraPosition);
		pixelpart::float3_t cameraRight = pixelpart_unity::fromUnity(viewRight);
		pixelpart::float3_t cameraUp = pixelpart_unity::fromUnity(viewUp);

		pixelpart::Transform emitterTransform = effect.sceneGraph().globalTransform(particleEmitter.id(), effectRuntime->effectEngine->runtimeContext());
		pixelpart::float3_t emitterPosition = emitterTransform.position();
		pixelpart::mat3_t emitterRotationMatrix = pixelpart_unity::rotation3d(emitterTransform.rotation());

		pixelpart::ParticleCollection::ReadPtr particleRenderData = particles;

		if(effect.is3d()) {
			if(particleType.spriteRendererSettings().sortCriterion != pixelpart::ParticleSortCriterion::none && particleCount > 1) {
				if(particleCount > meshData.sortedParticleCollection.capacity()) {
					meshData.sortedParticleCollection = pixelpart::ParticleCollection(particleCount);
					meshData.sortKeys.resize(particleCount);
				}

				std::iota(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount, 0);

				switch(particleType.spriteRendererSettings().sortCriterion) {
					case pixelpart::ParticleSortCriterion::age: {
	#ifdef PIXELPART_RUNTIME_MULTITHREADING
						if(pixelpart_unity::threadPool) {
							pixelpart_unity::parallelInsertionSort(*pixelpart_unity::threadPool, 64, meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
								[&particles](std::uint32_t i, std::uint32_t j) {
									return particles.id[i] < particles.id[j];
								});

							break;
						}
	#endif

						pixelpart_unity::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
							[&particles](std::uint32_t i, std::uint32_t j) {
								return particles.id[i] < particles.id[j];
							});

						break;
					}

					case pixelpart::ParticleSortCriterion::distance: {
	#ifdef PIXELPART_RUNTIME_MULTITHREADING
						if(pixelpart_unity::threadPool) {
							pixelpart_unity::parallelInsertionSort(*pixelpart_unity::threadPool, 64, meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
								[&particles, cameraPos](std::uint32_t i, std::uint32_t j) {
									return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
								});

							break;
						}
	#endif

						pixelpart_unity::insertionSort(meshData.sortKeys.begin(), meshData.sortKeys.begin() + particleCount,
							[&particles, cameraPos](std::uint32_t i, std::uint32_t j) {
								return glm::distance2(particles.globalPosition[i], cameraPos) > glm::distance2(particles.globalPosition[j], cameraPos);
							});

						break;
					}

					default: {
						break;
					}
				}

				pixelpart::ParticleCollection::WritePtr sortedParticleData = meshData.sortedParticleCollection.writePtr();

				for(std::uint32_t i = 0; i < particleCount; i++) {
					std::uint32_t j = meshData.sortKeys[i];

					sortedParticleData.id[i] = particles.id[j];
					sortedParticleData.life[i] = particles.life[j];
					sortedParticleData.globalPosition[i] = particles.globalPosition[j];
					sortedParticleData.velocity[i] = particles.velocity[j];
					sortedParticleData.force[i] = particles.force[j];
					sortedParticleData.rotation[i] = particles.rotation[j];
					sortedParticleData.size[i] = particles.size[j];
					sortedParticleData.color[i] = particles.color[j];
				}

				particleRenderData = meshData.sortedParticleCollection.readPtr();
			}
		}

		pixelpart::mat4_t globalScaleMatrix = glm::scale(pixelpart::float3_t(effectScale));

		for(std::uint32_t p = 0; p < particleCount; p++) {
			pixelpart::mat4_t alignmentMatrix = pixelpart::mat4_t(1.0);
			switch(particleType.alignmentMode()) {
				case pixelpart::AlignmentMode::motion:
					alignmentMatrix = pixelpart::mat4_t(pixelpart_unity::lookAt(particleRenderData.velocity[p]));
					break;
				case pixelpart::AlignmentMode::emission:
					alignmentMatrix = pixelpart::mat4_t(pixelpart_unity::lookAt(emitterPosition - particleRenderData.globalPosition[p]));
					break;
				case pixelpart::AlignmentMode::emitter:
					alignmentMatrix = pixelpart::mat4_t(emitterRotationMatrix);
					break;
				default:
					break;
			}

			pixelpart::float3_t pivot = particleType.pivot().value() * particleRenderData.size[p];
			pixelpart::mat4_t rotationMatrix =
				glm::translate(pivot) *
				pixelpart::mat4_t(pixelpart_unity::rotation3d(particleRenderData.rotation[p])) *
				glm::translate(-pivot);

			pixelpart::mat4_t translationMatrix = glm::translate(particleRenderData.globalPosition[p]);
			pixelpart::mat4_t scaleMatrix = glm::scale(particleRenderData.size[p]);

			glm::mat4 transformationMatrix =
				globalScaleMatrix * translationMatrix * alignmentMatrix * rotationMatrix * scaleMatrix;

			std::memcpy(transforms[p].data, &transformationMatrix[0][0], sizeof(pixelpart_unity::float_t) * 16);

			colors[p] = pixelpart_unity::toUnity(particleRenderData.color[p]);
			velocities[p] = pixelpart_unity::toUnity(particleRenderData.velocity[p], 0.0);
			lives[p] = pixelpart_unity::toUnity(particleRenderData.life[p]);
			ids[p] = static_cast<pixelpart_unity::float_t>(particleRenderData.id[p]);
		}

		return true;
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return false;
}
}
