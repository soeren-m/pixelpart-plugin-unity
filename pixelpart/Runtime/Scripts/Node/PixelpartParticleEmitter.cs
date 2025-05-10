using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartParticleEmitter : PixelpartNode {
	public enum ShapeType : int {
		Point = 0,
		Line = 1,
		Ellipse = 2,
		Rectangle = 3,
		Path = 4,
		Ellipsoid = 5,
		Cuboid = 6,
		Cylinder = 7
	}
	public enum DistributionType : int {
		Uniform = 0,
		Center = 1,
		Hole = 2,
		Boundary = 3,
		GridRandom = 4,
		GridOrdered = 5
	}
	public enum GridOrderType : int {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5
	}
	public enum EmissionModeType : int {
		Continuous = 0,
		BurstStart = 1,
		BurstEnd = 2
	}
	public enum DirectionModeType : int {
		Fixed = 0,
		Outwards = 1,
		Inwards = 2,
		Inherit = 3,
		InheritInverse = 4
	}

	public ShapeType Shape {
		get => (ShapeType)Plugin.PixelpartParticleEmitterGetShape(effectRuntime, Id);
		set => Plugin.PixelpartParticleEmitterSetShape(effectRuntime, Id, (int)value);
	}

	public int ShapePointCount => Plugin.PixelpartParticleEmitterGetShapePointCount(effectRuntime, Id);

	public DistributionType Distribution {
		get => (DistributionType)Plugin.PixelpartParticleEmitterGetDistribution(effectRuntime, Id);
		set => Plugin.PixelpartParticleEmitterSetDistribution(effectRuntime, Id, (int)value);
	}

	public GridOrderType GridOrder {
		get => (GridOrderType)Plugin.PixelpartParticleEmitterGetGridOrder(effectRuntime, Id);
		set => Plugin.PixelpartParticleEmitterSetGridOrder(effectRuntime, Id, (int)value);
	}

	public Vector3Int GridSize {
		get => new Vector3Int(
				Plugin.PixelpartParticleEmitterGetGridSizeX(effectRuntime, Id),
				Plugin.PixelpartParticleEmitterGetGridSizeY(effectRuntime, Id),
				Plugin.PixelpartParticleEmitterGetGridSizeZ(effectRuntime, Id));
		set => Plugin.PixelpartParticleEmitterSetGridSize(effectRuntime, Id, value.x, value.y, value.z);
	}

	public EmissionModeType EmissionMode {
		get => (EmissionModeType)Plugin.PixelpartParticleEmitterGetEmissionMode(effectRuntime, Id);
		set => Plugin.PixelpartParticleEmitterSetEmissionMode(effectRuntime, Id, (int)value);
	}

	public DirectionModeType DirectionMode {
		get => (DirectionModeType)Plugin.PixelpartParticleEmitterGetDirectionMode(effectRuntime, Id);
		set => Plugin.PixelpartParticleEmitterSetDirectionMode(effectRuntime, Id, (int)value);
	}

	public PixelpartAnimatedPropertyFloat3 Direction { get; }

	public PixelpartAnimatedPropertyFloat Spread { get; }

	[Obsolete("deprecated, use ShapePointCount")]
	public int NumShapePoints => ShapePointCount;

	public PixelpartParticleEmitter(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {
		Direction = new PixelpartAnimatedPropertyFloat3(
			Plugin.PixelpartParticleEmitterGetDirection(effectRuntimePtr, id));
		Spread = new PixelpartAnimatedPropertyFloat(
			Plugin.PixelpartParticleEmitterGetSpread(effectRuntimePtr, id));
	}

	public void AddShapePoint(Vector3 point) =>
		Plugin.PixelpartParticleEmitterAddShapePoint(effectRuntime, Id, point);

	public void RemoveShapePoint(int index) =>
		Plugin.PixelpartParticleEmitterRemoveShapePoint(effectRuntime, Id, index);

	public void SetShapePoint(int index, Vector3 point) =>
		Plugin.PixelpartParticleEmitterSetShapePoint(effectRuntime, Id, index, point);

	public Vector3 GetShapePoint(int index) =>
		Plugin.PixelpartParticleEmitterGetShapePoint(effectRuntime, Id, index);
}
}