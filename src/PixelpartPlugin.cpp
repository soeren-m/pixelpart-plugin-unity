#include "PixelpartPlugin.h"
#include "PixelpartShaderGraph.h"
#include <locale>

extern "C" {
static bool initialized = false;
static uint32_t maxNumParticlesPerEmitter = 10000;

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSetMaxNumParticlesPerEmitter(uint32_t value) {
	maxNumParticlesPerEmitter = value;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetMaxNumParticlesPerEmitter() {
	return maxNumParticlesPerEmitter;
}

UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const char* data, int32_t size) {
	if(!initialized) {
		try {
			nlohmann::ordered_json modelJson = nlohmann::json::parse(
				PixelpartShaderGraph_json,
				PixelpartShaderGraph_json + PixelpartShaderGraph_json_size);

			pixelpart::ShaderGraph::graphType = modelJson;
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
		nativeEffect->particleEngine = pixelpart::ParticleEngine(&(nativeEffect->project.effect), maxNumParticlesPerEmitter);
		nativeEffect->particleMeshBuilders.resize(nativeEffect->project.effect.particleEmitters.getCount());

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
		nativeEffect->simulationTime += std::min(dt, 1.0f) * nativeEffect->speed;

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

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateEffectForceSolver(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.updateForceSolver();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateEffectCollisionSolver(PixelpartNativeEffect* nativeEffect) {
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

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumSprites(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.sprites.getCount();
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

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectMaxLayer(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	uint32_t maxLayer = 0;
	for(const pixelpart::ParticleEmitter& emitter : nativeEffect->project.effect.particleEmitters) {
		maxLayer = std::max(maxLayer, emitter.layer);
	}
	for(const pixelpart::Sprite& sprite : nativeEffect->project.effect.sprites) {
		maxLayer = std::max(maxLayer, sprite.layer);
	}

	return maxLayer;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.particleEmitters.getCount()) {
		return 0;
	}

	return nativeEffect->particleEngine.getNumParticles(emitterIndex);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetParticleEmittersSortedByLayer(PixelpartNativeEffect* nativeEffect, uint32_t* indices) {
	if(!nativeEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = nativeEffect->project.effect.particleEmitters.getSortedIndices([](const pixelpart::ParticleEmitter& emitter1, const pixelpart::ParticleEmitter& emitter2) {
		return emitter1.layer < emitter2.layer;
	});

	for(uint32_t i = 0; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetSpritesSortedByLayer(PixelpartNativeEffect* nativeEffect, uint32_t* indices) {
	if(!nativeEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = nativeEffect->project.effect.sprites.getSortedIndices([](const pixelpart::Sprite& sprite1, const pixelpart::Sprite& sprite2) {
		return sprite1.layer < sprite2.layer;
	});

	for(uint32_t i = 0; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartBuildParticleShader(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex, char* bufferCode, char* bufferTextureIds, int32_t* outLengthCode, int32_t* outLengthTextureIds, int32_t bufferSizeCode, int32_t bufferSizeTexturesIds) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.containsIndex(emitterIndex) || !bufferCode || !bufferTextureIds || !outLengthCode || !outLengthTextureIds || bufferSizeCode < 2 || bufferSizeTexturesIds < 2) {
		return false;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.particleEmitters.getByIndex(emitterIndex);

	try {
		std::locale::global(std::locale::classic());

		pixelpart::ShaderGraph::BuildResult buildResult;
		emitter.particleShader.build(buildResult);

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

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartBuildSpriteShader(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, char* bufferCode, char* bufferTextureIds, int32_t* outLengthCode, int32_t* outLengthTextureIds, int32_t bufferSizeCode, int32_t bufferSizeTexturesIds) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.containsIndex(spriteIndex) || !bufferCode || !bufferTextureIds || !outLengthCode || !outLengthTextureIds || bufferSizeCode < 2 || bufferSizeTexturesIds < 2) {
		return 0;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.getByIndex(spriteIndex);

	try {
		std::locale::global(std::locale::classic());

		pixelpart::ShaderGraph::BuildResult buildResult;
		sprite.shader.build(buildResult);

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

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartPrepareParticleMeshBuild(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.particleEmitters.containsIndex(emitterIndex)) {
		return false;
	}

	nativeEffect->particleMeshBuilders[emitterIndex].update(
		nativeEffect->project.effect.particleEmitters.getByIndex(emitterIndex),
		nativeEffect->particleEngine.getParticles(emitterIndex),
		nativeEffect->particleEngine.getNumParticles(emitterIndex),
		nativeEffect->particleEngine.getTime());
	nativeEffect->activeParticleMeshBuilder = emitterIndex;

	return true;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartPrepareSpriteMeshBuild(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.containsIndex(spriteIndex)) {
		return false;
	}

	nativeEffect->spriteMeshBuilder = pixelpart::SpriteMeshBuilder(
		nativeEffect->project.effect.sprites.getByIndex(spriteIndex),
		nativeEffect->particleEngine.getTime());

	return true;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetParticleMeshNumTriangles(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect || nativeEffect->activeParticleMeshBuilder >= nativeEffect->particleMeshBuilders.size()) {
		return 0;
	}

	return nativeEffect->particleMeshBuilders[nativeEffect->activeParticleMeshBuilder].getNumIndices() / 3;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetParticleMeshNumVertices(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect || nativeEffect->activeParticleMeshBuilder >= nativeEffect->particleMeshBuilders.size()) {
		return 0;
	}

	return nativeEffect->particleMeshBuilders[nativeEffect->activeParticleMeshBuilder].getNumVertices();
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetParticleTriangleData(PixelpartNativeEffect* nativeEffect, float scaleX, float scaleY, int* triangles, float* positions, float* textureCoords, float* colors, float* velocities, float* forces, float* lives, int* ids) {
	if(!nativeEffect || nativeEffect->activeParticleMeshBuilder >= nativeEffect->particleMeshBuilders.size()) {
		return false;
	}

	nativeEffect->particleMeshBuilders[nativeEffect->activeParticleMeshBuilder].build(
		triangles,
		positions,
		textureCoords,
		colors,
		velocities,
		forces,
		lives,
		ids,
		scaleX,
		scaleY);

	return true;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetSpriteTriangleData(PixelpartNativeEffect* nativeEffect, float scaleX, float scaleY, int* triangles, float* positions, float* textureCoords, float* colors, float* lives) {
	if(!nativeEffect || !nativeEffect->spriteMeshBuilder.getSprite()) {
		return false;
	}

	nativeEffect->spriteMeshBuilder.build(
		triangles,
		positions,
		textureCoords,
		colors,
		lives,
		scaleX,
		scaleY);

	return true;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectResourceImageCount(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return static_cast<uint32_t>(nativeEffect->projectResources.images.size());
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetEffectResourceImageId(PixelpartNativeEffect* nativeEffect, uint32_t index, char* imageIdBuffer, int32_t bufferLength) {
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

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectResourceImageWidth(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return image.width;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectResourceImageHeight(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return image.height;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectResourceImageDataSize(PixelpartNativeEffect* nativeEffect, const char* imageId) {
	if(!nativeEffect || !imageId) {
		return 0;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	return static_cast<uint32_t>(image.data.size());
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetEffectResourceImageData(PixelpartNativeEffect* nativeEffect, const char* imageId, unsigned char* imageData) {
	if(!nativeEffect || !imageId) {
		return;
	}

	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(std::string(imageId));

	memcpy(imageData, image.data.data(), image.data.size());
}
}