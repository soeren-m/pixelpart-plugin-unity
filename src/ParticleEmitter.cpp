#include "Common.h"
#include "Error.h"
#include "EffectRuntime.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/effect/AnimatedProperty.h"
#include "pixelpart-runtime/effect/ParticleEmitter.h"
#include <string>
#include <vector>
#include <exception>
#include <algorithm>

namespace pixelpart_unity {
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
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShape(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t shape) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.shape(static_cast<pixelpart::ParticleEmitter::Shape>(shape));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShape(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::Shape::point);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.shape());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::Shape::point);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterAddShapePoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::vector3_t point) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = pixelpart_unity::getParticleEmitterShapePoints(emitter);
		points.push_back(pixelpart_unity::fromUnity(point));

		pixelpart_unity::setParticleEmitterShapePoints(emitter, points);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterRemoveShapePoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t index) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = pixelpart_unity::getParticleEmitterShapePoints(emitter);
		points.erase(points.begin() + static_cast<std::size_t>(index));

		pixelpart_unity::setParticleEmitterShapePoints(emitter, points);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetShapePoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t index, pixelpart_unity::vector3_t point) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		std::vector<pixelpart::float3_t> points = pixelpart_unity::getParticleEmitterShapePoints(emitter);
		points.at(static_cast<std::size_t>(index)) = pixelpart_unity::fromUnity(point);

		pixelpart_unity::setParticleEmitterShapePoints(emitter, points);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShapePointCount(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 0;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.path().pointCount());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 0;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::vector3_t UNITY_INTERFACE_API PixelpartParticleEmitterGetShapePoint(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t index) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}
	else if(index < 0) {
		pixelpart_unity::lastError = pixelpart_unity::invalidArgumentError;
		return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return pixelpart_unity::toUnity(emitter.path().point(static_cast<std::size_t>(index)).value);
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return pixelpart_unity::vector3_t{ 0.0f, 0.0f, 0.0f };
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDistribution(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.distribution(static_cast<pixelpart::ParticleEmitter::Distribution>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDistribution(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::Distribution::uniform);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.distribution());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::Distribution::uniform);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridOrder(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.gridOrder(static_cast<pixelpart::ParticleEmitter::GridOrder>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridOrder(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.gridOrder());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::GridOrder::x_y_z);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetGridSize(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t x, pixelpart_unity::int_t y, pixelpart_unity::int_t z) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.gridSize(
			static_cast<pixelpart_unity::uint_t>(std::max(x, 1)),
			static_cast<pixelpart_unity::uint_t>(std::max(y, 1)),
			static_cast<pixelpart_unity::uint_t>(std::max(z, 1)));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeX(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.gridSizeX());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeY(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.gridSizeY());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetGridSizeZ(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return 1;
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.gridSizeZ());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return 1;
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetEmissionMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.emissionMode(static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetEmissionMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::EmissionMode::continuous);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.emissionMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::EmissionMode::continuous);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API PixelpartParticleEmitterSetDirectionMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId, pixelpart_unity::int_t mode) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		emitter.directionMode(static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode));
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}
}

UNITY_INTERFACE_EXPORT pixelpart_unity::int_t UNITY_INTERFACE_API PixelpartParticleEmitterGetDirectionMode(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::DirectionMode::fixed);
	}

	try {
		const pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return static_cast<pixelpart_unity::int_t>(emitter.directionMode());
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return static_cast<pixelpart_unity::int_t>(pixelpart::ParticleEmitter::DirectionMode::fixed);
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float3_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetDirection(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return &emitter.direction();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}

UNITY_INTERFACE_EXPORT pixelpart::AnimatedProperty<pixelpart::float_t>* UNITY_INTERFACE_API PixelpartParticleEmitterGetSpread(pixelpart_unity::EffectRuntime* effectRuntime, pixelpart_unity::uint_t emitterId) {
	if(!effectRuntime) {
		pixelpart_unity::lastError = pixelpart_unity::invalidEffectRuntimeError;
		return nullptr;
	}

	try {
		pixelpart::ParticleEmitter& emitter =
			effectRuntime->effectAsset.effect().sceneGraph().at<pixelpart::ParticleEmitter>(emitterId);

		return &emitter.spread();
	}
	catch(const std::exception& e) {
		pixelpart_unity::lastError = std::string(e.what());
	}

	return nullptr;
}
}
