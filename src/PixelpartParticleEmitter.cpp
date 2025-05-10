#include "PixelpartPluginCommon.h"
#include "PixelpartEffectRuntime.h"
#include "PixelpartUnityTypes.h"
#include "PixelpartUtil.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include <vector>
#include <algorithm>

namespace internal {
void setParticleEmitterShapePoints(pixelpart::ParticleEmitter& particleEmitter, const std::vector<pixelpart::float3_t>& points) {
	std::vector<pixelpart::float_t> distances(points.size(), 0.0);
	pixelpart::float_t length = 0.0;

	for(std::size_t i = 1; i < points.size(); i++) {
		length += std::max(glm::distance(points[i], points[i - 1]), 0.000001);
		distances[i] = length;
	}

	pixelpart::Curve<pixelpart::float3_t> modifiedPath;
	for(std::size_t i = 0; i < points.size(); i++) {
		modifiedPath.addPoint(distances[i] / length, points[i]);
	}

	particleEmitter.path() = modifiedPath;
}

std::vector<pixelpart::float3_t> getParticleEmitterShapePoints(const pixelpart::ParticleEmitter& particleEmitter) {
	std::vector<pixelpart::float3_t> points(particleEmitter.path().pointCount(), pixelpart::float3_t(0.0));
	for(std::size_t i = 0; i < particleEmitter.path().pointCount(); i++) {
		points[i] = particleEmitter.path().point(i).value;
	}

	return points;
}
}

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt shape) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.shape(static_cast<pixelpart::ParticleEmitter::Shape>(shape));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::Shape::point);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.shape());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::Shape::point);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterAddShapePoint(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityVector3 point) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = internal::getParticleEmitterShapePoints(emitter);
		points.push_back(internal::fromUnity(point));

		internal::setParticleEmitterShapePoints(emitter, points);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterRemoveShapePoint(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt index) {
	if(!effectRuntime || index < 0) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = internal::getParticleEmitterShapePoints(emitter);
		points.erase(points.begin() + index);

		internal::setParticleEmitterShapePoints(emitter, points);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShapePoint(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt index, UnityVector3 point) {
	if(!effectRuntime || index < 0) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = internal::getParticleEmitterShapePoints(emitter);
		points[index] = internal::fromUnity(point);

		internal::setParticleEmitterShapePoints(emitter, points);
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetShapePointCount(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return 0;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.path().pointCount());
	}
	catch(...) {
		return 0;
	}
}

UNITY_INTERFACE_EXPORT UnityVector3 UNITY_INTERFACE_API PixelpartParticleEmitterGetShapePoint(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt index) {
	if(!effectRuntime || index < 0) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return internal::toUnity(emitter.path().point(static_cast<std::size_t>(index)).value);
	}
	catch(...) {
		return UnityVector3{ 0.0f, 0.0f, 0.0f };
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDistribution(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.distribution(static_cast<pixelpart::ParticleEmitter::Distribution>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.distribution());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::Distribution::uniform);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridOrder(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.gridOrder(static_cast<pixelpart::ParticleEmitter::GridOrder>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetGridOrder(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.gridOrder());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridSize(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt x, UnityInt y, UnityInt z) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.gridSize(
			static_cast<UnityUInt>(std::max(x, 1)),
			static_cast<UnityUInt>(std::max(y, 1)),
			static_cast<UnityUInt>(std::max(z, 1)));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeX(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.gridSizeX());
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeY(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.gridSizeY());
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeZ(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.gridSizeZ());
	}
	catch(...) {
		return 1;
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetEmissionMode(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.emissionMode(static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetEmissionMode(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.emissionMode());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDirectionMode(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId, UnityInt mode) {
	if(!effectRuntime) {
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.directionMode(static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode));
	}
	catch(...) {
		return;
	}
}

UNITY_INTERFACE_EXPORT UnityInt UNITY_INTERFACE_API PixelpartParticleEmitterGetDirectionMode(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<UnityInt>(emitter.directionMode());
	}
	catch(...) {
		return static_cast<UnityInt>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return &emitter.direction();
	}
	catch(...) {
		return nullptr;
	}
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(PixelpartEffectRuntime* effectRuntime, UnityUInt emitterId) {
	if(!effectRuntime) {
		return nullptr;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return &emitter.spread();
	}
	catch(...) {
		return nullptr;
	}
}
}