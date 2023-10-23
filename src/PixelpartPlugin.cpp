#include "PixelpartPlugin.h"
#include "ParticleSolverCPU.h"
#include "PixelpartShaderGraph.h"
#include "SortUtil.h"
#include <locale>

inline Vector2 toUnity(const pixelpart::vec2d& v) {
	return Vector2{
		static_cast<float>(v.x),
		static_cast<float>(v.y)
	};
}
inline Vector3 toUnity(const pixelpart::vec3d& v) {
	return Vector3{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}
inline Vector4 toUnity(const pixelpart::vec3d& v, pixelpart::floatd w) {
	return Vector4{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(w)
	};
}
inline Vector4 toUnity(const pixelpart::vec4d& v) {
	return Vector4{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(v.w)
	};
}
inline Color toUnityColor(const pixelpart::vec4d& v) {
	return Color{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z),
		static_cast<float>(v.w)
	};
}

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
		nativeEffect->particleEngine = pixelpart::ParticleEngine::create<pixelpart::ParticleSolverCPU>(&(nativeEffect->project.effect), particleCapacity);
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
	const pixelpart::floatd epsilon = 0.000001;

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
		nativeEffect->meshData.resize(particleTypeIndex + 1u);
	}

	PixelpartMeshData& meshData = nativeEffect->meshData[particleTypeIndex];

	*numTriangles = 0;
	*numVertices = 0;

	if(particleType.renderer == pixelpart::ParticleType::Renderer::sprite) {
		*numTriangles = static_cast<int32_t>(numParticles) * 6 / 3;
		*numVertices = static_cast<int32_t>(numParticles) * 4;

		return true;
	}
	else if(particleType.renderer == pixelpart::ParticleType::Renderer::trail) {
		if(numParticles < 2u) {
			return true;
		}

		std::vector<uint32_t> sortKeys(numParticles);
		std::iota(sortKeys.begin(), sortKeys.end(), 0u);
		std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
			return (particles.parentId[p0] != particles.parentId[p1])
				? particles.parentId[p0] > particles.parentId[p1]
				: particles.id[p0] > particles.id[p1];
		});

		std::unordered_map<uint32_t, PixelpartMeshData::ParticleTrail>& trails = meshData.trails;

		for(auto& entry : trails) {
			entry.second.numParticles = 0u;
		}

		PixelpartMeshData::ParticleTrail* currentTrail = nullptr;
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

		if(particleType.trailRendererSettings.numTrailSegments > 0u) {
			for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					currentTrail = &trails.at(trailId);

					std::vector<pixelpart::floatd> relativeParticlePosition(currentTrail->numParticles);
					relativeParticlePosition[0u] = 0.0;

					currentTrail->length = 0.0;
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1u]]);
						relativeParticlePosition[q] = currentTrail->length;
					}
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						relativeParticlePosition[q] /= currentTrail->length;
					}

					pixelpart::Curve<pixelpart::vec3d> positionCurve(pixelpart::CurveInterpolation::spline);
					positionCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition.data(), sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> sizeCurve(pixelpart::CurveInterpolation::spline);
					sizeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size.data(), sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec4d> colorCurve(pixelpart::CurveInterpolation::spline);
					colorCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color.data(), sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> velocityCurve(pixelpart::CurveInterpolation::spline);
					velocityCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity.data(), sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3d> forceCurve(pixelpart::CurveInterpolation::spline);
					forceCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					forceCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.force.data(), sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::floatd> lifeCurve(pixelpart::CurveInterpolation::spline);
					lifeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
					lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life.data(), sortKeys.data() + p, currentTrail->numParticles);

					currentTrail->position = positionCurve.getCache();
					currentTrail->size = sizeCurve.getCache();
					currentTrail->color = colorCurve.getCache();
					currentTrail->velocity = velocityCurve.getCache();
					currentTrail->force = forceCurve.getCache();
					currentTrail->life = lifeCurve.getCache();
					currentTrail->direction.resize(particleType.trailRendererSettings.numTrailSegments);
					currentTrail->index.resize(particleType.trailRendererSettings.numTrailSegments);
				}
			}
		}
		else {
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
					currentTrail->force.resize(currentTrail->numParticles);
					currentTrail->life.resize(currentTrail->numParticles);
					currentTrail->direction.resize(currentTrail->numParticles);
					currentTrail->index.resize(currentTrail->numParticles);
				}

				currentTrail->position[q] = particles.globalPosition[particleIndex];
				currentTrail->size[q] = particles.size[particleIndex];
				currentTrail->color[q] = particles.color[particleIndex];
				currentTrail->velocity[q] = particles.velocity[particleIndex];
				currentTrail->force[q] = particles.force[particleIndex];
				currentTrail->life[q] = particles.life[particleIndex];
			}
		}

		for(auto& entry : trails) {
			PixelpartMeshData::ParticleTrail& trail = entry.second;
			trail.length = 0.0;

			if(trail.position.size() < 2u) {
				continue;
			}

			std::size_t last = trail.position.size() - 1u;
			for(std::size_t i = 0u; i < last; i++) {
				pixelpart::vec3d toNext = trail.position[i + 1u] - trail.position[i];
				pixelpart::floatd distance = std::max(glm::length(toNext), epsilon);

				trail.direction[i] = toNext / distance;
				trail.index[i] = trail.length;
				trail.length += distance;
			}

			trail.direction[last] = trail.direction[last - 1u];
			trail.index[last] = trail.length;

			for(std::size_t i = last; i > 0u; i--) {
				pixelpart::vec3d toEdge = trail.direction[i] - trail.direction[i - 1u];
				pixelpart::floatd toEdgeLength = glm::length(toEdge);
				if(toEdgeLength < epsilon) {
					trail.direction[i] = pixelpart::vec3d(
						-trail.direction[i].y,
						trail.direction[i].x,
						trail.direction[i].z);
				}
				else {
					trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3d(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
						? +toEdge / toEdgeLength
						: -toEdge / toEdgeLength;
				}
			}

			trail.direction[0u] = pixelpart::vec3d(-trail.direction[0u].y, trail.direction[0u].x, trail.direction[0u].z);

			for(pixelpart::floatd& index : trail.index) {
				index /= trail.length;
			}

			*numTriangles += (static_cast<int32_t>(trail.position.size()) - 1) * 12 / 3;
			*numVertices += (static_cast<int32_t>(trail.position.size()) - 1) * 5;
		}

		return true;
	}

	return false;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartBuildParticleMesh(PixelpartNativeEffect* nativeEffect, uint32_t particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale, int32_t* triangles, Vector3* vertices, Color* colors, Vector2* uv, Vector4* uv2, Vector4* uv3, Vector4* uv4) {
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

	pixelpart::vec3d effectScale = pixelpart::vec3d(scale.x, scale.y, scale.z);
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
					(rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot) * effectScale,
					(rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot) * effectScale };

				switch(particleType.alignmentMode) {
					case pixelpart::AlignmentMode::camera: {
						worldPosition[0] = particleRenderData->globalPosition[p] * effectScale + cameraRight * localPosition[0].x + cameraUp * localPosition[0].y;
						worldPosition[1] = particleRenderData->globalPosition[p] * effectScale + cameraRight * localPosition[1].x + cameraUp * localPosition[1].y;
						worldPosition[2] = particleRenderData->globalPosition[p] * effectScale + cameraRight * localPosition[2].x + cameraUp * localPosition[2].y;
						worldPosition[3] = particleRenderData->globalPosition[p] * effectScale + cameraRight * localPosition[3].x + cameraUp * localPosition[3].y;
						break;
					}
					case pixelpart::AlignmentMode::motion: {
						pixelpart::mat3d lookAtMatrix = lookAt(particleRenderData->velocity[p] * effectScale);
						worldPosition[0] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[3];
						break;
					}
					case pixelpart::AlignmentMode::emission: {
						pixelpart::mat3d lookAtMatrix = lookAt(particleEmitter.position.get(alpha) - particleRenderData->globalPosition[p]);
						worldPosition[0] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[3];
						break;
					}
					case pixelpart::AlignmentMode::emitter: {
						pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
						worldPosition[0] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] * effectScale + lookAtMatrix * localPosition[3];
						break;
					}
					default: {
						worldPosition[0] = particleRenderData->globalPosition[p] * effectScale + localPosition[0];
						worldPosition[1] = particleRenderData->globalPosition[p] * effectScale + localPosition[1];
						worldPosition[2] = particleRenderData->globalPosition[p] * effectScale + localPosition[2];
						worldPosition[3] = particleRenderData->globalPosition[p] * effectScale + localPosition[3];
						break;
					}
				}

				vertices[p * 4 + 0] = toUnity(worldPosition[0]);
				vertices[p * 4 + 1] = toUnity(worldPosition[1]);
				vertices[p * 4 + 2] = toUnity(worldPosition[2]);
				vertices[p * 4 + 3] = toUnity(worldPosition[3]);
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

				worldPosition[0] *= effectScale;
				worldPosition[1] *= effectScale;
				worldPosition[2] *= effectScale;
				worldPosition[3] *= effectScale;
				worldPosition[0].z += zOffset;
				worldPosition[1].z += zOffset;
				worldPosition[2].z += zOffset;
				worldPosition[3].z += zOffset;

				vertices[p * 4 + 0] = toUnity(worldPosition[0]);
				vertices[p * 4 + 1] = toUnity(worldPosition[1]);
				vertices[p * 4 + 2] = toUnity(worldPosition[2]);
				vertices[p * 4 + 3] = toUnity(worldPosition[3]);
			}
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			colors[p * 4 + 0] = toUnityColor(particleRenderData->color[p]);
			colors[p * 4 + 1] = toUnityColor(particleRenderData->color[p]);
			colors[p * 4 + 2] = toUnityColor(particleRenderData->color[p]);
			colors[p * 4 + 3] = toUnityColor(particleRenderData->color[p]);
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv[p * 4 + 0] = Vector2{ 0.0f, 0.0f };
			uv[p * 4 + 1] = Vector2{ 1.0f, 0.0f };
			uv[p * 4 + 2] = Vector2{ 1.0f, 1.0f };
			uv[p * 4 + 3] = Vector2{ 0.0f, 1.0f };
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv2[p * 4 + 0] = toUnity(particleRenderData->velocity[p], 0.0);
			uv2[p * 4 + 1] = toUnity(particleRenderData->velocity[p], 0.0);
			uv2[p * 4 + 2] = toUnity(particleRenderData->velocity[p], 0.0);
			uv2[p * 4 + 3] = toUnity(particleRenderData->velocity[p], 0.0);
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv3[p * 4 + 0] = toUnity(particleRenderData->force[p], 0.0);
			uv3[p * 4 + 1] = toUnity(particleRenderData->force[p], 0.0);
			uv3[p * 4 + 2] = toUnity(particleRenderData->force[p], 0.0);
			uv3[p * 4 + 3] = toUnity(particleRenderData->force[p], 0.0);
		}

		for(uint32_t p = 0; p < numParticles; p++) {
			uv4[p * 4 + 0] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 1] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 2] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
			uv4[p * 4 + 3] = Vector4{ static_cast<float>(particleRenderData->life[p]), static_cast<float>(particleRenderData->id[p]), 0.0f, 0.0f };
		}
	}
	else if(particleType.renderer == pixelpart::ParticleType::Renderer::trail) {
		if(numParticles < 2u) {
			return true;
		}

		int32_t elementIndex = 0;
		int32_t vertexIndex = 0;

		pixelpart::floatd zOffset = !effect.is3d
			? static_cast<pixelpart::floatd>(particleType.layer) * -0.001
			: 0.0;

		for(const auto& entry : meshData.trails) {
			const PixelpartMeshData::ParticleTrail& trail = entry.second;
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
				pixelpart::vec3d startToEdge = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
				pixelpart::vec3d endToEdge = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;
				pixelpart::vec3d vertexPositions[5] = {
					(trail.position[p] + startToEdge) * effectScale + pixelpart::vec3d(0.0, 0.0, zOffset),
					(trail.position[p] - startToEdge) * effectScale + pixelpart::vec3d(0.0, 0.0, zOffset),
					(trail.position[p + 1] + endToEdge) * effectScale + pixelpart::vec3d(0.0, 0.0, zOffset),
					(trail.position[p + 1] - endToEdge) * effectScale + pixelpart::vec3d(0.0, 0.0, zOffset),
					(trail.position[p] + trail.position[p + 1]) * 0.5 * effectScale + pixelpart::vec3d(0.0, 0.0, zOffset)
				};

				vertices[vertexIndex + p * 5 + 0] = toUnity(vertexPositions[0]);
				vertices[vertexIndex + p * 5 + 1] = toUnity(vertexPositions[1]);
				vertices[vertexIndex + p * 5 + 2] = toUnity(vertexPositions[2]);
				vertices[vertexIndex + p * 5 + 3] = toUnity(vertexPositions[3]);
				vertices[vertexIndex + p * 5 + 4] = toUnity(vertexPositions[4]);
			}

			for(int32_t p = 0; p < numTrailSegments; p++) {
				colors[vertexIndex + p * 5 + 0] = toUnityColor(trail.color[p]);
				colors[vertexIndex + p * 5 + 1] = toUnityColor(trail.color[p]);
				colors[vertexIndex + p * 5 + 2] = toUnityColor(trail.color[p + 1]);
				colors[vertexIndex + p * 5 + 3] = toUnityColor(trail.color[p + 1]);
				colors[vertexIndex + p * 5 + 4] = toUnityColor((trail.color[p] + trail.color[p + 1]) * 0.5);
			}

			switch(particleType.trailRendererSettings.textureRotation) {
				case 1u:
					for(int32_t p = 0; p < numTrailSegments; p++) {
						uv[vertexIndex + p * 5 + 0] = Vector2{ static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[vertexIndex + p * 5 + 1] = Vector2{ static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[vertexIndex + p * 5 + 2] = Vector2{ static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[vertexIndex + p * 5 + 3] = Vector2{ static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[vertexIndex + p * 5 + 4] = Vector2{ static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor), 0.5f };
					}
					break;
				case 2u:
					for(int32_t p = 0; p < numTrailSegments; p++) {
						uv[vertexIndex + p * 5 + 0] = Vector2{ 1.0f, 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 1] = Vector2{ 0.0f, 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 2] = Vector2{ 1.0f, 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 3] = Vector2{ 0.0f, 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 4] = Vector2{ 0.5f, 1.0f - static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor) };
					}
					break;
				case 3u:
					for(int32_t p = 0; p < numTrailSegments; p++) {
						uv[vertexIndex + p * 5 + 0] = Vector2{ 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[vertexIndex + p * 5 + 1] = Vector2{ 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[vertexIndex + p * 5 + 2] = Vector2{ 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 0.0f };
						uv[vertexIndex + p * 5 + 3] = Vector2{ 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor), 1.0f };
						uv[vertexIndex + p * 5 + 4] = Vector2{ 1.0f - static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor), 0.5f };
					}
					break;
				default:
					for(int32_t p = 0; p < numTrailSegments; p++) {
						uv[vertexIndex + p * 5 + 0] = Vector2{ 0.0f, static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 1] = Vector2{ 1.0f, static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 2] = Vector2{ 0.0f, static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 3] = Vector2{ 1.0f, static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor) };
						uv[vertexIndex + p * 5 + 4] = Vector2{ 0.5f, static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor) };
					}
					break;
			}

			for(int32_t p = 0; p < numTrailSegments; p++) {
				uv2[vertexIndex + p * 5 + 0] = toUnity(trail.velocity[p], 0.0);
				uv2[vertexIndex + p * 5 + 1] = toUnity(trail.velocity[p], 0.0);
				uv2[vertexIndex + p * 5 + 2] = toUnity(trail.velocity[p + 1], 0.0);
				uv2[vertexIndex + p * 5 + 3] = toUnity(trail.velocity[p + 1], 0.0);
				uv2[vertexIndex + p * 5 + 4] = toUnity((trail.velocity[p] + trail.velocity[p + 1]) * 0.5, 0.0);
			}

			for(int32_t p = 0; p < numTrailSegments; p++) {
				uv3[vertexIndex + p * 5 + 0] = toUnity(trail.force[p], 0.0);
				uv3[vertexIndex + p * 5 + 1] = toUnity(trail.force[p], 0.0);
				uv3[vertexIndex + p * 5 + 2] = toUnity(trail.force[p + 1], 0.0);
				uv3[vertexIndex + p * 5 + 3] = toUnity(trail.force[p + 1], 0.0);
				uv3[vertexIndex + p * 5 + 4] = toUnity((trail.force[p] + trail.force[p + 1]) * 0.5, 0.0);
			}

			for(int32_t p = 0; p < numTrailSegments; p++) {
				uv4[vertexIndex + p * 5 + 0] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[vertexIndex + p * 5 + 1] = Vector4{ static_cast<float>(trail.life[p]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[vertexIndex + p * 5 + 2] = Vector4{ static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[vertexIndex + p * 5 + 3] = Vector4{ static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first), 0.0f, 0.0f };
				uv4[vertexIndex + p * 5 + 4] = Vector4{ static_cast<float>(trail.life[p] + trail.life[p + 1]) * 0.5f, static_cast<float>(entry.first), 0.0f, 0.0f };
			}

			elementIndex += numTrailSegments * 12;
			vertexIndex += numTrailSegments * 5;
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