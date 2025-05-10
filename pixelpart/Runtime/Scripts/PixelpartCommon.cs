namespace Pixelpart {
public enum InterpolationType : int {
	Off = 0,
	Linear = 1,
	Spline = 2,
	Bezier = 3
}

public enum BlendModeType : int {
	Off = 0,
	Normal = 1,
	Additive = 2,
	Subtractive = 3
}

public enum LightingModeType : int {
	Unlit = 0,
	Lit = 1
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

public enum ParticleRendererType : int {
	Sprite = 0,
	Trail = 1,
	Mesh = 2
}
}