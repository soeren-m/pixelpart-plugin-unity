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
        private readonly IntPtr effectRuntime;

        /// <summary>
        /// ID of the particle type.
        /// </summary>
        public uint Id { get; }

        /// <summary>
        /// ID of the parent particle type.
        /// </summary>
        public uint ParentId => Plugin.PixelpartParticleTypeGetParentId(effectRuntime, Id);

        /// <summary>
        /// Name of the particle type.
        /// </summary>
        public string Name
        {
            get
            {
                var buffer = new byte[256];
                var size = Plugin.PixelpartParticleTypeGetName(effectRuntime, Id, buffer, buffer.Length);

                return Encoding.UTF8.GetString(buffer, 0, size);
            }
        }

        /// <summary>
        /// Whether the position of particles is tied to the position of the emitter.
        /// </summary>
        public bool PositionRelative
        {
            get => Plugin.PixelpartParticleTypeIsPositionRelative(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetPositionRelative(effectRuntime, Id, value);
        }

        /// <summary>
        /// Whether the rotation property represents the exact rotation of particles
        /// or their angular velocity.
        /// </summary>
        public RotationModeType RotationMode
        {
            get => (RotationModeType)Plugin.PixelpartParticleTypeGetRotationMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetRotationMode(effectRuntime, Id, (int)value);
        }

        /// <summary>
        /// Direction to which particles are aligned.
        /// </summary>
        public AlignmentModeType AlignmentMode
        {
            get => (AlignmentModeType)Plugin.PixelpartParticleTypeGetAlignmentMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetAlignmentMode(effectRuntime, Id, (int)value);
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
            get => Plugin.PixelpartParticleTypeIsVisible(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetVisible(effectRuntime, Id, value);
        }

        /// <summary>
        /// Layer particles are drawn on.
        /// </summary>
        /// <remarks>
        /// Particles on upper layers occlude those on lower layers.
        /// </remarks>
        public int Layer
        {
            get => Plugin.PixelpartParticleTypeGetLayer(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetLayer(effectRuntime, Id, value);
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
        /// How many particles of this type are spawned by emitters.
        /// <b>Deprecated</b>, use <see cref="Count"/>.
        /// </summary>
        [Obsolete("deprecated, use Count")]
        public PixelpartAnimatedPropertyFloat NumParticles => Count;

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
                Plugin.PixelpartParticleTypeGetPivot(effectRuntimePtr, id));

            MotionPathForce = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartParticleTypeGetMotionPathForce(effectRuntimePtr, id));

            Count = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetCount(effectRuntimePtr, id));
            Lifespan = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetLifespan(effectRuntimePtr, id));
            InitialSize = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetInitialSize(effectRuntimePtr, id));
            InitialRotation = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetInitialRotation(effectRuntimePtr, id));
            InitialVelocity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetInitialVelocity(effectRuntimePtr, id));
            InheritedVelocity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetInheritedVelocity(effectRuntimePtr, id));
            InitialOpacity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetInitialOpacity(effectRuntimePtr, id));

            Position = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetMotionPath(effectRuntimePtr, id));
            Size = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetSize(effectRuntimePtr, id));
            Stretch = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetStretch(effectRuntimePtr, id));
            PhysicalSize = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetPhysicalSize(effectRuntimePtr, id));
            Rotation = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetRotation(effectRuntimePtr, id));
            RotationBySpeed = new PixelpartAnimatedPropertyFloat3(
                Plugin.PixelpartParticleTypeGetRotationBySpeed(effectRuntimePtr, id));
            Acceleration = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetAcceleration(effectRuntimePtr, id));
            RadialAcceleration = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetRadialAcceleration(effectRuntimePtr, id));
            Weight = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetWeight(effectRuntimePtr, id));
            Bounce = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetBounce(effectRuntimePtr, id));
            Friction = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetFriction(effectRuntimePtr, id));
            Color = new PixelpartAnimatedPropertyFloat4(
                Plugin.PixelpartParticleTypeGetColor(effectRuntimePtr, id));
            Opacity = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartParticleTypeGetOpacity(effectRuntimePtr, id));

            LifespanVariance = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartParticleTypeGetLifespanVariance(effectRuntimePtr, id));
            SizeVariance = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartParticleTypeGetSizeVariance(effectRuntimePtr, id));
            RotationVariance = new PixelpartStaticPropertyFloat3(
                Plugin.PixelpartParticleTypeGetRotationVariance(effectRuntimePtr, id));
            AngularVelocityVariance = new PixelpartStaticPropertyFloat3(
                Plugin.PixelpartParticleTypeGetAngularVelocityVariance(effectRuntimePtr, id));
            VelocityVariance = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartParticleTypeGetVelocityVariance(effectRuntimePtr, id));
            ColorVariance = new PixelpartStaticPropertyFloat4(
                Plugin.PixelpartParticleTypeGetColorVariance(effectRuntimePtr, id));
            OpacityVariance = new PixelpartStaticPropertyFloat(
                Plugin.PixelpartParticleTypeGetOpacityVariance(effectRuntimePtr, id));
        }
    }
}
