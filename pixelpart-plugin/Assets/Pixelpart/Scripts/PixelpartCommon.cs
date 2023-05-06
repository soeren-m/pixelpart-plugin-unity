using System;
using UnityEngine;

namespace Pixelpart {
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

public enum RotationModeType : int {
	Angle = 0,
	Velocity = 1
}

public enum AlignmentModeType : int {
	None = 0,
	Camera = 1,
	Motion = 2,
	Emission = 3,
	Emitter = 4
}
}