using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace pixelpart {
internal static class Plugin {
#if !UNITY_EDITOR && (UNITY_IPHONE || UNITY_WEBGL || UNITY_SWITCH)
	public const string pluginName = "__Internal";
#else
	public const string pluginName = "pixelpart";
#endif

	[DllImport(pluginName)]
	public static extern void PixelpartSetMaxNumParticlesPerEmitter(uint value);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetMaxNumParticlesPerEmitter();
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartLoadEffect(byte[] data, int size);
	[DllImport(pluginName)]
	public static extern void PixelpartDeleteEffect(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateEffect(IntPtr nativeEffect, float dt);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateEffectForceSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateEffectCollisionSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartPlayEffect(IntPtr nativeEffect, bool state);
	[DllImport(pluginName)]
	public static extern void PixelpartRestartEffect(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartResetEffect(IntPtr nativeEffect);
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
	public static extern uint PixelpartGetEffectNumSprites(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumForceFields(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumColliders(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectMaxLayer(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticles(IntPtr nativeEffect, uint emitterIndex);
	[DllImport(pluginName)]
	public static extern void PixelpartGetParticleEmittersSortedByLayer(IntPtr nativeEffect, uint[] indices);
	[DllImport(pluginName)]
	public static extern void PixelpartGetSpritesSortedByLayer(IntPtr nativeEffect, uint[] indices);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildParticleShader(IntPtr nativeEffect, uint emitterIndex, byte[] bufferCode, byte[] bufferTextureIds, out int outLengthCode, out int outLengthTextureIds, int bufferSizeCode, int bufferSizeTexturesIds);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildSpriteShader(IntPtr nativeEffect, uint spriteIndex, byte[] bufferCode, byte[] bufferTextureIds, out int outLengthCode, out int outLengthTextureIds, int bufferSizeCode, int bufferSizeTexturesIds);
	[DllImport(pluginName)]
	public static extern bool PixelpartPrepareParticleMeshBuild(IntPtr nativeEffect, uint emitterIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartPrepareSpriteMeshBuild(IntPtr nativeEffect, uint spriteIndex);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetParticleMeshNumTriangles(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetParticleMeshNumVertices(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleTriangleData(IntPtr nativeEffect, float scaleX, float scaleY, int[] triangles, float[] positions, float[] textureCoords, float[] colors, float[] velocities, float[] forces, float[] lives, int[] ids);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetSpriteTriangleData(IntPtr nativeEffect, float scaleX, float scaleY, int[] triangles, float[] positions, float[] textureCoords, float[] colors, float[] lives);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectResourceImageCount(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetEffectResourceImageId(IntPtr nativeEffect, uint index, byte[] imageIdBuffer, int bufferLength);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectResourceImageWidth(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectResourceImageHeight(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectResourceImageDataSize(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId);
	[DllImport(pluginName)]
	public static extern void PixelpartGetEffectResourceImageData(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string imageId, byte[] imageData);

	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitter(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitterByIndex(IntPtr nativeEffect, uint emitterIndex);
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
	public static extern void PixelpartParticleEmitterSetShape(IntPtr nativeEffect, uint emitterId, int type);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDistribution(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetSpawnMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetInstantiationMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetShape(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDistribution(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetSpawnMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetInstantiationMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetLayer(IntPtr nativeEffect, uint emitterId, uint layer);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetVisible(IntPtr nativeEffect, uint emitterId, bool visible);
	[DllImport(pluginName)]
	public static extern uint PixelpartParticleEmitterGetLayer(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleEmitterIsVisible(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetBlendMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetRenderer(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleRotationMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticlePivot(IntPtr nativeEffect, uint emitterId, float x, float y);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetParticleRotationMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticlePivotX(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticlePivotY(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleLifespanVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleInitialVelocityVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleRotationVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleAngularVelocityVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleSizeVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetParticleOpacityVariance(IntPtr nativeEffect, uint emitterId, float variance);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleLifespanVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleInitialVelocityVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleRotationVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleAngularVelocityVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleSizeVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetParticleOpacityVariance(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetShapePath(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetWidth(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetHeight(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetOrientation(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetDirection(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSpread(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetNumParticles(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleLifespan(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetMotionPath(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleMotionPath(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleInitialVelocity(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleAcceleration(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleRadialAcceleration(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleDamping(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleInitialRotation(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleRotation(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleWeight(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleBounce(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleFriction(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleInitialSize(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleSize(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleWidth(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleHeight(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleColor(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleInitialOpacity(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetParticleOpacity(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSpawnParticles(IntPtr nativeEffect, uint emitterId, uint count);

	[DllImport(pluginName)]
	public static extern uint PixelpartFindSprite(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindSpriteByIndex(IntPtr nativeEffect, uint spriteIndex);
	[DllImport(pluginName)]
	public static extern int PixelpartSpriteGetName(IntPtr nativeEffect, uint spriteId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetLifetimeStart(IntPtr nativeEffect, uint spriteId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetLifetimeDuration(IntPtr nativeEffect, uint spriteId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetRepeat(IntPtr nativeEffect, uint spriteId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartSpriteGetLifetimeStart(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern float PixelpartSpriteGetLifetimeDuration(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern bool PixelpartSpriteGetRepeat(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern bool PixelpartSpriteIsActive(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern float PixelpartSpriteGetLocalTime(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetAlignWithPath(IntPtr nativeEffect, uint spriteId, bool mode);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetPivot(IntPtr nativeEffect, uint spriteId, float x, float y);
	[DllImport(pluginName)]
	public static extern bool PixelpartSpriteGetAlignWithPath(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern float PixelpartSpriteGetPivotX(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern float PixelpartSpriteGetPivotY(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetLayer(IntPtr nativeEffect, uint spriteId, uint layer);
	[DllImport(pluginName)]
	public static extern void PixelpartSpriteSetVisible(IntPtr nativeEffect, uint spriteId, bool visible);
	[DllImport(pluginName)]
	public static extern uint PixelpartSpriteGetLayer(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern bool PixelpartSpriteIsVisible(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern int PixelpartSpriteGetBlendMode(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetWidth(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetHeight(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetOrientation(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetMotionPath(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetColor(IntPtr nativeEffect, uint spriteId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartSpriteGetOpacity(IntPtr nativeEffect, uint spriteId);

	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceField(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceFieldByIndex(IntPtr nativeEffect, uint forceFieldIndex);
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
	public static extern void PixelpartForceFieldSetType(IntPtr nativeEffect, uint forceFieldId, int type);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetType(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetDirectionVariance(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetStrengthVariance(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetDirectionVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetStrengthVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetGridSize(IntPtr nativeEffect, uint forceFieldId, int width, int height);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetGridWidth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetGridHeight(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetWidth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetHeight(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetOrientation(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetMotionPath(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetDirection(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetStrength(IntPtr nativeEffect, uint forceFieldId);

	[DllImport(pluginName)]
	public static extern uint PixelpartFindCollider(IntPtr nativeEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindColliderByIndex(IntPtr nativeEffect, uint colliderIndex);
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
	public static extern void PixelpartColliderAddPoint(IntPtr nativeEffect, uint colliderId, float x, float y);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetPoint(IntPtr nativeEffect, uint colliderId, int index, float x, float y);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderRemovePoint(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointX(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointY(IntPtr nativeEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern uint PixelpartColliderGetNumPoints(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetBounce(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetFriction(IntPtr nativeEffect, uint colliderId);

	[DllImport(pluginName)]
	public static extern float PixelpartCurveGet(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurveGetPoint(IntPtr curve, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveSet(IntPtr curve, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveAddPoint(IntPtr curve, float position, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveSetPoint(IntPtr curve, uint index, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveMovePoint(IntPtr curve, uint index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveShiftPoint(IntPtr curve, uint index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveRemovePoint(IntPtr curve, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveClear(IntPtr curve);
	[DllImport(pluginName)]
	public static extern uint PixelpartCurveGetNumPoints(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveMove(IntPtr curve, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveShift(IntPtr curve, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveSetInterpolation(IntPtr curve, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartCurveGetInterpolation(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveEnableAdaptiveCache(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveEnableFixedCache(IntPtr curve, uint size);
	[DllImport(pluginName)]
	public static extern uint PixelpartCurveGetCacheSize(IntPtr curve);

	[DllImport(pluginName)]
	public static extern float PixelpartPathGetX(IntPtr path, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartPathGetY(IntPtr path, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartPathGetPointX(IntPtr path, uint index);
	[DllImport(pluginName)]
	public static extern float PixelpartPathGetPointY(IntPtr path, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartPathSet(IntPtr path, float x, float y);
	[DllImport(pluginName)]
	public static extern void PixelpartPathAddPoint(IntPtr path, float position, float x, float y);
	[DllImport(pluginName)]
	public static extern void PixelpartPathSetPoint(IntPtr path, uint index, float x, float y);
	[DllImport(pluginName)]
	public static extern void PixelpartPathMovePoint(IntPtr path, uint index, float deltaX, float deltaY);
	[DllImport(pluginName)]
	public static extern void PixelpartPathShiftPoint(IntPtr path, uint index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartPathRemovePoint(IntPtr path, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartPathClear(IntPtr path);
	[DllImport(pluginName)]
	public static extern uint PixelpartPathGetNumPoints(IntPtr path);
	[DllImport(pluginName)]
	public static extern void PixelpartPathMove(IntPtr path, float deltaX, float deltaY);
	[DllImport(pluginName)]
	public static extern void PixelpartPathShift(IntPtr path, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartPathSetInterpolation(IntPtr path, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartPathGetInterpolation(IntPtr path);
	[DllImport(pluginName)]
	public static extern void PixelpartPathEnableAdaptiveCache(IntPtr path);
	[DllImport(pluginName)]
	public static extern void PixelpartPathEnableFixedCache(IntPtr path, uint size);
	[DllImport(pluginName)]
	public static extern uint PixelpartPathGetCacheSize(IntPtr path);

	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetR(IntPtr gradient, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetG(IntPtr gradient, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetB(IntPtr gradient, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetA(IntPtr gradient, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetPointR(IntPtr gradient, uint index);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetPointG(IntPtr gradient, uint index);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetPointB(IntPtr gradient, uint index);
	[DllImport(pluginName)]
	public static extern float PixelpartGradientGetPointA(IntPtr gradient, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientSet(IntPtr gradient, float r, float g, float b, float a);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientAddPoint(IntPtr gradient, float position, float r, float g, float b, float a);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientSetPoint(IntPtr gradient, uint index, float r, float g, float b, float a);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientMovePoint(IntPtr gradient, uint index, float deltaR, float deltaG, float deltaB, float deltaA);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientShiftPoint(IntPtr gradient, uint index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientRemovePoint(IntPtr gradient, uint index);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientClear(IntPtr gradient);
	[DllImport(pluginName)]
	public static extern uint PixelpartGradientGetNumPoints(IntPtr gradient);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientMove(IntPtr gradient, float deltaR, float deltaG, float deltaB, float deltaA);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientShift(IntPtr gradient, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientSetInterpolation(IntPtr gradient, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartGradientGetInterpolation(IntPtr gradient);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientEnableAdaptiveCache(IntPtr gradient);
	[DllImport(pluginName)]
	public static extern void PixelpartGradientEnableFixedCache(IntPtr gradient, uint size);
	[DllImport(pluginName)]
	public static extern uint PixelpartGradientGetCacheSize(IntPtr gradient);
}
}