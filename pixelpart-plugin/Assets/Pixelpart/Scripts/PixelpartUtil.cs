using System;
using UnityEngine;

namespace pixelpart {
public enum InterpolationType : int {
	None = 0,
	Linear = 1,
	Spline = 2
}

public enum BlendModeType : int {
	Normal = 0,
	Additive = 1,
	Subtractive = 2
}

public enum EmitterShapeType : int {
	Point = 0,
	Line = 1,
	Ellipse = 2,
	Rectangle = 3,
	Path = 4
}

public enum EmitterDistributionType : int {
	Uniform = 0,
	Center = 1,
	CenterReverse = 2,
	Boundary = 3
}

public enum EmitterSpawnMode : int {
	Fixed = 0,
	Out = 1,
	In = 2
}

public enum EmitterInstantiationMode : int {
	Continuous = 0,
	Burst = 1,
	BurstDeath = 2
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