using System;
using System.Text;

namespace Pixelpart
{
    /// <summary>
    /// Defines the appearance and behavior of particles.
    /// </summary>
    /// <remarks>
    /// Each particle emitter has a list of assigned particle types, which it will use for generating particles.
    /// The particle type defines how particles move through space (e.g. velocity, acceleration) and
    /// how they look (e.g. size, color).
    ///
    /// <para>
    /// Particle types can be assigned a parent particle type. This causes a particle of the parent type
    /// to emit particles of the child particle type.
    /// </para>
    ///
    /// <para>
    /// There are three types of particles, which determines how they are rendered.
    /// <list type="bullet">
    /// <item>Sprite particle: The sprite renderer renders each particle as a separate sprite.</item>
    /// <item>Trail particle: The trail renderer generates continuous trails along the path of particles
    /// with the same parent particle.</item>
    /// <item>Mesh particle: The mesh renderer renders each particles as a three-dimensional mesh.</item>
    /// </list>
    /// </para>
    /// </remarks>
    public class PixelpartParticleType
    {
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

        private readonly IntPtr effectRuntime;

        /// <summary>
        /// ID of the particle type.
        /// </summary>
        public uint Id { get; }

        /// <summary>
        /// ID of the parent particle type.
        /// </summary>
        public uint ParentId => PixelpartPlugin.PixelpartParticleTypeGetParentId(effectRuntime, Id);

        /// <summary>
        /// Name of the particle type.
        /// </summary>
        public string Name
        {
            get
            {
                var buffer = new byte[256];
                var size = PixelpartPlugin.PixelpartParticleTypeGetName(effectRuntime, Id, buffer, buffer.Length);

                return Encoding.UTF8.GetString(buffer, 0, size);
            }
        }

        /// <summary>
        /// Whether the position of particles is tied to the position of the emitter.
        /// </summary>
        public bool PositionRelative
        {
            get => PixelpartPlugin.PixelpartParticleTypeIsPositionRelative(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartParticleTypeSetPositionRelative(effectRuntime, Id, value);
        }

        /// <summary>
        /// Whether the rotation property represents the exact rotation of particles
        /// or their angular velocity.
        /// </summary>
        public RotationModeType RotationMode
        {
            get => (RotationModeType)PixelpartPlugin.PixelpartParticleTypeGetRotationMode(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartParticleTypeSetRotationMode(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Direction to which particles are aligned.
        /// </summary>
        public AlignmentModeType AlignmentMode
        {
            get => (AlignmentModeType)PixelpartPlugin.PixelpartParticleTypeGetAlignmentMode(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartParticleTypeSetAlignmentMode(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Center of rotation relative to the particle’s center.
        /// </summary>
        public PixelpartStaticPropertyFloat3 Pivot { get; }

        /// <summary>
        /// How much particles are influenced by the motion path.
        /// </summary>
        /// <remarks>
        /// Set to 0 to disable motion paths.
        /// </remarks>
        public PixelpartStaticPropertyFloat MotionPathForce { get; }

        /// <summary>
        /// Whether particles of this type are visible.
        /// </summary>
        public bool Visible
        {
            get => PixelpartPlugin.PixelpartParticleTypeIsVisible(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartParticleTypeSetVisible(effectRuntime, Id, value);
        }

        /// <summary>
        /// Layer particles are drawn on.
        /// </summary>
        /// <remarks>
        /// Particles on upper layers occlude those on lower layers.
        /// </remarks>
        public int Layer
        {
            get => PixelpartPlugin.PixelpartParticleTypeGetLayer(effectRuntime, Id);
            set => PixelpartPlugin.PixelpartParticleTypeSetLayer(effectRuntime, Id, value);
        }

        /// <summary>
        /// How many particles of this type are spawned by emitters.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Count { get; }

        /// <summary>
        /// How long particles exist after being spawned by emitters.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Lifespan { get; }

        /// <summary>
        /// Size of each particle when created.
        /// </summary>
        public PixelpartAnimatedPropertyFloat InitialSize { get; }

        /// <summary>
        /// Rotation of each particle when created, in degrees.
        /// </summary>
        /// <remarks>
        /// For 3D effects, rotation is composed of roll, yaw and pitch.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat3 InitialRotation { get; }

        /// <summary>
        /// How fast each particle is initially after being spawned by the emitter.
        /// </summary>
        public PixelpartAnimatedPropertyFloat InitialVelocity { get; }

        /// <summary>
        /// How much speed particles inherit from the parent particle or emitter.
        /// </summary>
        public PixelpartAnimatedPropertyFloat InheritedVelocity { get; }

        /// <summary>
        /// Opacity of each particle when created.
        /// </summary>
        public PixelpartAnimatedPropertyFloat InitialOpacity { get; }

        /// <summary>
        /// Motion path for particles.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 Position { get; }

        /// <summary>
        /// Size of each particle over its lifetime as a fraction of its initial size.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 Size { get; }

        /// <summary>
        /// How much particles are stretched based on their speed.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 Stretch { get; }

        /// <summary>
        /// Relative size of each particle for motion and collision calculations over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat PhysicalSize { get; }

        /// <summary>
        /// How particles rotate over time from their initial rotation, in degrees.
        /// </summary>
        /// <remarks>
        /// For 3D effects, rotation is composed of roll, yaw and pitch.
        /// </remarks>
        public PixelpartAnimatedPropertyFloat3 Rotation { get; }

        /// <summary>
        /// How much particles rotate based on their velocity, in degrees.
        /// </summary>
        public PixelpartAnimatedPropertyFloat3 RotationBySpeed { get; }

        /// <summary>
        /// Linear acceleration applied to each particle over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Acceleration { get; }

        /// <summary>
        /// Acceleration of each particle towards the emitter over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat RadialAcceleration { get; }

        /// <summary>
        /// How strongly each particle is affected by force fields over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Weight { get; }

        /// <summary>
        /// How much each particle bounces back from colliders over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Bounce { get; }

        /// <summary>
        /// How resistant each particle is to sliding down colliders over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Friction { get; }

        /// <summary>
        /// Color of each particle over its lifetime.
        /// </summary>
        public PixelpartAnimatedPropertyFloat4 Color { get; }

        /// <summary>
        /// Opacity of each particle over its lifetime as a fraction of its initial opacity.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Opacity { get; }

        /// <summary>
        /// How much the lifespan varies between particles.
        /// </summary>
        public PixelpartStaticPropertyFloat LifespanVariance { get; }

        /// <summary>
        /// How much the size varies between particles.
        /// </summary>
        public PixelpartStaticPropertyFloat SizeVariance { get; }

        /// <summary>
        /// How much the rotation varies between particles, in degrees.
        /// </summary>
        public PixelpartStaticPropertyFloat3 RotationVariance { get; }

        /// <summary>
        /// How much the angular velocity varies between particles, in degrees/second.
        /// </summary>
        public PixelpartStaticPropertyFloat3 AngularVelocityVariance { get; }

        /// <summary>
        /// How much the speed varies between particles.
        /// </summary>
        public PixelpartStaticPropertyFloat VelocityVariance { get; }

        /// <summary>
        /// How much the color varies between particles.
        /// </summary>
        public PixelpartStaticPropertyFloat4 ColorVariance { get; }

        /// <summary>
        /// How much the opacity varies between particles.
        /// </summary>
        public PixelpartStaticPropertyFloat OpacityVariance { get; }

        /// <summary>
        /// Construct <see cref="PixelpartParticleType"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Particle type ID</param>
        public PixelpartParticleType(IntPtr effectRuntimePtr, uint id)
        {
            effectRuntime = effectRuntimePtr;
            Id = id;

            Pivot = new PixelpartStaticPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetPivot(effectRuntimePtr, id));

            MotionPathForce = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetMotionPathForce(effectRuntimePtr, id));

            Count = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetCount(effectRuntimePtr, id));
            Lifespan = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetLifespan(effectRuntimePtr, id));
            InitialSize = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetInitialSize(effectRuntimePtr, id));
            InitialRotation = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetInitialRotation(effectRuntimePtr, id));
            InitialVelocity = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetInitialVelocity(effectRuntimePtr, id));
            InheritedVelocity = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetInheritedVelocity(effectRuntimePtr, id));
            InitialOpacity = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetInitialOpacity(effectRuntimePtr, id));

            Position = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetMotionPath(effectRuntimePtr, id));
            Size = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetSize(effectRuntimePtr, id));
            Stretch = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetStretch(effectRuntimePtr, id));
            PhysicalSize = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetPhysicalSize(effectRuntimePtr, id));
            Rotation = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetRotation(effectRuntimePtr, id));
            RotationBySpeed = new PixelpartAnimatedPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetRotationBySpeed(effectRuntimePtr, id));
            Acceleration = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetAcceleration(effectRuntimePtr, id));
            RadialAcceleration = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetRadialAcceleration(effectRuntimePtr, id));
            Weight = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetWeight(effectRuntimePtr, id));
            Bounce = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetBounce(effectRuntimePtr, id));
            Friction = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetFriction(effectRuntimePtr, id));
            Color = new PixelpartAnimatedPropertyFloat4(
                PixelpartPlugin.PixelpartParticleTypeGetColor(effectRuntimePtr, id));
            Opacity = new PixelpartAnimatedPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetOpacity(effectRuntimePtr, id));

            LifespanVariance = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetLifespanVariance(effectRuntimePtr, id));
            SizeVariance = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetSizeVariance(effectRuntimePtr, id));
            RotationVariance = new PixelpartStaticPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetRotationVariance(effectRuntimePtr, id));
            AngularVelocityVariance = new PixelpartStaticPropertyFloat3(
                PixelpartPlugin.PixelpartParticleTypeGetAngularVelocityVariance(effectRuntimePtr, id));
            VelocityVariance = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetVelocityVariance(effectRuntimePtr, id));
            ColorVariance = new PixelpartStaticPropertyFloat4(
                PixelpartPlugin.PixelpartParticleTypeGetColorVariance(effectRuntimePtr, id));
            OpacityVariance = new PixelpartStaticPropertyFloat(
                PixelpartPlugin.PixelpartParticleTypeGetOpacityVariance(effectRuntimePtr, id));
        }
    }
}
