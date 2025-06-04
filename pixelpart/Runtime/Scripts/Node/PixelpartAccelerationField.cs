using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// Force field that accelerates particles towards a given direction.
    /// </summary>
    /// <remarks>
    /// Acceleration fields also allow you to subdivide the force area with a grid,
    /// where each grid cell has a different force direction and strength.
    /// This can be useful for organic looking particle motion.
    /// </remarks>
    public class PixelpartAccelerationField : PixelpartForceField
    {
        /// <summary>
        /// Direction in which particles are accelerated, in degrees.
        /// </summary>
        /// <remarks>
        /// For 3D effects, the direction is composed of roll, yaw and pitch.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat3 AccelerationDirection { get; }

        /// <summary>
        /// How much the force direction varies between cells of the grid.
        /// </summary>
        public PixelpartAnimatedPropertyFloat AccelerationDirectionVariance { get; }

        /// <summary>
        /// How much the force strength varies between cells of the grid.
        /// </summary>
        public PixelpartAnimatedPropertyFloat AccelerationStrengthVariance { get; }

        /// <summary>
        /// Number of cells in each dimension of the force field grid.
        /// </summary>
        /// <remarks>
        /// Each cell of the grid varies in direction and strength determined by the direction and strength variance values.
        /// </remarks>
        public Vector3Int AccelerationGridSize
        {
            get => new Vector3Int(
                Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeX(effectRuntime, Id),
                Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeY(effectRuntime, Id),
                Plugin.PixelpartAccelerationFieldGetAccelerationGridSizeZ(effectRuntime, Id));
            set => Plugin.PixelpartAccelerationFieldSetAccelerationGridSize(effectRuntime, Id,
                value.x, value.y, value.z);
        }

        /// <summary>
        /// Construct <see cref="PixelpartAccelerationField"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartAccelerationField(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            AccelerationDirection = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartAccelerationFieldGetAccelerationDirection(effectRuntimePtr, id));
            AccelerationDirectionVariance = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartAccelerationFieldGetAccelerationDirectionVariance(effectRuntimePtr, id));
            AccelerationStrengthVariance = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartAccelerationFieldGetAccelerationStrengthVariance(effectRuntimePtr, id));
        }
    }
}