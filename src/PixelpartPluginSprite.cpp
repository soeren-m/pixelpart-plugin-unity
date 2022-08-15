#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindSprite(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	std::string name(buffer);
	if(!nativeEffect || !nativeEffect->project.effect.sprites.containsWithName(name)) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.sprites.getByName(name).id;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindSpriteByIndex(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || spriteIndex >= nativeEffect->project.effect.sprites.getCount()) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.sprites.getByIndex(spriteIndex).id;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartSpriteGetName(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId) || length < 2) {
		return 0;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	if(sprite.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(sprite.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, sprite.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.lifetimeStart = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.lifetimeDuration = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.sprites.get(spriteId).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.sprites.get(spriteId).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	return nativeEffect->project.effect.sprites.get(spriteId).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteIsActive(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return nativeEffect->particleEngine.getTime() >= sprite.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= sprite.lifetimeStart + sprite.lifetimeDuration || sprite.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return 0.0f;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - sprite.lifetimeStart, sprite.lifetimeDuration) / sprite.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetAlignWithPath(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, bool mode) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.alignWithPath = mode;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetPivot(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, float x, float y) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.pivot = pixelpart::vec2d(x, y);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteGetAlignWithPath(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return sprite.alignWithPath;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetPivotX(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return static_cast<float>(sprite.pivot.x);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetPivotY(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return static_cast<float>(sprite.pivot.y);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLayer(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, uint32_t layer) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.layer = layer;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetVisible(PixelpartNativeEffect* nativeEffect, uint32_t spriteId, bool visible) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);
	sprite.visible = visible;
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartSpriteGetLayer(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return 0;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return sprite.layer;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteIsVisible(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return sprite.visible;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartSpriteGetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return static_cast<int32_t>(pixelpart::BlendMode::normal);
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return static_cast<int32_t>(sprite.blendMode);
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.width;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetHeight(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.height;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartSpriteGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.motionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec4d>* UNITY_INTERFACE_API PixelpartSpriteGetColor(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.color;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetOpacity(PixelpartNativeEffect* nativeEffect, uint32_t spriteId) {
	if(!nativeEffect || !nativeEffect->project.effect.sprites.contains(spriteId)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.sprites.get(spriteId);

	return &sprite.opacity;
}
}