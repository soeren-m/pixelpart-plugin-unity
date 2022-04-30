using System;
using UnityEngine;

namespace pixelpart {
public enum InterpolationType : int {
	Step = 0,
	Linear = 1,
	Spline = 2
}

public enum BlendModeType : int {
	Normal = 0,
	Additive = 1,
	Subtractive = 2
}

public enum ColorModeType : int {
	Multiply = 0,
	Add = 1,
	Subtract = 2,
	Difference = 3,
	Screen = 4,
	Overlay = 5,
	Lighten = 6,
	Darken = 7,
	ColorOnly = 8
}

public enum EmitterShapeType : int {
	Point = 0,
	Line = 1,
	Ellipse = 2,
	Rectangle = 3
}

public enum EmitterDistributionType : int {
	Uniform = 0,
	Center = 1,
	CenterReverse = 2,
	Boundary = 3
}

public enum EmitterSpawnModeType : int {
	Fixed = 0,
	Out = 1,
	In = 2
}

public enum RotationModeType : int {
	Angle = 0,
	Velocity = 1,
	Align = 2,
	AlignPath = 3
}

public enum ForceType : int {
	Point = 0,
	Area = 1
}

public enum BillboardMode : int {
	Disabled = 0,
	Enabled = 1,
	EnabledFixedY = 2,
}
}