using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
internal static class Plugin {
#if !UNITY_EDITOR && (UNITY_IPHONE || UNITY_WEBGL || UNITY_SWITCH)
	private const string pluginName = "__Internal";
#else
	private const string pluginName = "pixelpart";
#endif

	[DllImport(pluginName)]
	public static extern void PixelpartSetParticleCapacity(int value);
	[DllImport(pluginName)]
	public static extern int PixelpartGetParticleCapacity();
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartLoadEffect(byte[] data, int size);
	[DllImport(pluginName)]
	public static extern void PixelpartDeleteEffect(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateEffect(IntPtr nativeEffect, float dt);
	[DllImport(pluginName)]
	public static extern void PixelpartRefreshParticleSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartRefreshForceSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartRefreshCollisionSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartPlayEffect(IntPtr nativeEffect, bool state);
	[DllImport(pluginName)]
	public static extern void PixelpartRestartEffect(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartResetEffect(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern bool PixelpartIsEffect3D(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectLoop(IntPtr nativeEffect, bool loop);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectLoopTime(IntPtr nativeEffect, float loopTime);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectSpeed(IntPtr nativeEffect, float speed);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectTimeStep(IntPtr nativeEffect, float timeStep);
	[DllImport(pluginName)]
	public static extern bool PixelpartIsEffectPlaying(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetEffectLoop(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectLoopTime(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectSpeed(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectTimeStep(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectTime(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticleEmitters(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticleTypes(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumForceFields(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumColliders(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticles(IntPtr nativeEffect, uint particleTypeIndex);
	[DllImport(pluginName)]
	public static extern void PixelpartGetParticleTypesSortedForRendering(IntPtr nativeEffect, uint[] indices);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildParticleShader(IntPtr nativeEffect, uint particleTypeIndex, byte[] bufferCode, byte[] bufferTextureIds, out int outLengthCode, out int outLengthTextureIds, int bufferSizeCode, int bufferSizeTexturesIds);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleSpriteVertexData(IntPtr nativeEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleTrailVertexData(IntPtr nativeEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleMeshVertexData(IntPtr nativeEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleSpriteVertexData(IntPtr nativeEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3, [In, Out] Vector4[] uv4);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleTrailVertexData(IntPtr nativeEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3, [In, Out] Vector4[] uv4);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleMeshVertexData(IntPtr nativeEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3, [In, Out] Vector4[] uv4);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetImageResourceCount(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceId(IntPtr nativeEffect, uint index, byte[] imageIdBuffer, int bufferLength);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceWidth(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceHeight(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetImageResourceDataSize(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern void PixelpartGetImageResourceData(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId, byte[] imageData);
	[DllImport(pluginName)]
	public static extern void PixelpartSpawnParticles(IntPtr nativeEffect, uint particleTypeId, int count);

	// ParticleEmitter
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitter(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitterByIndex(IntPtr nativeEffect, uint emitterIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasParticleEmitter(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetName(IntPtr nativeEffect, uint emitterId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern uint PixelpartParticleEmitterGetParentId(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetLifetimeStart(IntPtr nativeEffect, uint emitterId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetLifetimeDuration(IntPtr nativeEffect, uint emitterId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetRepeat(IntPtr nativeEffect, uint emitterId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLifetimeStart(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLifetimeDuration(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleEmitterGetRepeat(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleEmitterIsActive(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLocalTime(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetPosition(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetShape(IntPtr nativeEffect, uint emitterId, int shape);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetShape(IntPtr nativeEffect, uint emitterId);
	// TODO
	/*[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetPath(IntPtr nativeEffect, uint emitterId);*/
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSize(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetOrientation(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDistribution(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDistribution(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetGridOrder(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridOrder(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetGridSize(IntPtr nativeEffect, uint emitterId, int width, int height, int depth);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridWidth(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridHeight(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridDepth(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetEmissionMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetEmissionMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDirectionMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDirectionMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetDirection(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSpread(IntPtr nativeEffect, uint emitterId);

	// ParticleType
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleType(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleTypeByIndex(IntPtr nativeEffect, uint particleTypeIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasParticleType(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetName(IntPtr nativeEffect, uint particleTypeId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern uint PixelpartParticleTypeGetParentId(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetPosition(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetNumParticles(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetLifespan(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetLifespanVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetPositionRelative(IntPtr nativeEffect, uint particleTypeId, bool relative);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeGetPositionRelative(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetMotionPathForce(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialVelocity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInheritedVelocity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetVelocityVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetAcceleration(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRadialAcceleration(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetRotationMode(IntPtr nativeEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRotationMode(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetAlignmentMode(IntPtr nativeEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetAlignmentMode(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialRotation(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotation(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotationBySpeed(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotationVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetAngularVelocityVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetPivot(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetWeight(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetBounce(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetFriction(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetVisible(IntPtr nativeEffect, uint particleTypeId, bool visible);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeIsVisible(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetLayer(IntPtr nativeEffect, uint particleTypeId, int layer);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetLayer(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialSize(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetSize(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetSizeVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetStretch(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetColor(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetColorVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialOpacity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetOpacity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetOpacityVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRenderer(IntPtr nativeEffect, uint particleTypeId);

	// ForceField
	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceField(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceFieldByIndex(IntPtr nativeEffect, uint forceFieldIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasForceField(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetName(IntPtr nativeEffect, uint forceFieldId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetLifetimeStart(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetLifetimeDuration(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetRepeat(IntPtr nativeEffect, uint forceFieldId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLifetimeStart(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLifetimeDuration(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldGetRepeat(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldIsActive(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLocalTime(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetPosition(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetType(IntPtr nativeEffect, uint forceFieldId, int type);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetType(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetSize(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetOrientation(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetStrength(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationDirection(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationDirectionVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationStrengthVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetAccelerationGridSize(IntPtr nativeEffect, uint forceFieldId, int width, int height, int depth);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridWidth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridHeight(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridDepth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetVectorFilter(IntPtr nativeEffect, uint forceFieldId, int filter);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetVectorFilter(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridDepth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetVectorTightness(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseOctaves(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseOctaves(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseFrequency(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoisePersistence(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseLacunarity(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetNoiseAnimated(IntPtr nativeEffect, uint forceFieldId, bool animated);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldGetNoiseAnimated(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseAnimationTimeScale(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseAnimationTimeBase(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetDragVelocityInfluence(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetDragSizeInfluence(IntPtr nativeEffect, uint forceFieldId);

	// Collider
	[DllImport(pluginName)]
	public static extern uint PixelpartFindCollider(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindColliderByIndex(IntPtr nativeEffect, uint colliderIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasCollider(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern int PixelpartColliderGetName(IntPtr nativeEffect, uint colliderId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetLifetimeStart(IntPtr nativeEffect, uint colliderId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetLifetimeDuration(IntPtr nativeEffect, uint colliderId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetRepeat(IntPtr nativeEffect, uint colliderId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLifetimeStart(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLifetimeDuration(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern bool PixelpartColliderGetRepeat(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern bool PixelpartColliderIsActive(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLocalTime(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderAddPoint(IntPtr nativeEffect, uint colliderId, Vector3 point);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetPoint(IntPtr nativeEffect, uint colliderId, int index, Vector3 point);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderRemovePoint(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointX(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointY(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointZ(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartColliderGetNumPoints(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetWidth(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetOrientation(IntPtr nativeEffect, uint colliderId);
	// TODO: kill on contact
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetBounce(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetFriction(IntPtr nativeEffect, uint colliderId);

	// StaticPropertyInt
	[DllImport(pluginName)]
	public static extern int PixelpartStaticPropertyIntGet(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyIntSetValue(IntPtr property, int value);
	[DllImport(pluginName)]
	public static extern int PixelpartStaticPropertyIntGetValue(IntPtr property);

	// StaticPropertyFloat
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloatGet(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyFloatSetValue(IntPtr property, float value);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloatGetValue(IntPtr property);

	// StaticPropertyFloat2
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat2GetX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat2GetY(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyFloat2SetValue(IntPtr property, Vector2 value);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat2GetValueX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat2GetValueY(IntPtr property);

	// StaticPropertyFloat3
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetY(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetZ(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyFloat3SetValue(IntPtr property, Vector3 value);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetValueX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetValueY(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat3GetValueZ(IntPtr property);

	// StaticPropertyFloat4
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetY(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetZ(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetW(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyFloat4SetValue(IntPtr property, Vector4 value);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetValueX(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetValueY(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetValueZ(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartStaticPropertyFloat4GetValueW(IntPtr property);

	// AnimatedPropertyInt
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyIntGet(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntAddPoint(IntPtr property, float position, int value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntRemovePoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntSetPoint(IntPtr property, int index, int value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntSetPointPosition(IntPtr property, int index, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntClear(IntPtr property);
	[DllImport(pluginName)]
	public static extern bool PixelpartAnimatedPropertyIntContainsPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyIntGetNumPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyIntGetPoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyIntGetPointIndex(IntPtr property, float position, float epsilon);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntSetInterpolation(IntPtr property, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyIntGetInterpolation(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntEnableAdaptiveCache(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyIntEnableFixedCache(IntPtr property, int size);

	// AnimatedPropertyFloat
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloatGet(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatAddPoint(IntPtr property, float position, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatRemovePoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatSetPoint(IntPtr property, int index, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatSetPointPosition(IntPtr property, int index, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatClear(IntPtr property);
	[DllImport(pluginName)]
	public static extern bool PixelpartAnimatedPropertyFloatContainsPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloatGetNumPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloatGetPoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloatGetPointIndex(IntPtr property, float position, float epsilon);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatSetInterpolation(IntPtr property, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloatGetInterpolation(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatEnableAdaptiveCache(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloatEnableFixedCache(IntPtr property, int size);

	// AnimatedPropertyFloat2
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat2GetX(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat2GetY(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2AddPoint(IntPtr property, float position, Vector2 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2RemovePoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2SetPoint(IntPtr property, int index, Vector2 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2SetPointPosition(IntPtr property, int index, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2Clear(IntPtr property);
	[DllImport(pluginName)]
	public static extern bool PixelpartAnimatedPropertyFloat2ContainsPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat2GetNumPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat2GetPointX(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat2GetPointY(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat2GetPointIndex(IntPtr property, float position, float epsilon);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2SetInterpolation(IntPtr property, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat2GetInterpolation(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat2EnableFixedCache(IntPtr property, int size);

	// AnimatedPropertyFloat3
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetX(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetY(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetZ(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3AddPoint(IntPtr property, float position, Vector3 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3RemovePoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3SetPoint(IntPtr property, int index, Vector3 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3SetPointPosition(IntPtr property, int index, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3Clear(IntPtr property);
	[DllImport(pluginName)]
	public static extern bool PixelpartAnimatedPropertyFloat3ContainsPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat3GetNumPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetPointX(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetPointY(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat3GetPointZ(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat3GetPointIndex(IntPtr property, float position, float epsilon);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3SetInterpolation(IntPtr property, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat3GetInterpolation(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat3EnableFixedCache(IntPtr property, int size);

	// AnimatedPropertyFloat4
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetX(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetY(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetZ(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetW(IntPtr property, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4AddPoint(IntPtr property, float position, Vector4 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4RemovePoint(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4SetPoint(IntPtr property, int index, Vector4 value);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4SetPointPosition(IntPtr property, int index, float position);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4Clear(IntPtr property);
	[DllImport(pluginName)]
	public static extern bool PixelpartAnimatedPropertyFloat4ContainsPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat4GetNumPoints(IntPtr property);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetPointX(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetPointY(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetPointZ(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartAnimatedPropertyFloat4GetPointW(IntPtr property, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat4GetPointIndex(IntPtr property, float position, float epsilon);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4SetInterpolation(IntPtr property, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartAnimatedPropertyFloat4GetInterpolation(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartAnimatedPropertyFloat4EnableFixedCache(IntPtr property, int size);
}
}