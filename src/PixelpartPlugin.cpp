#include "PixelpartPlugin.h"
#include "PixelpartShaderGraph.h"
#include "SortUtil.h"
#include <locale>

pixelpart::vec3d rotate2d(const pixelpart::vec3d& p, const pixelpart::vec3d& o, pixelpart::floatd a) {
	pixelpart::floatd s = std::sin(glm::radians(a));
	pixelpart::floatd c = std::cos(glm::radians(a));

	return pixelpart::vec3d(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3d rotation3d(const pixelpart::vec3d& angle) {
	pixelpart::vec3d rotation = glm::radians(angle);
	pixelpart::floatd cy = std::cos(rotation.y);
	pixelpart::floatd sy = std::sin(rotation.y);
	pixelpart::floatd cp = std::cos(rotation.z);
	pixelpart::floatd sp = std::sin(rotation.z);
	pixelpart::floatd cr = std::cos(rotation.x);
	pixelpart::floatd sr = std::sin(rotation.x);

	return pixelpart::mat3d(
		pixelpart::vec3d(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::vec3d(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::vec3d(sy * cp, -sp, cy * cp));
}
pixelpart::mat3d lookAt(const pixelpart::vec3d& direction) {
	pixelpart::vec3d up = pixelpart::vec3d(0.0, 1.0, 0.0);
	pixelpart::vec3d front = glm::normalize(direction);
	pixelpart::vec3d right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3d(right, up, front);
}

extern "C" {
static int32_t particleCapacity = 10000;

static bool initialized = false;

#ifndef __EMSCRIPTEN__
static std::unique_ptr<pixelpart::ThreadPool> sortThreadPool = nullptr;
#endif

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetParticleCapacity(int32_t value) {
	particleCapacity = std::max(value, 1);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetParticleCapacity() {
	return particleCapacity;
}

UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const char* data, int32_t size) {
	if(!initialized) {
		try {
			nlohmann::ordered_json modelJson = nlohmann::json::parse(
				PixelpartShaderGraph_json,
				PixelpartShaderGraph_json + PixelpartShaderGraph_json_size);

			pixelpart::ShaderGraph::graphType = modelJson;

#ifndef __EMSCRIPTEN__
			sortThreadPool = std::unique_ptr<pixelpart::ThreadPool>(
				new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
#endif

			initialized = true;
		}
		catch(...) {
			return nullptr;
		}
	}

	if(!data || size <= 0) {
		return nullptr;
	}

	try {
		PixelpartNativeEffect* nativeEffect = new PixelpartNativeEffect();
		nativeEffect->project = pixelpart::deserialize(data, static_cast<std::size_t>(size), nativeEffect->projectResources);
		nativeEffect->particleEngine = pixelpart::ParticleEngine(&(nativeEffect->project.effect), particleCapacity);
		nativeEffect->meshData.resize(nativeEffect->project.effect.particleTypes.getCount());

		return nativeEffect;
	}
	catch(...) {
		return nullptr;
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartDeleteEffect(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	delete nativeEffect;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateEffect(PixelpartNativeEffect* nativeEffect, float dt) {
	if(!nativeEffect) {
		return;
	}

	if(nativeEffect->playing) {
		nativeEffect->simulationTime += glm::clamp(dt, 0.0f, 1.0f) * nativeEffect->speed;

		while(nativeEffect->simulationTime > nativeEffect->timeStep) {
			nativeEffect->simulationTime -= nativeEffect->timeStep;
			nativeEffect->particleEngine.step(nativeEffect->timeStep);

			if(nativeEffect->loop) {
				if(nativeEffect->particleEngine.getTime() > nativeEffect->loopTime) {
					nativeEffect->particleEngine.reset();
				}
			}
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateParticleSolver(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.updateParticleSolver();
}


UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateForceSolver(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateCollisionSolver(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.updateCollisionSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartPlayEffect(PixelpartNativeEffect* nativeEffect, bool state) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->playing = state;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartRestartEffect(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.restart();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartResetEffect(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.reset();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartIsEffect3D(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return false;
	}

	return nativeEffect->project.effect.is3d;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectLoop(PixelpartNativeEffect* nativeEffect, bool loop) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->loop = loop;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectLoopTime(PixelpartNativeEffect* nativeEffect, float loopTime) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->loopTime = std::max(loopTime, 0.0f);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectSpeed(PixelpartNativeEffect* nativeEffect, float speed) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->speed = std::max(speed, 0.0f);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetEffectTimeStep(PixelpartNativeEffect* nativeEffect, float timeStep) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->timeStep = std::max(timeStep, 0.001f);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartIsEffectPlaying(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return false;
	}

	return nativeEffect->playing;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetEffectLoop(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return false;
	}

	return nativeEffect->loop;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectLoopTime(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0.0f;
	}

	return nativeEffect->loopTime;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectSpeed(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0.0f;
	}

	return nativeEffect->speed;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectTimeStep(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0.0f;
	}

	return nativeEffect->timeStep;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartGetEffectTime(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0.0f;
	}

	return nativeEffect->particleEngine.getTime();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticleEmitters(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.particleEmitters.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticleTypes(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.particleTypes.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumForceFields(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.forceFields.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumColliders(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.colliders.getCount();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex) {
	if(!nativeEffect || particleTypeIndex >= nativeEffect->project.effect.particleTypes.getCount()) {
		return 0;
	}

	return nativeEffect->particleEngine.getNumParticles(particleTypeIndex);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetParticleTypesSortedByLayer(PixelpartNativeEffect* nativeEffect, uint32_t* indices) {
	if(!nativeEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = nativeEffect->project.effect.particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	for(uint32_t i = 0; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartBuildParticleShader(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex,
char* bufferCode, char* bufferTextureIds,
int32_t* outLengthCode, int32_t* outLengthTextureIds,
int32_t bufferSizeCode, int32_t bufferSizeTexturesIds) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.containsIndex(particleTypeIndex) ||
	!bufferCode || !bufferTextureIds ||
	!outLengthCode || !outLengthTextureIds ||
	bufferSizeCode < 2 || bufferSizeTexturesIds < 2) {
		return false;
	}

	pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.getByIndex(particleTypeIndex);

	try {
		std::locale::global(std::locale::classic());

		pixelpart::ShaderGraph::BuildResult buildResult;
		particleType.shader.build(buildResult);

		std::string textureIdString;
		for(const std::string& textureId : buildResult.textureIds) {
			textureIdString += textureId;
			textureIdString += ";";
		}

		if(!textureIdString.empty()) {
			textureIdString.pop_back();
		}

		*outLengthCode = std::min(static_cast<int32_t>(buildResult.code.size()), bufferSizeCode - 1);
		strncpy(bufferCode, buildResult.code.c_str(), *outLengthCode);
		bufferCode[*outLengthCode] = '\0';

		*outLengthTextureIds = std::min(static_cast<int32_t>(textureIdString.size()), bufferSizeTexturesIds - 1);
		strncpy(bufferTextureIds, textureIdString.c_str(), *outLengthTextureIds);
		bufferTextureIds[*outLengthTextureIds] = '\0';

		return true;
	}
	catch(...) {
		return false;
	}

	return false;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartPrepareParticleMesh(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex, int32_t* numTriangles, int32_t* numVertices) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.containsIndex(particleTypeIndex) || !numTriangles || !numVertices) {
		return false;
	}

	const pixelpart::ParticleType& particleType = nativeEffect->project.effect.particleTypes.getByIndex(particleTypeIndex);
	if(!nativeEffect->project.effect.particleEmitters.contains(particleType.parentId)) {
		return false;
	}

	const pixelpart::ParticleEmitter& particleEmitter = nativeEffect->project.effect.particleEmitters.get(particleType.parentId);
	const pixelpart::ParticleData& particles = nativeEffect->particleEngine.getParticles(particleTypeIndex);
	uint32_t numParticles = nativeEffect->particleEngine.getNumParticles(particleTypeIndex);

	if(particleTypeIndex > nativeEffect->meshData.size()) {
		nativeEffect->meshData.resize(particleTypeIndex + 1);
	}

	PixelpartMeshData& meshData = nativeEffect->meshData[particleTypeIndex];

	if(particleType.renderer == pixelpart::ParticleType::Renderer::sprite) {
		*numTriangles = static_cast<int32_t>(numParticles) * 6 / 3;
		*numVertices = static_cast<int32_t>(numParticles) * 4;

		return true;
	}
	else if(particleType.renderer == pixelpart::ParticleType::Renderer::trail && numParticles > 1) {
		std::vector<uint32_t> sortKeys(numParticles);
		std::iota(sortKeys.begin(), sortKeys.end(), 0);
		std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
			return (particles.parentId[p0] != particles.parentId[p1])
				? particles.parentId[p0] > particles.parentId[p1]
				: particles.id[p0] > particles.id[p1];
		});

		for(auto& entry : meshData.trails) {
			entry.second.numParticles = 0;
		}

		PixelpartMeshData::ParticleTrail* trail = nullptr;
		uint32_t trailId = 0xFFFFFFFF - 1;
		uint32_t particleParentId = 0;

		for(uint32_t p = 0; p < numParticles; p++) {
			particleParentId = particles.parentId[sortKeys[p]];

			if(particleParentId != trailId) {
				trailId = particleParentId;

				if(meshData.trails.count(trailId) == 0) {
					meshData.trails[trailId] = PixelpartMeshData::ParticleTrail();
				}

				trail = &meshData.trails.at(trailId);
			}

			trail->numParticles++;
		}

		for(auto it = meshData.trails.begin(); it != meshData.trails.end(); ) {
			if(it->second.numParticles == 0) {
				it = meshData.trails.erase(it);
			}
			else {
				it++;
			}
		}

		trail = nullptr;
		trailId = 0xFFFFFFFF - 1;
		particleParentId = 0;

		if(particleType.trailRendererSettings.numTrailSegments > 0) {
			for(uint32_t p = 0; p < numParticles; p++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					trail = &meshData.trails.at(trailId);

					trail->length = 0.0;
					std::vector<pixelpart::floatd> relativeParticlePosition(trail->numParticles, 0.0);
					for(uint32_t q = 1; q < trail->numParticles; q++) {
						trail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1]]);
						relativeParticlePosition[q] = trail->length;
					}
					for(uint32_t q = 0; q < trail->numParticles; q++) {
						relativeParticlePosition[q] /= trail->length;
					}

					pixelpart::Curve<pixelpart::vec3d> positionCurve(pixelpart::CurveInterpolation::spline);
					positionCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition.data(), sortKeys.data() + p, trail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> sizeCurve(pixelpart::CurveInterpolation::spline);
					sizeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size.data(), sortKeys.data() + p, trail->numParticles);

					pixelpart::Curve<pixelpart::vec4d> colorCurve(pixelpart::CurveInterpolation::spline);
					colorCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color.data(), sortKeys.data() + p, trail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> velocityCurve(pixelpart::CurveInterpolation::spline);
					velocityCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity.data(), sortKeys.data() + p, trail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> forceCurve(pixelpart::CurveInterpolation::spline);
					forceCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					forceCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.force.data(), sortKeys.data() + p, trail->numParticles);

					pixelpart::Curve<pixelpart::floatd> lifeCurve(pixelpart::CurveInterpolation::spline);
					lifeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life.data(), sortKeys.data() + p, trail->numParticles);

					trail->position = positionCurve.getCache();
					trail->size = sizeCurve.getCache();
					trail->color = colorCurve.getCache();
					trail->velocity = velocityCurve.getCache();
					trail->force = forceCurve.getCache();
					trail->direction.resize(particleType.trailRendererSettings.numTrailSegments);
					trail->index.resize(particleType.trailRendererSettings.numTrailSegments);
					trail->life = lifeCurve.getCache();
				}
			}
		}
		else {
			for(uint32_t p = 0, q = 0; p < numParticles; p++, q++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					q = 0;

					trail = &meshData.trails.at(trailId);
					trail->position.resize(trail->numParticles);
					trail->size.resize(trail->numParticles);
					trail->color.resize(trail->numParticles);
					trail->velocity.resize(trail->numParticles);
					trail->force.resize(trail->numParticles);
					trail->direction.resize(trail->numParticles);
					trail->index.resize(trail->numParticles);
					trail->life.resize(trail->numParticles);
				}

				trail->position[q] = particles.globalPosition[particleIndex];
				trail->size[q] = particles.size[particleIndex];
				trail->color[q] = particles.color[particleIndex];
				trail->velocity[q] = particles.velocity[particleIndex];
				trail->force[q] = particles.force[particleIndex];
				trail->life[q] = particles.life[particleIndex];
			}
		}

		*numTriangles = 0;
		*numVertices = 0;

		for(auto& entry : meshData.trails) {
			PixelpartMeshData::ParticleTrail& trail = entry.second;
			trail.length = 0.0;

			if(trail.numParticles < 2) {
				continue;
			}

			std::size_t last = trail.numParticles - 1;
			for(std::size_t i = 0; i < last; i++) {
				pixelpart::vec3d toNext = trail.position[i + 1] - trail.position[i];
				pixelpart::floatd distance = std::max(glm::length(toNext), 0.000001);

				trail.direction[i] = toNext / distance;
				trail.index[i] = trail.length;
				trail.length += distance;
			}

			trail.direction[last] = trail.direction[last - 1];
			trail.index[last] = trail.length;

			trail.direction[0] = pixelpart::vec3d(-trail.direction[0].y, trail.direction[0].x, trail.direction[0].z);

			for(std::size_t i = last; i > 0; i--) {
				pixelpart::vec3d h = trail.direction[i] - trail.direction[i - 1];
				pixelpart::floatd l = glm::length(h);
				if(l < 0.0001) {
					trail.direction[i] = pixelpart::vec3d(
						-trail.direction[i].y,
						trail.direction[i].x,
						trail.direction[i].z);
				}
				else {
					trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3d(0.0, 0.0, 1.0)), h / l) < 0.0)
						? +h / l
						: -h / l;
				}
			}

			for(pixelpart::floatd& x : trail.index) {
				x /= trail.length;
			}

			*numTriangles += (trail.numParticles - 1) * 6 / 3;
			*numVertices += (trail.numParticles - 1) * 4;
		}

		return true;
	}

	*numTriangles = 0;
	*numVertices = 0;

	return false;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartBuildParticleMesh(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, float scaleX, float scaleY, int32_t* triangles, Vector3* vertices, Color* colors, Vector2* uv, Vector4* uv2, Vector4* uv3, Vector4* uv4) {
	if(!nativeEffect || !nativeEffect->project.effect.particleTypes.containsIndex(particleTypeIndex)) {
		return false;
	}

	const pixelpart::Effect& effect = nativeEffect->project.effect;
	const pixelpart::ParticleType& particleType = effect.particleTypes.getByIndex(particleTypeIndex);
	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	const pixelpart::ParticleData& particles = nativeEffect->particleEngine.getParticles(particleTypeIndex);
	PixelpartMeshData& meshData = nativeEffect->meshData[particleTypeIndex];
	uint32_t numParticles = nativeEffect->particleEngine.getNumParticles(particleTypeIndex);
	pixelpart::floatd alpha = std::fmod(nativeEffect->particleEngine.getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	pixelpart::vec3d cameraRight = pixelpart::vec3d(viewRight.x, viewRight.y, viewRight.z);
	pixelpart::vec3d cameraUp = pixelpart::vec3d(viewUp.x, viewUp.y, viewUp.z);

	if(particleType.renderer == pixelpart::ParticleType::Renderer::sprite) {
		for(int32_t p = 0; p < static_cast<int32_t>(numParticles); p++) {
			triangles[p * 6 + 0] = p * 4 + 0;
			triangles[p * 6 + 1] = p * 4 + 1;
			triangles[p * 6 + 2] = p * 4 + 3;
			triangles[p * 6 + 3] = p * 4 + 1;
			triangles[p * 6 + 4] = p * 4 + 2;
			triangles[p * 6 + 5] = p * 4 + 3;
		}

		const pixelpart::ParticleData* particleRenderData = &particles;

		if(effect.is3d) {
			if(particleType.spriteRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none && numParticles > 1) {
				meshData.sortedParticleData.resize(particles.id.size());
				meshData.sortKeys.resize(particles.id.size());
				std::iota(meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles, 0);

				switch(particleType.spriteRendererSettings.sortCriterion) {
					case pixelpart::ParticleSortCriterion::age: {
#ifndef __EMSCRIPTEN__
						if(sortThreadPool) {
							util::parallelInsertionSort(*sortThreadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
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
						pixelpart::vec3d cameraPos = pixelpart::vec3d(cameraPosition.x, cameraPosition.y, cameraPosition.z);

#ifndef __EMSCRIPTEN__
						if(sortThreadPool) {
							util::parallelInsertionSort(*sortThreadPool, 64u, meshData.sortKeys.begin(), meshData.sortKeys.begin() + numParticles,
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

				for(uint32_t i = 0; i < numParticles; i++) {
					uint32_t j = meshData.sortKeys[i];

					meshData.sortedParticleData.id[i] = particles.id[j];
					meshData.sortedParticleData.life[i] = particles.life[j];
					meshData.sortedParticleData.globalPosition[i] = particles.globalPosition[j];
					meshData.sortedParticleData.velocity[i] = particles.velocity[j];
					meshData.sortedParticleData.force[i] = particles.force[j];
					meshData.sortedParticleData.rotation[i] = particles.rotation[j];
					meshData.sortedParticleData.size[i] = particles.size[j];
					meshData.sortedParticleData.color[i] = particles.color[j];
				}

				particleRenderData = &meshData.sortedParticleData;
			}

			for(uint32_t p = 0; p < numParticles; p++) {
				pixelpart::mat3d rotationMatrix = rotation3d(particleRenderData->rotation[p]);
				pixelpart::vec3d pivot = particleType.pivot * particleRenderData->size[p];
				pixelpart::vec3d worldPosition[4];
				pixelpart::vec3d localPosition[4] = {
					rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
					rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
					rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
					rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot };

				switch(particleType.alignmentMode) {
					case pixelpart::AlignmentMode::camera: {
						worldPosition[0] = particleRenderData->globalPosition[p] + cameraRight * localPosition[0].x + cameraUp * localPosition[0].y;
						worldPosition[1] = particleRenderData->globalPosition[p] + cameraRight * localPosition[1].x + cameraUp * localPosition[1].y;
						worldPosition[2] = particleRenderData->globalPosition[p] + cameraRight * localPosition[2].x + cameraUp * localPosition[2].y;
						worldPosition[3] = particleRenderData->globalPosition[p] + cameraRight * localPosition[3].x + cameraUp * localPosition[3].y;
						break;
					}
					case pixelpart::AlignmentMode::motion: {
						pixelpart::mat3d lookAtMatrix = lookAt(particleRenderData->velocity[p]);
						worldPosition[0] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[3];
						break;
					}
					case pixelpart::AlignmentMode::emission: {
						pixelpart::mat3d lookAtMatrix = lookAt(particleEmitter.position.get(alpha) - particleRenderData->globalPosition[p]);
						worldPosition[0] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[3];
						break;
					}
					case pixelpart::AlignmentMode::emitter: {
						pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
						worldPosition[0] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] + lookAtMatrix * localPosition[3];
						break;
					}
					default: {
						worldPosition[0] = particleRenderData->globalPosition[p] + localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] + localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] + localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] + localPosition[3];
						break;
					}
				}

				vertices[p * 4 + 0] = Vector3{
					static_cast<float>(worldPosition[0].x) * scaleX,
					static_cast<float>(worldPosition[0].y) * scaleY,
					static_cast<float>(worldPosition[0].z)
				};
				vertices[p * 4 + 1] = Vector3{
					static_cast<float>(worldPosition[1].x) * scaleX,
					static_cast<float>(worldPosition[1].y) * scaleY,
					static_cast<float>(worldPosition[1].z)
				};
				vertices[p * 4 + 2] = Vector3{
					static_cast<float>(worldPosition[2].x) * scaleX,
					static_cast<float>(worldPosition[2].y) * scaleY,
					static_cast<float>(worldPosition[2].z)
				};
				vertices[p * 4 + 3] = Vector3{
					static_cast<float>(worldPosition[3].x) * scaleX,
					static_cast<float>(worldPosition[3].y) * scaleY,
					static_cast<float>(worldPosition[3].z)
				};
			}
		}
		else {
			pixelpart::floatd zOffset = -0.001 * static_cast<pixelpart::floatd>(particleType.layer);

			for(uint32_t p = 0; p < numParticles; p++) {
				pixelpart::vec3d worldPosition[4];

				switch(particleType.alignmentMode) {
					case pixelpart::AlignmentMode::motion: {
						pixelpart::floatd angle = glm::degrees(glm::orientedAngle(pixelpart::vec2d(0.0, 1.0), (particles.velocity[p] != pixelpart::vec3d(0.0))
							? pixelpart::vec2d(glm::normalize(particles.velocity[p]))
							: pixelpart::vec2d(0.0, 1.0)));
						worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						break;
					}
					case pixelpart::AlignmentMode::emission: {
						pixelpart::vec3d emissionDirection = particleEmitter.position.get(alpha) - particles.globalPosition[p];
						pixelpart::floatd angle = glm::degrees(glm::orientedAngle(pixelpart::vec2d(0.0, 1.0), (emissionDirection != pixelpart::vec3d(0.0))
								? pixelpart::vec2d(glm::normalize(emissionDirection))
								: pixelpart::vec2d(0.0, 1.0)));
						worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
						break;
					}
					case pixelpart::AlignmentMode::emitter: {
						pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
						pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
						worldPosition[0] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[1] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[2] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[3] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						break;
					}
					default: {
						pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
						worldPosition[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						worldPosition[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
						break;
					}
				}

				vertices[p * 4 + 0] = Vector3{
					static_cast<float>(worldPosition[0].x) * scaleX,
					static_cast<float>(worldPosition[0].y) * scaleY,
					static_cast<float>(worldPosition[0].z + zOffset)
				};
				vertices[p * 4 + 1] = Vector3{
					static_cast<float>(worldPosition[1].x) * scaleX,
					static_cast<float>(worldPosition[1].y) * scaleY,
					static_cast<float>(worldPosition[1].z + zOffset)
				};
				vertices[p * 4 + 2] = Vector3{
					static_cast<float>(worldPosition[2].x) * scaleX,
					static_cast<float>(worldPosition[2].y) * scaleY,
					static_cast<float>(worldPosition[2].z + zOffset)
				};
				vertices[p * 4 + 3] = Vector3{
					static_cast<float>(worldPosition[3].x) * scaleX,
					static_cast<float>(worldPosition[3].y) * scaleY,
					static_cast<float>(worldPosition[3].z + zOffset)
				};
			}
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			colors[p * 4 + 0] = Color{
				static_cast<float>(particleRenderData->color[p].r),
				static_cast<float>(particleRenderData->color[p].g),
				static_cast<float>(particleRenderData->color[p].b),
				static_cast<float>(particleRenderData->color[p].a)
			};
			colors[p * 4 + 1] = Color{
				static_cast<float>(particleRenderData->color[p].r),
				static_cast<float>(particleRenderData->color[p].g),
				static_cast<float>(particleRenderData->color[p].b),
				static_cast<float>(particleRenderData->color[p].a)
			};
			colors[p * 4 + 2] = Color{
				static_cast<float>(particleRenderData->color[p].r),
				static_cast<float>(particleRenderData->color[p].g),
				static_cast<float>(particleRenderData->color[p].b),
				static_cast<float>(particleRenderData->color[p].a)
			};
			colors[p * 4 + 3] = Color{
				static_cast<float>(particleRenderData->color[p].r),
				static_cast<float>(particleRenderData->color[p].g),
				static_cast<float>(particleRenderData->color[p].b),
				static_cast<float>(particleRenderData->color[p].a)
			};
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv[p * 4 + 0] = Vector2{ 0.0f, 0.0f };
			uv[p * 4 + 1] = Vector2{ 1.0f, 0.0f };
			uv[p * 4 + 2] = Vector2{ 1.0f, 1.0f };
			uv[p * 4 + 3] = Vector2{ 0.0f, 1.0f };
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv2[p * 4 + 0] = Vector4{ static_cast<float>(particleRenderData->velocity[p].x), static_cast<float>(particleRenderData->velocity[p].y), static_cast<float>(particleRenderData->velocity[p].z), 0.0f };
			uv2[p * 4 + 1] = Vector4{ static_cast<float>(particleRenderData->velocity[p].x), static_cast<float>(particleRenderData->velocity[p].y), static_cast<float>(particleRenderData->velocity[p].z), 0.0f };
			uv2[p * 4 + 2] = Vector4{ static_cast<float>(particleRenderData->velocity[p].x), static_cast<float>(particleRenderData->velocity[p].y), static_cast<float>(particleRenderData->velocity[p].z), 0.0f };
			uv2[p * 4 + 3] = Vector4{ static_cast<float>(particleRenderData->velocity[p].x), static_cast<float>(particleRenderData->velocity[p].y), static_cast<float>(particleRenderData->velocity[p].z), 0.0f };
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv3[p * 4 + 0] = Vector4{ static_cast<float>(particleRenderData->force[p].x), static_cast<float>(particleRenderData->force[p].y), static_cast<float>(particleRenderData->force[p].z), 0.0f };
			uv3[p * 4 + 1] = Vector4{ static_cast<float>(particleRenderData->force[p].x), static_cast<float>(particleRenderData->force[p].y), static_cast<float>(particleRenderData->force[p].z), 0.0f };
			uv3[p * 4 + 2] = Vector4{ static_cast<float>(particleRenderData->force[p].x), static_cast<float>(particleRenderData->force[p].y), static_cast<float>(particleRenderData->force[p].z), 0.0f };
			uv3[p * 4 + 3] = Vector4{ static_cast<float>(particleRenderData->force[p].x), static_cast<float>(particleRenderData->force[p].y), static_cast<float>(particleRenderData->force[p].z), 0.0f };
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv4[p * 4 + 0] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 1] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 2] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 3] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
		}
	}
	else if(particleType.renderer == pixelpart::ParticleType::Renderer::trail && numParticles > 1) {
		uint32_t i0 = 0;
		uint32_t v0 = 0;

		pixelpart::floatd zOffset = !effect.is3d ?
			-0.001 * static_cast<pixelpart::floatd>(particleType.layer)
			: 0.0;

		for(const auto& entry : meshData.trails) {
			const PixelpartMeshData::ParticleTrail& trail = entry.second;
			if(trail.numParticles < 2) {
				continue;
			}

			for(int32_t p = 0; p < static_cast<int32_t>(trail.numParticles) - 1; p++) {
				triangles[i0 + p * 6 + 0] = v0 + p * 4 + 0;
				triangles[i0 + p * 6 + 1] = v0 + p * 4 + 2;
				triangles[i0 + p * 6 + 2] = v0 + p * 4 + 1;
				triangles[i0 + p * 6 + 3] = v0 + p * 4 + 2;
				triangles[i0 + p * 6 + 4] = v0 + p * 4 + 3;
				triangles[i0 + p * 6 + 5] = v0 + p * 4 + 1;
			}

			for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
				pixelpart::vec3d n0 = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
				pixelpart::vec3d n1 = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;
				pixelpart::vec3d p0 = trail.position[p] + n0;
				pixelpart::vec3d p1 = trail.position[p] - n0;
				pixelpart::vec3d p2 = trail.position[p + 1] + n1;
				pixelpart::vec3d p3 = trail.position[p + 1] - n1;

				vertices[v0 + p * 4 + 0] = Vector3{
					static_cast<float>(p0.x) * scaleX,
					static_cast<float>(p0.y) * scaleY,
					static_cast<float>(p0.z + zOffset)
				};
				vertices[v0 + p * 4 + 1] = Vector3{
					static_cast<float>(p1.x) * scaleX,
					static_cast<float>(p1.y) * scaleY,
					static_cast<float>(p1.z + zOffset)
				};
				vertices[v0 + p * 4 + 2] = Vector3{
					static_cast<float>(p2.x) * scaleX,
					static_cast<float>(p2.y) * scaleY,
					static_cast<float>(p2.z + zOffset)
				};
				vertices[v0 + p * 4 + 3] = Vector3{
					static_cast<float>(p3.x) * scaleX,
					static_cast<float>(p3.y) * scaleY,
					static_cast<float>(p3.z + zOffset)
				};
			}

			for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
				colors[v0 + p * 4 + 0] = Color{
					static_cast<float>(trail.color[p].r),
					static_cast<float>(trail.color[p].g),
					static_cast<float>(trail.color[p].b),
					static_cast<float>(trail.color[p].a)
				};
				colors[v0 + p * 4 + 1] = Color{
					static_cast<float>(trail.color[p].r),
					static_cast<float>(trail.color[p].g),
					static_cast<float>(trail.color[p].b),
					static_cast<float>(trail.color[p].a)
				};
				colors[v0 + p * 4 + 2] = Color{
					static_cast<float>(trail.color[p].r),
					static_cast<float>(trail.color[p].g),
					static_cast<float>(trail.color[p].b),
					static_cast<float>(trail.color[p].a)
				};
				colors[v0 + p * 4 + 3] = Color{
					static_cast<float>(trail.color[p].r),
					static_cast<float>(trail.color[p].g),
					static_cast<float>(trail.color[p].b),
					static_cast<float>(trail.color[p].a)
				};
			}

			switch(particleType.trailRendererSettings.textureRotation) {
				case 1: {
					for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
						uv[v0 + p * 4 + 0] = Vector2{ static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[v0 + p * 4 + 1] = Vector2{ static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[v0 + p * 4 + 2] = Vector2{ static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[v0 + p * 4 + 3] = Vector2{ static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					}
					break;
				}
				case 2: {
					for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
						uv[v0 + p * 4 + 0] = Vector2{ 1.0f, 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 1] = Vector2{ 0.0f, 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 2] = Vector2{ 1.0f, 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 3] = Vector2{ 0.0f, 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					}
					break;
				}
				case 3: {
					for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
						uv[v0 + p * 4 + 0] = Vector2{ 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[v0 + p * 4 + 1] = Vector2{ 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[v0 + p * 4 + 2] = Vector2{ 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[v0 + p * 4 + 3] = Vector2{ 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
					}
					break;
				}
				default: {
					for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
						uv[v0 + p * 4 + 0] = Vector2{ 0.0f, static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 1] = Vector2{ 1.0f, static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 2] = Vector2{ 0.0f, static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[v0 + p * 4 + 3] = Vector2{ 1.0f, static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
					}
					break;
				}
			}

			for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
				uv2[v0 + p * 4 + 0] = Vector4{ static_cast<float>(trail.velocity[p].x), static_cast<float>(trail.velocity[p].y), static_cast<float>(trail.velocity[p].z), 0.0f };
				uv2[v0 + p * 4 + 1] = Vector4{ static_cast<float>(trail.velocity[p].x), static_cast<float>(trail.velocity[p].y), static_cast<float>(trail.velocity[p].z), 0.0f };
				uv2[v0 + p * 4 + 2] = Vector4{ static_cast<float>(trail.velocity[p].x), static_cast<float>(trail.velocity[p].y), static_cast<float>(trail.velocity[p].z), 0.0f };
				uv2[v0 + p * 4 + 3] = Vector4{ static_cast<float>(trail.velocity[p].x), static_cast<float>(trail.velocity[p].y), static_cast<float>(trail.velocity[p].z), 0.0f };
			}

			for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
				uv3[v0 + p * 4 + 0] = Vector4{ static_cast<float>(trail.force[p].x), static_cast<float>(trail.force[p].y), static_cast<float>(trail.force[p].z), 0.0f };
				uv3[v0 + p * 4 + 1] = Vector4{ static_cast<float>(trail.force[p].x), static_cast<float>(trail.force[p].y), static_cast<float>(trail.force[p].z), 0.0f };
				uv3[v0 + p * 4 + 2] = Vector4{ static_cast<float>(trail.force[p].x), static_cast<float>(trail.force[p].y), static_cast<float>(trail.force[p].z), 0.0f };
				uv3[v0 + p * 4 + 3] = Vector4{ static_cast<float>(trail.force[p].x), static_cast<float>(trail.force[p].y), static_cast<float>(trail.force[p].z), 0.0f };
			}

			for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
				uv4[v0 + p * 4 + 0] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[v0 + p * 4 + 1] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[v0 + p * 4 + 2] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[v0 + p * 4 + 3] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
			}

			i0 += (trail.numParticles - 1) * 6;
			v0 += (trail.numParticles - 1) * 4;
		}
	}

	return true;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceCount(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return static_cast<uint32_t>(nativeEffect->projectResources.images.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceId(PixelpartNativeEffect* nativeEffect, uint32_t index, char* imageIdBuffer, int32_t bufferLength) {
	if(!nativeEffect || !imageIdBuffer) {
		return 0;
	}

	int32_t length = 0;
	uint32_t i = 0;
	for(const auto& entry : nativeEffect->projectResources.images) {
		if(index == i) {
			length = static_cast<int32_t>(entry.first.size());
			memcpy(imageIdBuffer, entry.first.c_str(), static_cast<std::size_t>(length));

			break;
		}

		i++;
	}

	return length;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceWidth(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return static_cast<int32_t>(image.width);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetImageResourceHeight(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return static_cast<int32_t>(image.height);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetImageResourceDataSize(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return static_cast<uint32_t>(image.data.size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetImageResourceData(PixelpartNativeEffect* nativeEffect, const char* imageId, unsigned char* imageData) {
	if(!nativeEffect || !imageId) {
		return;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	memcpy(imageData, image.data.data(), image.data.size());
}
}