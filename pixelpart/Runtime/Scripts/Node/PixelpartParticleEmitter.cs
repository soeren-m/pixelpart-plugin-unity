using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// Node that emits particles.
    /// </summary>
    /// <remarks>
    /// Particle emitters continuously spawn new particles of assigned particle types in a defined area.
    /// They also define in which direction particles move initially.
    /// </remarks>
    public class PixelpartParticleEmitter : PixelpartNode
    {
        /// <summary>
        /// Types of emitter shapes.
        /// </summary>
        public enum ShapeType : int
        {
            /// <summary>
            /// The emitter generates particles at a single point.
            /// </summary>
            Point = 0,

            /// <summary>
            /// Particles are emitted on a straight line.
            /// </summary>
            Line = 1,

            /// <summary>
            /// Particles are emitted inside a circular or elliptical shape.
            /// </summary>
            Ellipse = 2,

            /// <summary>
            /// Particles are emitted inside a rectangle.
            /// </summary>
            Rectangle = 3,

            /// <summary>
            /// Particles are emitted on a custom path consisting of connected line segments.
            /// </summary>
            Path = 4,

            /// <summary>
            /// Particles are emitted inside a sphere or ellipsoid. Only for 3D effects.
            /// </summary>
            Ellipsoid = 5,

            /// <summary>
            /// Particles are emitted inside a cube. Only for 3D effects.
            /// </summary>
            Cuboid = 6,

            /// <summary>
            /// Particles are emitted inside a cylinder. Only for 3D effects.
            /// </summary>
            Cylinder = 7
        }

        /// <summary>
        /// Particle distribution modes.
        /// </summary>
        /// <remarks>
        /// When spawning particles inside the emitter shape, particle emitters can use different techniques
        /// to determine the exact points where particles are spawned.
        /// For example, you may want to spawn particles near the edge of the shape or on in a grid-like pattern.
        /// </remarks>
        public enum DistributionType : int
        {
            /// <summary>
            /// Particles are spawned at randomly generated locations (uniform distribution) inside the emitter area.
            /// </summary>
            Uniform = 0,

            /// <summary>
            /// Particles are created mostly around the center and less at the boundary.
            /// </summary>
            Center = 1,

            /// <summary>
            /// More particles spawn near the emitter boundary and less in the center.
            /// </summary>
            Hole = 2,

            /// <summary>
            /// Particles are created only on the emitter boundary.
            /// </summary>
            Boundary = 3,

            /// <summary>
            /// Particles spawn at random points of a grid.
            /// </summary>
            GridRandom = 4,

            /// <summary>
            /// Particles are created on points in a grid, one grid point after the other.
            /// </summary>
            GridOrdered = 5
        }

        /// <summary>
        /// Order of points for grid distribution mode.
        /// </summary>
        public enum GridOrderType : int
        {
            XYZ = 0,
            XZY = 1,
            YXZ = 2,
            YZX = 3,
            ZXY = 4,
            ZYX = 5
        }

        /// <summary>
        /// Modes for particle emission.
        /// </summary>
        /// <remarks>
        /// The emission mode of particle emitters controls when particles are created during the lifetime of the emitter.
        /// </remarks>
        public enum EmissionModeType : int
        {
            /// <summary>
            /// The emitter continuously spawns particles throughout its lifetime.
            /// </summary>
            Continuous = 0,

            /// <summary>
            /// The emitter instantiates all particles immediately after being created.
            /// </summary>
            BurstStart = 1,

            /// <summary>
            /// Particles are spawned at the end of the emitter lifetime, or
            /// - if the emitter is a sub-emitter - when particles of the parent emitter
            /// reach the end of their lifetime.
            /// </summary>
            BurstEnd = 2
        }

        /// <summary>
        /// Modes to determine the initial direction of particles.
        /// </summary>
        /// <remarks>
        /// When a particle is created by the particle emitter,
        /// it decides in which direction to emit the particle
        /// based on the specified direction and the given direction mode.
        /// </remarks>
        public enum DirectionModeType : int
        {
            /// <summary>
            /// Particles shoot off in a common direction.
            /// </summary>
            Fixed = 0,

            /// <summary>
            /// Particles move away from the emitter.
            /// </summary>
            Outwards = 1,

            /// <summary>
            /// Particles move towards the emitter’s center.
            /// </summary>
            Inwards = 2,

            /// <summary>
            /// Particles move along the direction of the parent particle or emitter.
            /// </summary>
            Inherit = 3,

            /// <summary>
            /// Particles move against the direction of the parent particle or emitter.
            /// </summary>
            InheritInverse = 4
        }

        /// <summary>
        /// Shape of the emitter area.
        /// </summary>
        public ShapeType Shape
        {
            get => (ShapeType)Plugin.PixelpartParticleEmitterGetShape(effectRuntime, Id);
            set => Plugin.PixelpartParticleEmitterSetShape(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Number of points in the emitter shape.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        public int ShapePointCount => Plugin.PixelpartParticleEmitterGetShapePointCount(effectRuntime, Id);

        /// <summary>
        /// Where particles are spawned inside the emitter area.
        /// </summary>
        public DistributionType Distribution
        {
            get => (DistributionType)Plugin.PixelpartParticleEmitterGetDistribution(effectRuntime, Id);
            set => Plugin.PixelpartParticleEmitterSetDistribution(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Grid order of grid distribution modes.
        /// </summary>
        public GridOrderType GridOrder
        {
            get => (GridOrderType)Plugin.PixelpartParticleEmitterGetGridOrder(effectRuntime, Id);
            set => Plugin.PixelpartParticleEmitterSetGridOrder(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Number of grid cells.
        /// </summary>
        /// <remarks>
        /// Only applicable for grid distribution modes.
        /// </remarks>
        public Vector3Int GridSize
        {
            get => new Vector3Int(
                    Plugin.PixelpartParticleEmitterGetGridSizeX(effectRuntime, Id),
                    Plugin.PixelpartParticleEmitterGetGridSizeY(effectRuntime, Id),
                    Plugin.PixelpartParticleEmitterGetGridSizeZ(effectRuntime, Id));
            set => Plugin.PixelpartParticleEmitterSetGridSize(effectRuntime, Id, value.x, value.y, value.z);
        }

        /// <summary>
        /// Emission mode to control when particles are created
        /// during the lifetime of the emitter.
        /// </summary>
        public EmissionModeType EmissionMode
        {
            get => (EmissionModeType)Plugin.PixelpartParticleEmitterGetEmissionMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleEmitterSetEmissionMode(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Mode to control initial particle direction.
        /// </summary>
        /// <remarks>
        /// When a particle is created by the particle emitter,
        /// it decides in which direction to emit the particle based on the specified direction and the given direction mode.
        /// </remarks>
        public DirectionModeType DirectionMode
        {
            get => (DirectionModeType)Plugin.PixelpartParticleEmitterGetDirectionMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleEmitterSetDirectionMode(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Direction in which particles move when spawned, in degrees.
        /// </summary>
        /// <remarks>
        /// For 3D effects, the direction is composed of roll, yaw and pitch.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat3 Direction { get; }

        /// <summary>
        /// How much the direction varies.
        /// </summary>
        /// <remarks>
        /// A spread of 0° means that particles only move in the specified direction,
        /// while 360° leads to particles shooting off in all directions.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat Spread { get; }

        /// <summary>
        /// Number of points in the emitter shape.
        /// <b>Deprecated</b>, use <see cref="ShapePointCount"/>.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        [Obsolete("deprecated, use ShapePointCount")]
        public int NumShapePoints => ShapePointCount;

        /// <summary>
        /// Construct <see cref="PixelpartParticleEmitter"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartParticleEmitter(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            Direction = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleEmitterGetDirection(effectRuntimePtr, id));
            Spread = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleEmitterGetSpread(effectRuntimePtr, id));
        }

        /// <summary>
        /// Add a new point to the emitter shape.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        /// <param name="point">Point to add</param>
        public void AddShapePoint(Vector3 point) =>
            Plugin.PixelpartParticleEmitterAddShapePoint(effectRuntime, Id, point);

        /// <summary>
        /// Remove the point at the given index from the emitter shape.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        /// <param name="index">Index of point to remove</param>
        public void RemoveShapePoint(int index) =>
            Plugin.PixelpartParticleEmitterRemoveShapePoint(effectRuntime, Id, index);

        /// <summary>
        /// Change the location of the point at the given index in the emitter shape.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        /// <param name="index">Index of point to change</param>
        /// <param name="point">New location</param>
        public void SetShapePoint(int index, Vector3 point) =>
            Plugin.PixelpartParticleEmitterSetShapePoint(effectRuntime, Id, index, point);

        /// <summary>
        /// Return the location of the point at the given index in the emitter shape.
        /// </summary>
        /// <remarks>
        /// Only applicable for emitter shape <c>Path</c>.
        /// </remarks>
        /// <param name="index">Index of point</param>
        /// <returns>Location of point</returns>
        public Vector3 GetShapePoint(int index) =>
            Plugin.PixelpartParticleEmitterGetShapePoint(effectRuntime, Id, index);
    }
}