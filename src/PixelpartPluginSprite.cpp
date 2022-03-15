#include "PixelpartPlugin.h"

extern "C" {
UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartFindSprite(PixelpartNativeEffect* nativeEffect, const char* buffer) {
	if(!nativeEffect) {
		return pixelpart::NullId;
	}

	return nativeEffect->project.effect.findSpriteByName(std::string(buffer));
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartSpriteGetName(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, char* buffer, int32_t length) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex) || length < 2) {
		return 0;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);	
	if(sprite.name.empty()) {
		return 0;
	}

	std::size_t size = std::min(sprite.name.size(), static_cast<std::size_t>(length) - 1);
	strncpy(buffer, sprite.name.c_str(), size);
	buffer[size] = '\0';

	return static_cast<int32_t>(size);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.lifetimeStart = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, float value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.lifetimeDuration = value;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, bool value) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.repeat = value;
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLifetimeStart(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getSprite(spriteIndex).lifetimeStart);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLifetimeDuration(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return 0.0f;
	}

	return static_cast<float>(nativeEffect->project.effect.getSprite(spriteIndex).lifetimeDuration);
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteGetRepeat(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return false;
	}

	return nativeEffect->project.effect.getSprite(spriteIndex).repeat;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteIsActive(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	return nativeEffect->particleEngine.getTime() >= sprite.lifetimeStart &&
		(nativeEffect->particleEngine.getTime() <= sprite.lifetimeStart + sprite.lifetimeDuration || sprite.repeat);
}

UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API PixelpartSpriteGetLocalTime(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return 0.0f;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return static_cast<float>(std::fmod(nativeEffect->particleEngine.getTime() - sprite.lifetimeStart, sprite.lifetimeDuration) / sprite.lifetimeDuration);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.blendMode = static_cast<pixelpart::BlendMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetColorMode(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, int32_t mode) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.colorMode = static_cast<pixelpart::ColorMode>(mode);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetLayer(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, uint32_t layer) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.layer = layer;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartSpriteSetVisible(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex, bool visible) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	sprite.visible = visible;
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartSpriteGetBlendMode(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return static_cast<int32_t>(pixelpart::BlendMode::normal);
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	return static_cast<float>(sprite.blendMode);
}

UNITY_INTERFACE_EXPORT int32_t UNITY_INTERFACE_API PixelpartSpriteGetColorMode(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return static_cast<int32_t>(pixelpart::ColorMode::multiply);
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	return static_cast<float>(sprite.colorMode);
}

UNITY_INTERFACE_EXPORT uint32_t UNITY_INTERFACE_API PixelpartSpriteGetLayer(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return 0;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	return sprite.layer;
}

UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API PixelpartSpriteIsVisible(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return false;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);

	return sprite.visible;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec2d>* UNITY_INTERFACE_API PixelpartSpriteGetMotionPath(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.motionPath;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetWidth(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.width;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetHeight(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.height;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetOrientation(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.orientation;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::vec4d>* UNITY_INTERFACE_API PixelpartSpriteGetColor(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.color;
}

UNITY_INTERFACE_EXPORT pixelpart::Curve<pixelpart::floatd>* UNITY_INTERFACE_API PixelpartSpriteGetOpacity(PixelpartNativeEffect* nativeEffect, uint32_t spriteIndex) {
	if(!nativeEffect || !nativeEffect->project.effect.hasSprite(spriteIndex)) {
		return nullptr;
	}

	pixelpart::Sprite& sprite = nativeEffect->project.effect.getSprite(spriteIndex);
	
	return &sprite.opacity;
}
}