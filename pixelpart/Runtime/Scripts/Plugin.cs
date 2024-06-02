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

	// General
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartLoadEffect(byte[] data, int size);
	[DllImport(pluginName)]
	public static extern void PixelpartDeleteEffect(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartUpdateEffect(IntPtr internalEffect, float dt);
	[DllImport(pluginName)]
	public static extern void PixelpartPlayEffect(IntPtr internalEffect, bool state);
	[DllImport(pluginName)]
	public static extern void PixelpartRestartEffect(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartResetEffect(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern bool PixelpartIsEffect3D(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectLoop(IntPtr internalEffect, bool loop);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectLoopTime(IntPtr internalEffect, float loopTime);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectSpeed(IntPtr internalEffect, float speed);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectTimeStep(IntPtr internalEffect, float timeStep);
	[DllImport(pluginName)]
	public static extern bool PixelpartIsEffectPlaying(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetEffectLoop(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectLoopTime(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectSpeed(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectTimeStep(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectTime(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticleEmitters(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticleTypes(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumForceFields(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumColliders(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetEffectNumParticles(IntPtr internalEffect, uint particleTypeIndex);
	[DllImport(pluginName)]
	public static extern int PixelpartGetNumEffectInputs(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetEffectInputs(IntPtr internalEffect, uint[] ids, byte[] names, int bufferSizeNames);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputBool(IntPtr internalEffect, uint inputId, bool value);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputInt(IntPtr internalEffect, uint inputId, int value);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputFloat(IntPtr internalEffect, uint inputId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputFloat2(IntPtr internalEffect, uint inputId, Vector2 value);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputFloat3(IntPtr internalEffect, uint inputId, Vector3 value);
	[DllImport(pluginName)]
	public static extern void PixelpartSetEffectInputFloat4(IntPtr internalEffect, uint inputId, Vector4 value);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetEffectInputBool(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern int PixelpartGetEffectInputInt(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern float PixelpartGetEffectInputFloat(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern Vector2 PixelpartGetEffectInputFloat2(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern Vector3 PixelpartGetEffectInputFloat3(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern Vector4 PixelpartGetEffectInputFloat4(IntPtr internalEffect, uint inputId);
	[DllImport(pluginName)]
	public static extern void PixelpartSpawnParticles(IntPtr internalEffect, uint particleTypeId, int count);

	// Rendering
	[DllImport(pluginName)]
	public static extern void PixelpartGetParticleTypesSortedForRendering(IntPtr internalEffect, uint[] indices);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleSpriteVertexData(IntPtr internalEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleTrailVertexData(IntPtr internalEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern void PixelpartPrepareParticleMeshVertexData(IntPtr internalEffect, uint particleTypeIndex, out int numTriangles, out int numVertices);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleSpriteVertexData(IntPtr internalEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
		int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleTrailVertexData(IntPtr internalEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
		int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3);
	[DllImport(pluginName)]
	public static extern bool PixelpartGetParticleMeshInstanceData(IntPtr internalEffect, uint particleTypeIndex, Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 scale,
		[In, Out] Matrix4x4[] transforms, [In, Out] Vector4[] colors, [In, Out] Vector4[] velocities, [In, Out] float[] lives, [In, Out] float[] ids);

	// Resources
	[DllImport(pluginName)]
	public static extern uint PixelpartGetImageResourceCount(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceId(IntPtr internalEffect, uint index, byte[] resourceIdBuffer, int bufferLength);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceWidth(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern int PixelpartGetImageResourceHeight(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetImageResourceDataSize(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern void PixelpartGetImageResourceData(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId, byte[] imageData);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetMeshResourceCount(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMeshResourceId(IntPtr internalEffect, uint index, byte[] resourceIdBuffer, int bufferLength);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMeshResourceIndexCount(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMeshResourceVertexCount(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern void PixelpartGetMeshResourceVertexData(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv);
	[DllImport(pluginName)]
	public static extern uint PixelpartGetMaterialResourceCount(IntPtr internalEffect);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMaterialResourceId(IntPtr internalEffect, uint index, byte[] resourceIdBuffer, int bufferLength);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMaterialResourceBlendMode(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern int PixelpartGetMaterialResourceLightingMode(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
	[DllImport(pluginName)]
	public static extern bool PixelpartBuildMaterialShader(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string resourceId, int renderPipeline,
		byte[] bufferMainCode, byte[] bufferParameterCode, byte[] bufferParameterNames, uint[] bufferParameterIds, byte[] bufferTextureResourceIds, byte[] bufferSamplerNames,
		out int lengthMainCode, out int lengthParameterCode, out int lengthParameterNames, out int lengthParameterIds, out int lengthTextureResourceIds, out int lengthSamplerNames,
		int bufferSizeMainCode, int bufferSizeParameterCode, int bufferSizeParameterNames, int bufferSizeParameterIds, int bufferSizeTextureResourceIds, int bufferSizeSamplerNames);

	// ParticleEmitter
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitter(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleEmitterByIndex(IntPtr internalEffect, uint emitterIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasParticleEmitter(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetName(IntPtr internalEffect, uint emitterId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern uint PixelpartParticleEmitterGetParentId(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetLifetimeStart(IntPtr internalEffect, uint emitterId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetLifetimeDuration(IntPtr internalEffect, uint emitterId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetRepeat(IntPtr internalEffect, uint emitterId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLifetimeStart(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLifetimeDuration(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleEmitterGetRepeat(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleEmitterIsActive(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleEmitterGetLocalTime(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetPosition(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetShape(IntPtr internalEffect, uint emitterId, int shape);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetShape(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterAddShapePoint(IntPtr internalEffect, uint emitterId, Vector3 point);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterRemoveShapePoint(IntPtr internalEffect, uint emitterId, int index);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetShapePoint(IntPtr internalEffect, uint emitterId, int index, Vector3 point);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetNumShapePoints(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern Vector3 PixelpartParticleEmitterGetShapePoint(IntPtr internalEffect, uint emitterId, int index);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSize(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetOrientation(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDistribution(IntPtr internalEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDistribution(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetGridOrder(IntPtr internalEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridOrder(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetGridSize(IntPtr internalEffect, uint emitterId, int width, int height, int depth);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridWidth(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridHeight(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetGridDepth(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetEmissionMode(IntPtr internalEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetEmissionMode(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleEmitterSetDirectionMode(IntPtr internalEffect, uint emitterId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleEmitterGetDirectionMode(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetDirection(IntPtr internalEffect, uint emitterId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleEmitterGetSpread(IntPtr internalEffect, uint emitterId);

	// ParticleType
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleType(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindParticleTypeByIndex(IntPtr internalEffect, uint particleTypeIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasParticleType(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetName(IntPtr internalEffect, uint particleTypeId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern uint PixelpartParticleTypeGetParentId(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetPosition(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetNumParticles(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetLifespan(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetLifespanVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetPositionRelative(IntPtr internalEffect, uint particleTypeId, bool relative);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeGetPositionRelative(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetMotionPathForce(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialVelocity(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInheritedVelocity(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetVelocityVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetAcceleration(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRadialAcceleration(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetRotationMode(IntPtr internalEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRotationMode(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetAlignmentMode(IntPtr internalEffect, uint particleTypeId, int mode);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetAlignmentMode(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialRotation(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotation(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotationBySpeed(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetRotationVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetAngularVelocityVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetPivot(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetPhysicalSize(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetWeight(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetBounce(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetFriction(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetVisible(IntPtr internalEffect, uint particleTypeId, bool visible);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeIsVisible(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeSetLayer(IntPtr internalEffect, uint particleTypeId, int layer);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetLayer(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialSize(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetSize(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetSizeVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetStretch(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetColor(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetColorVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetInitialOpacity(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetOpacity(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartParticleTypeGetOpacityVariance(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetMaterialId(IntPtr internalEffect, uint particleTypeId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeIsMaterialBuiltIn(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetNumMaterialParameters(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern void PixelpartParticleTypeGetMaterialParameterIds(IntPtr internalEffect, uint particleTypeId, uint[] parameterIds);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetMaterialParameterType(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern bool PixelpartParticleTypeGetMaterialParameterValueBool(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetMaterialParameterValueInt(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern float PixelpartParticleTypeGetMaterialParameterValueFloat(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern Vector2 PixelpartParticleTypeGetMaterialParameterValueFloat2(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern Vector3 PixelpartParticleTypeGetMaterialParameterValueFloat3(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern Vector4 PixelpartParticleTypeGetMaterialParameterValueFloat4(IntPtr internalEffect, uint particleTypeId, uint parameterId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetMaterialParameterValueResourceId(IntPtr internalEffect, uint particleTypeId, uint parameterId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetRenderer(IntPtr internalEffect, uint particleTypeId);
	[DllImport(pluginName)]
	public static extern int PixelpartParticleTypeGetMeshRendererMeshResourceId(IntPtr internalEffect, uint particleTypeId, byte[] buffer, int length);

	// ForceField
	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceField(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindForceFieldByIndex(IntPtr internalEffect, uint forceFieldIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasForceField(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetName(IntPtr internalEffect, uint forceFieldId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetLifetimeStart(IntPtr internalEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetLifetimeDuration(IntPtr internalEffect, uint forceFieldId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetRepeat(IntPtr internalEffect, uint forceFieldId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLifetimeStart(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLifetimeDuration(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldGetRepeat(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldIsActive(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern float PixelpartForceFieldGetLocalTime(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetPosition(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetType(IntPtr internalEffect, uint forceFieldId, int type);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetType(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetSize(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetOrientation(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetStrength(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationDirection(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationDirectionVariance(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetAccelerationStrengthVariance(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetAccelerationGridSize(IntPtr internalEffect, uint forceFieldId, int width, int height, int depth);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridWidth(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridHeight(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetAccelerationGridDepth(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetVectorFilter(IntPtr internalEffect, uint forceFieldId, int filter);
	[DllImport(pluginName)]
	public static extern int PixelpartForceFieldGetVectorFilter(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetVectorTightness(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseOctaves(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseFrequency(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoisePersistence(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseLacunarity(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern void PixelpartForceFieldSetNoiseAnimated(IntPtr internalEffect, uint forceFieldId, bool animated);
	[DllImport(pluginName)]
	public static extern bool PixelpartForceFieldGetNoiseAnimated(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseAnimationTimeScale(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetNoiseAnimationTimeBase(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetDragVelocityInfluence(IntPtr internalEffect, uint forceFieldId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartForceFieldGetDragSizeInfluence(IntPtr internalEffect, uint forceFieldId);

	// Collider
	[DllImport(pluginName)]
	public static extern uint PixelpartFindCollider(IntPtr internalEffect, [MarshalAs(UnmanagedType.LPStr)] string buffer);
	[DllImport(pluginName)]
	public static extern uint PixelpartFindColliderByIndex(IntPtr internalEffect, uint colliderIndex);
	[DllImport(pluginName)]
	public static extern bool PixelpartHasCollider(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern int PixelpartColliderGetName(IntPtr internalEffect, uint colliderId, byte[] buffer, int length);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetLifetimeStart(IntPtr internalEffect, uint colliderId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetLifetimeDuration(IntPtr internalEffect, uint colliderId, float value);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetRepeat(IntPtr internalEffect, uint colliderId, bool value);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLifetimeStart(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLifetimeDuration(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern bool PixelpartColliderGetRepeat(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern bool PixelpartColliderIsActive(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetLocalTime(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderAddPoint(IntPtr internalEffect, uint colliderId, Vector3 point);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderSetPoint(IntPtr internalEffect, uint colliderId, int index, Vector3 point);
	[DllImport(pluginName)]
	public static extern void PixelpartColliderRemovePoint(IntPtr internalEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointX(IntPtr internalEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointY(IntPtr internalEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern float PixelpartColliderGetPointZ(IntPtr internalEffect, uint colliderId, int index);
	[DllImport(pluginName)]
	public static extern int PixelpartColliderGetNumPoints(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetWidth(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetOrientation(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetKillOnContact(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetBounce(IntPtr internalEffect, uint colliderId);
	[DllImport(pluginName)]
	public static extern IntPtr PixelpartColliderGetFriction(IntPtr internalEffect, uint colliderId);

	// StaticPropertyBool
	[DllImport(pluginName)]
	public static extern bool PixelpartStaticPropertyBoolGet(IntPtr property);
	[DllImport(pluginName)]
	public static extern void PixelpartStaticPropertyBoolSetValue(IntPtr property, bool value);
	[DllImport(pluginName)]
	public static extern bool PixelpartStaticPropertyBoolGetValue(IntPtr property);

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