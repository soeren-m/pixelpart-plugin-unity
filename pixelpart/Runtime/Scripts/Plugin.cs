using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart
{
    internal static class Plugin
    {
#if !UNITY_EDITOR && (UNITY_IPHONE || UNITY_WEBGL || UNITY_SWITCH)
	private const string pluginName = "__Internal";
#else
        private const string pluginName = "pixelpart";
#endif

        // General
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartLoadEffect(byte[] data, int size,
            byte[] errorBuffer, int errorBufferSize, out int errorLength);
        [DllImport(pluginName)]
        public static extern void PixelpartDeleteEffect(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectTransform(IntPtr effectRuntime, Matrix4x4 transformMatrix, Vector3 scale);
        [DllImport(pluginName)]
        public static extern void PixelpartAdvanceEffect(IntPtr effectRuntime, float dt, bool loop, float loopTime, float speed, float timeStep);
        [DllImport(pluginName)]
        public static extern void PixelpartRestartEffect(IntPtr effectRuntime, bool reset);
        [DllImport(pluginName)]
        public static extern bool PixelpartIsEffect3d(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern float PixelpartGetEffectTime(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetEffectNodeCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetEffectParticleTypeCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern uint PixelpartGetEffectParticleRuntimeInstanceCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern void PixelpartGetEffectParticleRuntimeInstances(IntPtr effectRuntime,
            [In, Out] Pixelpart.PixelpartParticleRuntimeId[] runtimeIds);
        [DllImport(pluginName)]
        public static extern uint PixelpartGetEffectParticleCount(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartSpawnParticles(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId, int count);

        // EffectInput
        [DllImport(pluginName)]
        public static extern int PixelpartGetEffectInputCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetEffectInputs(IntPtr effectRuntime, uint[] ids, int[] types, byte[] names, int namesBufferSize);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputBool(IntPtr effectRuntime, uint inputId, bool value);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputInt(IntPtr effectRuntime, uint inputId, int value);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputFloat(IntPtr effectRuntime, uint inputId, float value);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputFloat2(IntPtr effectRuntime, uint inputId, Vector2 value);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputFloat3(IntPtr effectRuntime, uint inputId, Vector3 value);
        [DllImport(pluginName)]
        public static extern void PixelpartSetEffectInputFloat4(IntPtr effectRuntime, uint inputId, Vector4 value);
        [DllImport(pluginName)]
        public static extern bool PixelpartGetEffectInputBool(IntPtr effectRuntime, uint inputId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetEffectInputInt(IntPtr effectRuntime, uint inputId);
        [DllImport(pluginName)]
        public static extern float PixelpartGetEffectInputFloat(IntPtr effectRuntime, uint inputId);
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartGetEffectInputFloat2(IntPtr effectRuntime, uint inputId);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartGetEffectInputFloat3(IntPtr effectRuntime, uint inputId);
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartGetEffectInputFloat4(IntPtr effectRuntime, uint inputId);

        // Trigger
        [DllImport(pluginName)]
        public static extern int PixelpartGetTriggerCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetTriggers(IntPtr effectRuntime, uint[] ids, byte[] names, int namesBufferSize);
        [DllImport(pluginName)]
        public static extern void PixelpartActivateTrigger(IntPtr effectRuntime, uint triggerId);
        [DllImport(pluginName)]
        public static extern bool PixelpartIsTriggerActivated(IntPtr effectRuntime, uint triggerId);

        // Rendering
        [DllImport(pluginName)]
        public static extern void PixelpartGetSortedParticleRuntimeInstances(IntPtr effectRuntime, int[] indices);
        [DllImport(pluginName)]
        public static extern void PixelpartPrepareParticleSpriteVertexData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId, out int triangleCount, out int vertexCount);
        [DllImport(pluginName)]
        public static extern void PixelpartPrepareParticleTrailVertexData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId, out int triangleCount, out int vertexCount);
        [DllImport(pluginName)]
        public static extern void PixelpartPrepareParticleMeshVertexData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId, out int triangleCount, out int vertexCount);
        [DllImport(pluginName)]
        public static extern bool PixelpartGetParticleSpriteVertexData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId,
            Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 effectSize,
            int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3);
        [DllImport(pluginName)]
        public static extern bool PixelpartGetParticleTrailVertexData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId,
            Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 effectSize,
            int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Color[] colors, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv, [In, Out] Vector4[] uv2, [In, Out] Vector4[] uv3);
        [DllImport(pluginName)]
        public static extern bool PixelpartGetParticleMeshInstanceData(IntPtr effectRuntime, uint particleEmitterId, uint particleTypeId,
            Vector3 cameraPosition, Vector3 viewRight, Vector3 viewUp, Vector3 effectSize,
            [In, Out] Matrix4x4[] transforms, [In, Out] Vector4[] colors, [In, Out] Vector4[] velocities, [In, Out] float[] lives, [In, Out] float[] ids);

        // Resources
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceId(IntPtr effectRuntime, int index, byte[] resourceIdBuffer, int bufferLength);
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceWidth(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceHeight(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceChannels(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetImageResourceColorSpace(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern uint PixelpartGetImageResourceDataSize(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern void PixelpartGetImageResourceData(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId, byte[] imageData);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMeshResourceCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMeshResourceId(IntPtr effectRuntime, int index, byte[] resourceIdBuffer, int bufferLength);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMeshResourceIndexCount(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMeshResourceVertexCount(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern void PixelpartGetMeshResourceVertexData(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId, int[] triangles, [In, Out] Vector3[] vertices, [In, Out] Vector3[] normals, [In, Out] Vector2[] uv);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMaterialResourceCount(IntPtr effectRuntime);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMaterialResourceId(IntPtr effectRuntime, int index, byte[] resourceIdBuffer, int bufferLength);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMaterialResourceBlendMode(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern int PixelpartGetMaterialResourceLightingMode(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId);
        [DllImport(pluginName)]
        public static extern bool PixelpartBuildMaterialShader(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string resourceId, int renderPipeline,
            byte[] bufferMainCode, byte[] bufferParameterCode, byte[] bufferParameterNames, uint[] bufferParameterIds, byte[] bufferTextureResourceIds, byte[] bufferSamplerNames,
            out int lengthMainCode, out int lengthParameterCode, out int lengthParameterNames, out int lengthParameterIds, out int lengthTextureResourceIds, out int lengthSamplerNames,
            int bufferSizeMainCode, int bufferSizeParameterCode, int bufferSizeParameterNames, int bufferSizeParameterIds, int bufferSizeTextureResourceIds, int bufferSizeSamplerNames);

        // Node
        [DllImport(pluginName)]
        public static extern uint PixelpartFindNode(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string buffer);
        [DllImport(pluginName)]
        public static extern uint PixelpartFindNodeByIndex(IntPtr effectRuntime, int nodeIndex);
        [DllImport(pluginName)]
        public static extern bool PixelpartNodeExists(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern int PixelpartNodeGetType(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern int PixelpartNodeGetName(IntPtr effectRuntime, uint nodeId, byte[] buffer, int length);
        [DllImport(pluginName)]
        public static extern uint PixelpartNodeGetParentId(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern void PixelpartNodeSetStart(IntPtr effectRuntime, uint nodeId, float value);
        [DllImport(pluginName)]
        public static extern void PixelpartNodeSetDuration(IntPtr effectRuntime, uint nodeId, float value);
        [DllImport(pluginName)]
        public static extern void PixelpartNodeSetRepeat(IntPtr effectRuntime, uint nodeId, bool value);
        [DllImport(pluginName)]
        public static extern float PixelpartNodeGetStart(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern float PixelpartNodeGetDuration(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern bool PixelpartNodeIsRepeating(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern bool PixelpartNodeIsActive(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern float PixelpartNodeGetLocalTime(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNodeGetPosition(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNodeGetRotation(IntPtr effectRuntime, uint nodeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNodeGetScale(IntPtr effectRuntime, uint nodeId);

        // ParticleEmitter
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetShape(IntPtr effectRuntime, uint emitterId, int shape);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetShape(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterAddShapePoint(IntPtr effectRuntime, uint emitterId, Vector3 point);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterRemoveShapePoint(IntPtr effectRuntime, uint emitterId, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetShapePoint(IntPtr effectRuntime, uint emitterId, int index, Vector3 point);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetShapePointCount(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartParticleEmitterGetShapePoint(IntPtr effectRuntime, uint emitterId, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetDistribution(IntPtr effectRuntime, uint emitterId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetDistribution(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetGridOrder(IntPtr effectRuntime, uint emitterId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetGridOrder(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetGridSize(IntPtr effectRuntime, uint emitterId, int x, int y, int z);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetGridSizeX(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetGridSizeY(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetGridSizeZ(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetEmissionMode(IntPtr effectRuntime, uint emitterId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetEmissionMode(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleEmitterSetDirectionMode(IntPtr effectRuntime, uint emitterId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleEmitterGetDirectionMode(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleEmitterGetDirection(IntPtr effectRuntime, uint emitterId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleEmitterGetSpread(IntPtr effectRuntime, uint emitterId);

        // ForceField
        [DllImport(pluginName)]
        public static extern void PixelpartForceFieldSetInfinite(IntPtr effectRuntime, uint forceFieldId, bool mode);
        [DllImport(pluginName)]
        public static extern bool PixelpartForceFieldIsInfinite(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartForceFieldGetStrength(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartAccelerationFieldGetAccelerationDirection(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartAccelerationFieldGetAccelerationDirectionVariance(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartAccelerationFieldGetAccelerationStrengthVariance(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern void PixelpartAccelerationFieldSetAccelerationGridSize(IntPtr effectRuntime, uint forceFieldId, int x, int y, int z);
        [DllImport(pluginName)]
        public static extern int PixelpartAccelerationFieldGetAccelerationGridSizeX(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern int PixelpartAccelerationFieldGetAccelerationGridSizeY(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern int PixelpartAccelerationFieldGetAccelerationGridSizeZ(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern void PixelpartVectorFieldSetVectorFieldFilter(IntPtr effectRuntime, uint forceFieldId, int filter);
        [DllImport(pluginName)]
        public static extern int PixelpartVectorFieldGetVectorFieldFilter(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartVectorFieldGetTightness(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoiseOctaves(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoiseFrequency(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoisePersistence(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoiseLacunarity(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern void PixelpartNoiseFieldSetNoiseAnimated(IntPtr effectRuntime, uint forceFieldId, bool animated);
        [DllImport(pluginName)]
        public static extern bool PixelpartNoiseFieldIsNoiseAnimated(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoiseAnimationTimeScale(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartNoiseFieldGetNoiseAnimationTimeBase(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartDragFieldGetVelocityInfluence(IntPtr effectRuntime, uint forceFieldId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartDragFieldGetSizeInfluence(IntPtr effectRuntime, uint forceFieldId);

        // Collider
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartColliderGetKillOnContact(IntPtr effectRuntime, uint colliderId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartColliderGetBounce(IntPtr effectRuntime, uint colliderId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartColliderGetFriction(IntPtr effectRuntime, uint colliderId);
        [DllImport(pluginName)]
        public static extern void PixelpartLineColliderAddPoint(IntPtr effectRuntime, uint colliderId, Vector3 point);
        [DllImport(pluginName)]
        public static extern void PixelpartLineColliderSetPoint(IntPtr effectRuntime, uint colliderId, int index, Vector3 point);
        [DllImport(pluginName)]
        public static extern void PixelpartLineColliderRemovePoint(IntPtr effectRuntime, uint colliderId, int index);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartLineColliderGetPoint(IntPtr effectRuntime, uint colliderId, int index);
        [DllImport(pluginName)]
        public static extern int PixelpartLineColliderGetPointCount(IntPtr effectRuntime, uint colliderId);

        // LightSource
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartLightSourceGetAttenuation(IntPtr effectRuntime, uint lightSourceId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartLightSourceGetColor(IntPtr effectRuntime, uint lightSourceId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartLightSourceGetIntensity(IntPtr effectRuntime, uint lightSourceId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartSpotLightSourceGetSpotAngle(IntPtr effectRuntime, uint lightSourceId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartSpotLightSourceGetSpotAngleAttenuation(IntPtr effectRuntime, uint lightSourceId);

        // ParticleType
        [DllImport(pluginName)]
        public static extern uint PixelpartFindParticleType(IntPtr effectRuntime, [MarshalAs(UnmanagedType.LPStr)] string buffer);
        [DllImport(pluginName)]
        public static extern uint PixelpartFindParticleTypeByIndex(IntPtr effectRuntime, int particleTypeIndex);
        [DllImport(pluginName)]
        public static extern bool PixelpartParticleTypeExists(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetIndex(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern uint PixelpartParticleTypeGetParentId(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetName(IntPtr effectRuntime, uint particleTypeId, byte[] buffer, int length);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeSetPositionRelative(IntPtr effectRuntime, uint particleTypeId, bool relative);
        [DllImport(pluginName)]
        public static extern bool PixelpartParticleTypeIsPositionRelative(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeSetRotationMode(IntPtr effectRuntime, uint particleTypeId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetRotationMode(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeSetAlignmentMode(IntPtr effectRuntime, uint particleTypeId, int mode);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetAlignmentMode(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetPivot(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetMotionPathForce(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeSetVisible(IntPtr effectRuntime, uint particleTypeId, bool visible);
        [DllImport(pluginName)]
        public static extern bool PixelpartParticleTypeIsVisible(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeSetLayer(IntPtr effectRuntime, uint particleTypeId, int layer);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetLayer(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMaterialId(IntPtr effectRuntime, uint particleTypeId, byte[] buffer, int length);
        [DllImport(pluginName)]
        public static extern bool PixelpartParticleTypeIsMaterialBuiltIn(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMaterialParameterCount(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern void PixelpartParticleTypeGetMaterialParameterIds(IntPtr effectRuntime, uint particleTypeId, uint[] parameterIds);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMaterialParameterType(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern bool PixelpartParticleTypeGetMaterialParameterValueBool(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMaterialParameterValueInt(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern float PixelpartParticleTypeGetMaterialParameterValueFloat(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartParticleTypeGetMaterialParameterValueFloat2(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartParticleTypeGetMaterialParameterValueFloat3(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartParticleTypeGetMaterialParameterValueFloat4(IntPtr effectRuntime, uint particleTypeId, uint parameterId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMaterialParameterValueResourceId(IntPtr effectRuntime, uint particleTypeId, uint parameterId, byte[] buffer, int length);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetRenderer(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern int PixelpartParticleTypeGetMeshRendererMeshResourceId(IntPtr effectRuntime, uint particleTypeId, byte[] buffer, int length);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetCount(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetLifespan(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetInitialSize(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetInitialRotation(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetInitialVelocity(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetInheritedVelocity(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetInitialOpacity(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetMotionPath(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetSize(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetStretch(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetPhysicalSize(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetRotation(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetRotationBySpeed(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetAcceleration(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetRadialAcceleration(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetWeight(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetBounce(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetFriction(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetColor(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetOpacity(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetLifespanVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetSizeVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetRotationVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetAngularVelocityVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetVelocityVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetColorVariance(IntPtr effectRuntime, uint particleTypeId);
        [DllImport(pluginName)]
        public static extern IntPtr PixelpartParticleTypeGetOpacityVariance(IntPtr effectRuntime, uint particleTypeId);

        // StaticPropertyBool
        [DllImport(pluginName)]
        public static extern bool PixelpartStaticPropertyBoolValue(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyBoolSetBaseValue(IntPtr property, bool value);
        [DllImport(pluginName)]
        public static extern bool PixelpartStaticPropertyBoolGetBaseValue(IntPtr property);

        // StaticPropertyInt
        [DllImport(pluginName)]
        public static extern int PixelpartStaticPropertyIntValue(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyIntSetBaseValue(IntPtr property, int value);
        [DllImport(pluginName)]
        public static extern int PixelpartStaticPropertyIntGetBaseValue(IntPtr property);

        // StaticPropertyFloat
        [DllImport(pluginName)]
        public static extern float PixelpartStaticPropertyFloatValue(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyFloatSetBaseValue(IntPtr property, float value);
        [DllImport(pluginName)]
        public static extern float PixelpartStaticPropertyFloatGetBaseValue(IntPtr property);

        // StaticPropertyFloat2
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartStaticPropertyFloat2Value(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyFloat2SetBaseValue(IntPtr property, Vector2 value);
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartStaticPropertyFloat2GetBaseValue(IntPtr property);

        // StaticPropertyFloat3
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartStaticPropertyFloat3Value(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyFloat3SetBaseValue(IntPtr property, Vector3 value);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartStaticPropertyFloat3GetBaseValue(IntPtr property);

        // StaticPropertyFloat4
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartStaticPropertyFloat4Value(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartStaticPropertyFloat4SetBaseValue(IntPtr property, Vector4 value);
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartStaticPropertyFloat4GetBaseValue(IntPtr property);

        // AnimatedPropertyFloat
        [DllImport(pluginName)]
        public static extern float PixelpartAnimatedPropertyFloatAt(IntPtr property, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatAddKeyframe(IntPtr property, float position, float value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatRemoveKeyframe(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatSetKeyframeValue(IntPtr property, int index, float value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatSetKeyframePosition(IntPtr property, int index, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatClearKeyframes(IntPtr property);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloatKeyframeCount(IntPtr property);
        [DllImport(pluginName)]
        public static extern float PixelpartAnimatedPropertyFloatKeyframeValue(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloatKeyframeIndex(IntPtr property, float position, float epsilon);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatSetKeyframeInterpolation(IntPtr property, int method);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloatGetKeyframeInterpolation(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatEnableAdaptiveCache(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloatEnableFixedCache(IntPtr property, int size);

        // AnimatedPropertyFloat2
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartAnimatedPropertyFloat2At(IntPtr property, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2AddKeyframe(IntPtr property, float position, Vector2 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2RemoveKeyframe(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2SetKeyframeValue(IntPtr property, int index, Vector2 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2SetKeyframePosition(IntPtr property, int index, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2ClearKeyframes(IntPtr property);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat2KeyframeCount(IntPtr property);
        [DllImport(pluginName)]
        public static extern Vector2 PixelpartAnimatedPropertyFloat2KeyframeValue(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat2KeyframeIndex(IntPtr property, float position, float epsilon);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2SetKeyframeInterpolation(IntPtr property, int method);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat2GetKeyframeInterpolation(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2EnableAdaptiveCache(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat2EnableFixedCache(IntPtr property, int size);

        // AnimatedPropertyFloat3
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartAnimatedPropertyFloat3At(IntPtr property, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3AddKeyframe(IntPtr property, float position, Vector3 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3RemoveKeyframe(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3SetKeyframeValue(IntPtr property, int index, Vector3 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3SetKeyframePosition(IntPtr property, int index, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3ClearKeyframes(IntPtr property);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat3KeyframeCount(IntPtr property);
        [DllImport(pluginName)]
        public static extern Vector3 PixelpartAnimatedPropertyFloat3KeyframeValue(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat3KeyframeIndex(IntPtr property, float position, float epsilon);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3SetKeyframeInterpolation(IntPtr property, int method);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat3GetKeyframeInterpolation(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3EnableAdaptiveCache(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat3EnableFixedCache(IntPtr property, int size);

        // AnimatedPropertyFloat4
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartAnimatedPropertyFloat4At(IntPtr property, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4AddKeyframe(IntPtr property, float position, Vector4 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4RemoveKeyframe(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4SetKeyframeValue(IntPtr property, int index, Vector4 value);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4SetKeyframePosition(IntPtr property, int index, float position);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4ClearKeyframes(IntPtr property);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat4KeyframeCount(IntPtr property);
        [DllImport(pluginName)]
        public static extern Vector4 PixelpartAnimatedPropertyFloat4KeyframeValue(IntPtr property, int index);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat4KeyframeIndex(IntPtr property, float position, float epsilon);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4SetKeyframeInterpolation(IntPtr property, int method);
        [DllImport(pluginName)]
        public static extern int PixelpartAnimatedPropertyFloat4GetKeyframeInterpolation(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4EnableAdaptiveCache(IntPtr property);
        [DllImport(pluginName)]
        public static extern void PixelpartAnimatedPropertyFloat4EnableFixedCache(IntPtr property, int size);
    }
}