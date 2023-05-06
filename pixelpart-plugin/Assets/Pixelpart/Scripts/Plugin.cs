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
	public static extern void PixelpartUpdateParticleSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateForceSolver(IntPtr nativeEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateCollisionSolver(IntPtr nativeEffect);
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
	public static extern void PixelpartGetParticleTypesSortedByLayer(IntPtr nativeEffect, uint[] indices);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildParticleShader(IntPtr nativeEffect, uint particleTypeIndex, byte[] bufferCode, byte[] bufferTextureIds, out int outLengthCode, out int outLengthTextureIds, int bufferSizeCode, int bufferSizeTexturesIds);
	[DllImport(pluginName)]
	public static extern bool PixelpartPrepareParticleMesh(IntPtr nativeEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildParticleMesh(IntPtr nativeEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, float scaleX, float scaleY, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3, [In, Out] Vector4[] uv4);
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
	public static extern void PixelpartParticleEmitterSetShape(IntPtr nativeEffect, uint emitterId, int type);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetShape(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetPath(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSize(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetOrientation(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDistribution(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetEmissionMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDirectionMode(IntPtr nativeEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDistribution(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetEmissionMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDirectionMode(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetDirection(IntPtr nativeEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSpread(IntPtr nativeEffect, uint emitterId);

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
	public static extern void PixelpartParticleTypeSetLifespanVariance(IntPtr nativeEffect, uint particleTypeId, float variance);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetLifespanVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetPositionRelative(IntPtr nativeEffect, uint particleTypeId, bool relative);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeGetPositionRelative(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetMotionPathForce(IntPtr nativeEffect, uint particleTypeId, float force);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetMotionPathForce(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialVelocity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetVelocityVariance(IntPtr nativeEffect, uint particleTypeId, float variance);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetVelocityVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetAcceleration(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRadialAcceleration(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetDamping(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetRotationMode(IntPtr nativeEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetAlignmentMode(IntPtr nativeEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRotationMode(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetAlignmentMode(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialRotation(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotation(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetRotationVariance(IntPtr nativeEffect, uint particleTypeId, float varianceRoll, float varianceYaw, float variancePitch);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetRotationVarianceRoll(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetRotationVarianceYaw(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetRotationVariancePitch(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetAngularVelocityVariance(IntPtr nativeEffect, uint particleTypeId, float varianceRoll, float varianceYaw, float variancePitch);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetAngularVelocityVarianceRoll(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetAngularVelocityVarianceYaw(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetAngularVelocityVariancePitch(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetPivot(IntPtr nativeEffect, uint particleTypeId, float x, float y, float z);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetPivotX(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetPivotY(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetPivotZ(IntPtr nativeEffect, uint particleTypeId);
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
	public static extern void PixelpartParticleTypeSetBlendMode(IntPtr nativeEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetBlendMode(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialSize(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetSize(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetSizeVariance(IntPtr nativeEffect, uint particleTypeId, float variance);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetSizeVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetColor(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetColorVariance(IntPtr nativeEffect, uint particleTypeId, float h, float s, float v);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetColorVarianceHue(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetColorVarianceSaturation(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetColorVarianceValue(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialOpacity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetOpacity(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetOpacityVariance(IntPtr nativeEffect, uint particleTypeId, float variance);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetOpacityVariance(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRenderer(IntPtr nativeEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSpawnParticles(IntPtr nativeEffect, uint particleTypeId, int count);

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
	public static extern IntPtr PixelpartForceFieldGetDirection(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetStrength(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetDirectionVariance(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetStrengthVariance(IntPtr nativeEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetDirectionVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetStrengthVariance(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetGridSize(IntPtr nativeEffect, uint forceFieldId, int width, int height, int depth);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetGridWidth(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetGridHeight(IntPtr nativeEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetGridDepth(IntPtr nativeEffect, uint forceFieldId);

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
	public static extern void PixelpartColliderAddPoint(IntPtr nativeEffect, uint colliderId, float x, float y, float z);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetPoint(IntPtr nativeEffect, uint colliderId, int index, float x, float y, float z);
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
	public static extern IntPtr PixelpartColliderGetBounce(IntPtr nativeEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetFriction(IntPtr nativeEffect, uint colliderId);

	[DllImport(pluginName)]
	public static extern float PixelpartCurveGet(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurveGetPoint(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveSet(IntPtr curve, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveAddPoint(IntPtr curve, float position, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveSetPoint(IntPtr curve, int index, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveMovePoint(IntPtr curve, int index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveShiftPoint(IntPtr curve, int index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveRemovePoint(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurveClear(IntPtr curve);
	[DllImport(pluginName)]
	public static extern int PixelpartCurveGetNumPoints(IntPtr curve);
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
	public static extern void PixelpartCurveEnableFixedCache(IntPtr curve, int size);
	[DllImport(pluginName)]
	public static extern int PixelpartCurveGetCacheSize(IntPtr curve);

	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetX(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetY(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetZ(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetPointX(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetPointY(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve3GetPointZ(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3Set(IntPtr curve, float x, float y, float z);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3AddPoint(IntPtr curve, float position, float x, float y, float z);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3SetPoint(IntPtr curve, int index, float x, float y, float z);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3MovePoint(IntPtr curve, int index, float deltaX, float deltaY, float deltaZ);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3ShiftPoint(IntPtr curve, int index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3RemovePoint(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3Clear(IntPtr curve);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve3GetNumPoints(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3Move(IntPtr curve, float deltaX, float deltaY, float deltaZ);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3Shift(IntPtr curve, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3SetInterpolation(IntPtr curve, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve3GetInterpolation(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3EnableAdaptiveCache(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve3EnableFixedCache(IntPtr curve, int size);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve3GetCacheSize(IntPtr curve);

	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetX(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetY(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetZ(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetW(IntPtr curve, float position);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetPointX(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetPointY(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetPointZ(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartCurve4GetPointW(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4Set(IntPtr curve, float x, float y, float z, float w);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4AddPoint(IntPtr curve, float position, float x, float y, float z, float w);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4SetPoint(IntPtr curve, int index, float x, float y, float z, float w);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4MovePoint(IntPtr curve, int index, float deltaX, float deltaY, float deltaZ, float deltaW);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4ShiftPoint(IntPtr curve, int index, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4RemovePoint(IntPtr curve, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4Clear(IntPtr curve);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve4GetNumPoints(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4Move(IntPtr curve, float deltaX, float deltaY, float deltaZ, float deltaW);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4Shift(IntPtr curve, float delta);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4SetInterpolation(IntPtr curve, int method);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve4GetInterpolation(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4EnableAdaptiveCache(IntPtr curve);
	[DllImport(pluginName)]
	public static extern void PixelpartCurve4EnableFixedCache(IntPtr curve, int size);
	[DllImport(pluginName)]
	public static extern int PixelpartCurve4GetCacheSize(IntPtr curve);
}
}