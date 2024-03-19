using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Pixelpart {
public class PixelpartForceField {
	public enum ForceType : int {
		AttractionField = 0,
		AccelerationField = 1,
		VectorField = 2,
		NoiseField = 3,
		DragField = 4
	}
	public enum VectorFieldFilter : int {
		None = 0,
		Linear = 1
	}

	public uint Id {
		get {
			return forceFieldId;
		}
	}
	private readonly uint forceFieldId;

	public string Name {
		get {
			byte[] buffer = new byte[256];
			int size = Plugin.PixelpartForceFieldGetName(nativeEffect, forceFieldId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeStart(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeStart(nativeEffect, forceFieldId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeDuration(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeDuration(nativeEffect, forceFieldId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartForceFieldGetRepeat(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetRepeat(nativeEffect, forceFieldId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartForceFieldIsActive(nativeEffect, forceFieldId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartForceFieldGetLocalTime(nativeEffect, forceFieldId);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Position {
		get {
			return position;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 position;

	public ForceType Type {
		get {
			return (ForceType)Plugin.PixelpartForceFieldGetType(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetType(nativeEffect, forceFieldId, (int)value);
		}
	}

	public PixelpartAnimatedPropertyFloat3 Size {
		get {
			return size;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 size;

	public PixelpartAnimatedPropertyFloat3 Orientation {
		get {
			return orientation;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 orientation;

	public PixelpartAnimatedPropertyFloat Strength {
		get {
			return strength;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat strength;

	public PixelpartAnimatedPropertyFloat3 AccelerationDirection {
		get {
			return accelerationDirection;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat3 accelerationDirection;

	public PixelpartAnimatedPropertyFloat AccelerationDirectionVariance {
		get {
			return accelerationDirectionVariance;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat accelerationDirectionVariance;

	public PixelpartAnimatedPropertyFloat AccelerationStrengthVariance {
		get {
			return accelerationStrengthVariance;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat accelerationStrengthVariance;

	public Vector3Int AccelerationGridSize {
		get {
			return new Vector3Int(
				Plugin.PixelpartForceFieldGetAccelerationGridWidth(nativeEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetAccelerationGridHeight(nativeEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetAccelerationGridDepth(nativeEffect, forceFieldId));
		}
		set {
			Plugin.PixelpartForceFieldSetAccelerationGridSize(nativeEffect, forceFieldId, value.x, value.y, value.z);
		}
	}

	public VectorFieldFilter VectorFilter {
		get {
			return (VectorFieldFilter)Plugin.PixelpartForceFieldGetVectorFilter(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetVectorFilter(nativeEffect, forceFieldId, (int)value);
		}
	}

	public PixelpartAnimatedPropertyFloat VectorTightness {
		get {
			return vectorTightness;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat vectorTightness;

	public PixelpartStaticPropertyInt NoiseOctaves {
		get {
			return noiseOctaves;
		}
	}
	private readonly PixelpartStaticPropertyInt noiseOctaves;

	public PixelpartAnimatedPropertyFloat NoiseFrequency {
		get {
			return noiseFrequency;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat noiseFrequency;

	public PixelpartAnimatedPropertyFloat NoisePersistence {
		get {
			return noisePersistence;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat noisePersistence;

	public PixelpartAnimatedPropertyFloat NoiseLacunarity {
		get {
			return noiseLacunarity;
		}
	}
	private readonly PixelpartAnimatedPropertyFloat noiseLacunarity;

	public bool NoiseAnimated {
		get {
			return Plugin.PixelpartForceFieldGetNoiseAnimated(nativeEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetNoiseAnimated(nativeEffect, forceFieldId, value);
		}
	}

	public PixelpartStaticPropertyFloat NoiseAnimationTimeScale {
		get {
			return noiseAnimationTimeScale;
		}
	}
	private readonly PixelpartStaticPropertyFloat noiseAnimationTimeScale;

	public PixelpartStaticPropertyFloat NoiseAnimationTimeBase {
		get {
			return noiseAnimationTimeBase;
		}
	}
	private readonly PixelpartStaticPropertyFloat noiseAnimationTimeBase;

	public PixelpartStaticPropertyFloat DragVelocityInfluence {
		get {
			return dragVelocityInfluence;
		}
	}
	private readonly PixelpartStaticPropertyFloat dragVelocityInfluence;

	public PixelpartStaticPropertyFloat DragSizeInfluence {
		get {
			return dragSizeInfluence;
		}
	}
	private readonly PixelpartStaticPropertyFloat dragSizeInfluence;

	private readonly IntPtr nativeEffect;

	public PixelpartForceField(IntPtr nativeEffectPtr, uint nativeForceFieldId) {
		nativeEffect = nativeEffectPtr;
		forceFieldId = nativeForceFieldId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetPosition(nativeEffect, forceFieldId), nativeEffect);
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetSize(nativeEffect, forceFieldId), nativeEffect);
		orientation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetOrientation(nativeEffect, forceFieldId), nativeEffect);
		strength = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetStrength(nativeEffect, forceFieldId), nativeEffect);
		accelerationDirection = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetAccelerationDirection(nativeEffect, forceFieldId), nativeEffect);
		accelerationDirectionVariance = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetAccelerationDirectionVariance(nativeEffect, forceFieldId), nativeEffect);
		accelerationStrengthVariance = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetAccelerationStrengthVariance(nativeEffect, forceFieldId), nativeEffect);
		vectorTightness = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetVectorTightness(nativeEffect, forceFieldId), nativeEffect);
		noiseOctaves = new PixelpartStaticPropertyInt(Plugin.PixelpartForceFieldGetNoiseOctaves(nativeEffect, forceFieldId), nativeEffect);
		noiseFrequency = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoiseFrequency(nativeEffect, forceFieldId), nativeEffect);
		noisePersistence = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoisePersistence(nativeEffect, forceFieldId), nativeEffect);
		noiseLacunarity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoiseLacunarity(nativeEffect, forceFieldId), nativeEffect);
		noiseAnimationTimeScale = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetNoiseAnimationTimeScale(nativeEffect, forceFieldId), nativeEffect);
		noiseAnimationTimeBase = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetNoiseAnimationTimeBase(nativeEffect, forceFieldId), nativeEffect);
		dragVelocityInfluence = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetDragVelocityInfluence(nativeEffect, forceFieldId), nativeEffect);
		dragSizeInfluence = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetDragSizeInfluence(nativeEffect, forceFieldId), nativeEffect);
	}
}
}