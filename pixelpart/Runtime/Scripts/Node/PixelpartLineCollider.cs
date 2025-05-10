using System;
using UnityEngine;

namespace Pixelpart {
public class PixelpartLineCollider : PixelpartCollider {
	public int PointCount => Plugin.PixelpartLineColliderGetPointCount(effectRuntime, Id);

	[Obsolete("deprecated, use PointCount")]
	public int NumPoints => PointCount;

	public PixelpartLineCollider(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id) {

	}

	public void AddPoint(Vector3 point) =>
		Plugin.PixelpartLineColliderAddPoint(effectRuntime, Id, point);

	public void SetPoint(int index, Vector3 point) =>
		Plugin.PixelpartLineColliderSetPoint(effectRuntime, Id, index, point);

	public void RemovePoint(int index) =>
		Plugin.PixelpartLineColliderRemovePoint(effectRuntime, Id, index);

	public Vector3 GetPoint(int index) =>
		Plugin.PixelpartLineColliderGetPoint(effectRuntime, Id, index);
}
}