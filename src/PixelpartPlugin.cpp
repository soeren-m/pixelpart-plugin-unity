#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API PixelpartLoadEffect(const char* data, int size) {
	if(!data || size <= 0) {
		return nullptr;
	}

	try {
		PixelpartNativeEffect* nativeEffect = new PixelpartNativeEffect();
		nativeEffect->project = pixelpart::deserialize(data, static_cast<std::size_t>(size), nativeEffect->projectResources);
		nativeEffect->particleEngine = pixelpart::ParticleEngine(&(nativeEffect->project.effect), maxNumParticlesPerEmitter);
		nativeEffect->particleMeshBuilders.resize(nativeEffect->project.effect.getNumParticleEmitters());

		return nativeEffect;
	}
	catch(...) {

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

	nativeEffect->particleEngine.onForceFieldUpdate();
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartUpdateEffectCollisionSolver(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return;
	}

	nativeEffect->particleEngine.onColliderUpdate();
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

	return nativeEffect->project.effect.getNumParticleEmitters();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumForceFields(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.getNumForceFields();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumColliders(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.getNumColliders();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumSprites(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.getNumSprites();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectMaxLayer(PixelpartNativeEffect* nativeEffect) {
	if(!nativeEffect) {
		return 0;
	}

	return nativeEffect->project.effect.getMaxLayer();
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetParticleEmitterId(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.getNumParticleEmitters()) {
		return pixelpart::NullId;
	}

	pixelpart::ParticleEmitter& emitter = nativeEffect->project.effect.getParticleEmitterByIndex(emitterIndex);

	return emitter.id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartGetEffectNumParticles(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.getNumParticleEmitters()) {
		return 0;
	}

	return nativeEffect->particleEngine.getNumParticles(emitterIndex);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetParticleEmittersSortedByLayer(PixelpartNativeEffect* nativeEffect, uint32_t* indices) {
	if(!nativeEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = nativeEffect->project.effect.getParticleEmittersSortedByLayer();
	for(uint32_t i = 0; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartGetSpritesSortedByLayer(PixelpartNativeEffect* nativeEffect, uint32_t* indices) {
	if(!nativeEffect) {
		return;
	}

	std::vector<uint32_t> sortedIndices = nativeEffect->project.effect.getSpritesSortedByLayer();
	for(uint32_t i = 0; i < sortedIndices.size(); i++) {
		indices[i] = sortedIndices[i];
	}
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetEffectParticleImageId(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex, char* buffer, int32_t length) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.getNumParticleEmitters() || length < 2) {
		return 0;
	}

	const std::string& imageId = nativeEffect->project.effect.getParticleEmitterByIndex(emitterIndex).particleSprite.id;
	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(imageId);

	std::size_t size = std::min(imageId.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, imageId.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartGetEffectSpriteImageId(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex) || length < 2) {
		return 0;
	}

	const std::string& imageId = nativeEffect->project.effect.getSprite(spriteIndex).image.id;
	const pixelpart::ImageResource& image = nativeEffect->projectResources.images.at(imageId);

	std::size_t size = std::min(imageId.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, imageId.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartPrepareParticleMeshBuild(PixelpartNativeEffect* nativeEffect, uint32_t emitterIndex) {
	if(!nativeEffect || emitterIndex >= nativeEffect->project.effect.getNumParticleEmitters()) {
		return false;
	}

	nativeEffect->particleMeshBuilders[emitterIndex].update(
		nativeEffect->project.effect.getParticleEmitterByIndex(emitterIndex),
		nativeEffect->particleEngine.getParticles(emitterIndex),
		nativeEffect->particleEngine.getNumParticles(emitterIndex),
		nativeEffect->particleEngine.getTime());
	nativeEffect->activeParticleMeshBuilder = emitterIndex;

	return true;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartPrepareSpriteMeshBuild(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return false;
	}

	const pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	nativeEffect->spriteMeshBuilder = pixelpart::SpriteMeshBuilder(
		sprite,
		nativeEffect->projectResources.images.at(sprite.image.id),
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

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetParticleTriangleData(PixelpartNativeEffect* nativeEffect, float scaleX, float scaleY, int* triangles, float* positions, float* uvs, float* colors) {
	if(!nativeEffect || nativeEffect->activeParticleMeshBuilder >= nativeEffect->particleMeshBuilders.size()) {
		return false;
	}

	nativeEffect->particleMeshBuilders[nativeEffect->activeParticleMeshBuilder].build(
		triangles,
		positions,
		uvs,
		colors,
		scaleX,
		scaleY);

	return true;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartGetSpriteTriangleData(PixelpartNativeEffect* nativeEffect, float scaleX, float scaleY, int* triangles, float* positions, float* uvs, float* colors) {
	if(!nativeEffect || !nativeEffect->spriteMeshBuilder.getSprite()) {
		return false;
	}

	nativeEffect->spriteMeshBuilder.build(
		triangles,
		positions,
		uvs,
		colors,
		scaleX,
		scaleY);

	return true;
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