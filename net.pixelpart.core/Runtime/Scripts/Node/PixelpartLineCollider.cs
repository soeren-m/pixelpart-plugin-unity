using System;
using UnityEngine;

namespace Pixelpart
{
    /// <summary>
    /// Collider consisting of several connected line segments (2D only).
    /// </summary>
    /// <remarks>
    /// Line colliders combine several collision segments together and
    /// are defined by a set of points at the vertices of the line segments.
    /// </remarks>
    public class PixelpartLineCollider : PixelpartCollider
    {
        /// <summary>
        /// Number of vertices in the line collider.
        /// </summary>
        public int PointCount => Plugin.PixelpartLineColliderGetPointCount(effectRuntime, Id);

        /// <summary>
        /// Number of vertices in the line collider.
        /// <b>Deprecated</b>, use <see cref="PointCount"/>.
        /// </summary>
        [Obsolete("deprecated, use PointCount")]
        public int NumPoints => PointCount;

        /// <summary>
        /// Construct <see cref="PixelpartLineCollider"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartLineCollider(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {

        }

        /// <summary>
        /// Add a vertex to the end of the line collider.
        /// </summary>
        /// <param name="point">Point to add</param>
        public void AddPoint(Vector3 point) =>
            Plugin.PixelpartLineColliderAddPoint(effectRuntime, Id, point);

        /// <summary>
        /// Change the location of a vertex in the line collider.
        /// </summary>
        /// <param name="index">Index of the vertex</param>
        /// <param name="point">New location</param>
        public void SetPoint(int index, Vector3 point) =>
            Plugin.PixelpartLineColliderSetPoint(effectRuntime, Id, index, point);

        /// <summary>
        /// Remove a vertex from the line collider.
        /// </summary>
        /// <param name="index">Index of vertex to remove</param>
        public void RemovePoint(int index) =>
            Plugin.PixelpartLineColliderRemovePoint(effectRuntime, Id, index);

        /// <summary>
        /// Return the location of a vertex in the line collider.
        /// </summary>
        /// <param name="index">Index of the vertex</param>
        /// <returns>Vertex location</returns>
        public Vector3 GetPoint(int index) =>
            Plugin.PixelpartLineColliderGetPoint(effectRuntime, Id, index);
    }
}
