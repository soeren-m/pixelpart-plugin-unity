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
			int size = Plugin.PixelpartForceFieldGetName(internalEffect, forceFieldId, buffer, buffer.Length);

			return System.Text.Encoding.UTF8.GetString(buffer, 0, size);
		}
	}

	public float LifetimeStart {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeStart(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeStart(internalEffect, forceFieldId, value);
		}
	}
	public float LifetimeDuration {
		get {
			return Plugin.PixelpartForceFieldGetLifetimeDuration(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetLifetimeDuration(internalEffect, forceFieldId, value);
		}
	}
	public bool Repeat {
		get {
			return Plugin.PixelpartForceFieldGetRepeat(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetRepeat(internalEffect, forceFieldId, value);
		}
	}
	public bool Active {
		get {
			return Plugin.PixelpartForceFieldIsActive(internalEffect, forceFieldId);
		}
	}
	public float LocalTime {
		get {
			return Plugin.PixelpartForceFieldGetLocalTime(internalEffect, forceFieldId);
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
			return (ForceType)Plugin.PixelpartForceFieldGetType(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetType(internalEffect, forceFieldId, (int)value);
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
				Plugin.PixelpartForceFieldGetAccelerationGridWidth(internalEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetAccelerationGridHeight(internalEffect, forceFieldId),
				Plugin.PixelpartForceFieldGetAccelerationGridDepth(internalEffect, forceFieldId));
		}
		set {
			Plugin.PixelpartForceFieldSetAccelerationGridSize(internalEffect, forceFieldId, value.x, value.y, value.z);
		}
	}

	public VectorFieldFilter VectorFilter {
		get {
			return (VectorFieldFilter)Plugin.PixelpartForceFieldGetVectorFilter(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetVectorFilter(internalEffect, forceFieldId, (int)value);
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
			return Plugin.PixelpartForceFieldGetNoiseAnimated(internalEffect, forceFieldId);
		}
		set {
			Plugin.PixelpartForceFieldSetNoiseAnimated(internalEffect, forceFieldId, value);
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

	private readonly IntPtr internalEffect;

	public PixelpartForceField(IntPtr internalEffectPtr, uint nativeForceFieldId) {
		internalEffect = internalEffectPtr;
		forceFieldId = nativeForceFieldId;

		position = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetPosition(internalEffect, forceFieldId));
		size = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetSize(internalEffect, forceFieldId));
		orientation = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetOrientation(internalEffect, forceFieldId));
		strength = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetStrength(internalEffect, forceFieldId));
		accelerationDirection = new PixelpartAnimatedPropertyFloat3(Plugin.PixelpartForceFieldGetAccelerationDirection(internalEffect, forceFieldId));
		accelerationDirectionVariance = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetAccelerationDirectionVariance(internalEffect, forceFieldId));
		accelerationStrengthVariance = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetAccelerationStrengthVariance(internalEffect, forceFieldId));
		vectorTightness = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetVectorTightness(internalEffect, forceFieldId));
		noiseOctaves = new PixelpartStaticPropertyInt(Plugin.PixelpartForceFieldGetNoiseOctaves(internalEffect, forceFieldId));
		noiseFrequency = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoiseFrequency(internalEffect, forceFieldId));
		noisePersistence = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoisePersistence(internalEffect, forceFieldId));
		noiseLacunarity = new PixelpartAnimatedPropertyFloat(Plugin.PixelpartForceFieldGetNoiseLacunarity(internalEffect, forceFieldId));
		noiseAnimationTimeScale = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetNoiseAnimationTimeScale(internalEffect, forceFieldId));
		noiseAnimationTimeBase = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetNoiseAnimationTimeBase(internalEffect, forceFieldId));
		dragVelocityInfluence = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetDragVelocityInfluence(internalEffect, forceFieldId));
		dragSizeInfluence = new PixelpartStaticPropertyFloat(Plugin.PixelpartForceFieldGetDragSizeInfluence(internalEffect, forceFieldId));
	}
}
}