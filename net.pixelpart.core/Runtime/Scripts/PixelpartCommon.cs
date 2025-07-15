namespace Pixelpart
{
    /// <summary>
    /// Keyframe interpolation types.
    /// </summary>
    public enum InterpolationType : int
    {
        /// <summary>
        /// Values change abruptly between keyframes.
        /// </summary>
        Off = 0,

        /// <summary>
        /// Values are interpolated linearly.
        /// </summary>
        Linear = 1,

        /// <summary>
        /// Values are interpolated with Spline interpolation producing smooth transitions.
        /// </summary>
        Spline = 2,

        /// <summary>
        /// Values are interpolated with Bezier interpolation producing smooth transitions.
        /// </summary>
        Bezier = 3
    }

    /// <summary>
    /// Mode that determines how particles are blended together and are composed with other objects in the effect.
    /// </summary>
    public enum BlendModeType : int
    {
        /// <summary>
        /// Particles are not blended together and occlude each other.
        /// </summary>
        Off = 0,

        /// <summary>
        /// Particles are blended together based on their opacity.
        /// </summary>
        Normal = 1,

        /// <summary>
        /// Particle colors are added together.
        /// </summary>
        Additive = 2,

        /// <summary>
        /// Particle colors are subtracted from each other.
        /// </summary>
        Subtractive = 3
    }

    /// <summary>
    /// Mode that determines whether the material is shaded by lights in the scene.
    /// </summary>
    public enum LightingModeType : int
    {
        /// <summary>
        /// The material is not shaded based on lighting.
        /// </summary>
        Unlit = 0,

        /// <summary>
        /// The material is shaded based on lighting.
        /// </summary>
        Lit = 1
    }

    /// <summary>
    /// Whether the rotation property represents the exact rotation of particles or their angular velocity.
    /// </summary>
    public enum RotationModeType : int
    {
        /// <summary>
        /// Rotation property defines angle of rotation in degrees.
        /// </summary>
        Angle = 0,

        /// <summary>
        /// Rotation property defines angular velocity in degrees/second.
        /// </summary>
        Velocity = 1
    }

    /// <summary>
    /// Direction to which particles are aligned.
    /// </summary>
    public enum AlignmentModeType : int
    {
        /// <summary>
        /// Particles do not align to anything.
        /// </summary>
        None = 0,

        /// <summary>
        /// Particles always face the camera (3D).
        /// </summary>
        Camera = 1,

        /// <summary>
        /// Particles align to their own velocity vector.
        /// </summary>
        Motion = 2,

        /// <summary>
        /// Particles face their emitter.
        /// </summary>
        Emission = 3,

        /// <summary>
        /// Particles copy the rotation of their emitter.
        /// </summary>
        Emitter = 4
    }

    /// <summary>
    /// Types determining how particles are rendered.
    /// </summary>
    public enum ParticleRendererType : int
    {
        /// <summary>
        /// The sprite renderer renders each particle as a separate sprite.
        /// </summary>
        Sprite = 0,

        /// <summary>
        /// The trail renderer generates continuous trails along the path of particles with the same parent particle.
        /// </summary>
        Trail = 1,

        /// <summary>
        /// The mesh renderer renders each particles as a three-dimensional mesh.
        /// </summary>
        Mesh = 2
    }
}