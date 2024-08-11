using System;
using UnityEngine;

namespace Pixelpart {
[Serializable]
public struct PixelpartVariantValue {
	public enum VariantType {
		Null = -1,
		Bool,
		Int,
		Float,
		Float2,
		Float3,
		Float4
	}

	public VariantType type;
	public float x;
	public float y;
	public float z;
	public float w;

	public PixelpartVariantValue(bool b) {
		type = VariantType.Bool;
		x = b ? 1.0f : 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
	public PixelpartVariantValue(int i) {
		type = VariantType.Int;
		x = (float)i;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
	public PixelpartVariantValue(float f) {
		type = VariantType.Float;
		x = f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
	public PixelpartVariantValue(Vector2 v) {
		type = VariantType.Float2;
		x = v.x;
		y = v.y;
		z = 0.0f;
		w = 0.0f;
	}
	public PixelpartVariantValue(Vector3 v) {
		type = VariantType.Float3;
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.0f;
	}
	public PixelpartVariantValue(Vector4 v) {
		type = VariantType.Float4;
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	public PixelpartVariantValue(VariantType t, Vector4 v) {
		type = t;
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
}
}