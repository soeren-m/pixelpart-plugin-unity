using System;
using System.Text;

namespace Pixelpart
{
    public class PixelpartParticleType
    {
        private readonly IntPtr effectRuntime;

        public uint Id { get; }

        public uint ParentId => Plugin.PixelpartParticleTypeGetParentId(effectRuntime, Id);

        public string Name
        {
            get
            {
                var buffer = new byte[256];
                var size = Plugin.PixelpartParticleTypeGetName(effectRuntime, Id, buffer, buffer.Length);

                return Encoding.UTF8.GetString(buffer, 0, size);
            }
        }

        public bool PositionRelative
        {
            get => Plugin.PixelpartParticleTypeIsPositionRelative(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetPositionRelative(effectRuntime, Id, value);
        }

        public RotationModeType RotationMode
        {
            get => (RotationModeType)Plugin.PixelpartParticleTypeGetRotationMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetRotationMode(effectRuntime, Id, (int)value);
        }

        public AlignmentModeType AlignmentMode
        {
            get => (AlignmentModeType)Plugin.PixelpartParticleTypeGetAlignmentMode(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetAlignmentMode(effectRuntime, Id, (int)value);
        }

        public PixelpartStaticPropertyFloat3 Pivot { get; }

        public PixelpartStaticPropertyFloat MotionPathForce { get; }

        public bool Visible
        {
            get => Plugin.PixelpartParticleTypeIsVisible(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetVisible(effectRuntime, Id, value);
        }

        public int Layer
        {
            get => Plugin.PixelpartParticleTypeGetLayer(effectRuntime, Id);
            set => Plugin.PixelpartParticleTypeSetLayer(effectRuntime, Id, value);
        }

        public PixelpartAnimatedPropertyFloat Count { get; }

        public PixelpartAnimatedPropertyFloat Lifespan { get; }

        public PixelpartAnimatedPropertyFloat InitialSize { get; }

        public PixelpartAnimatedPropertyFloat3 InitialRotation { get; }

        public PixelpartAnimatedPropertyFloat InitialVelocity { get; }

        public PixelpartAnimatedPropertyFloat InheritedVelocity { get; }

        public PixelpartAnimatedPropertyFloat InitialOpacity { get; }

        public PixelpartAnimatedPropertyFloat3 Position { get; }

        public PixelpartAnimatedPropertyFloat3 Size { get; }

        public PixelpartAnimatedPropertyFloat3 Stretch { get; }

        public PixelpartAnimatedPropertyFloat PhysicalSize { get; }

        public PixelpartAnimatedPropertyFloat3 Rotation { get; }

        public PixelpartAnimatedPropertyFloat3 RotationBySpeed { get; }

        public PixelpartAnimatedPropertyFloat Acceleration { get; }

        public PixelpartAnimatedPropertyFloat RadialAcceleration { get; }

        public PixelpartAnimatedPropertyFloat Weight { get; }

        public PixelpartAnimatedPropertyFloat Bounce { get; }

        public PixelpartAnimatedPropertyFloat Friction { get; }

        public PixelpartAnimatedPropertyFloat4 Color { get; }

        public PixelpartAnimatedPropertyFloat Opacity { get; }

        public PixelpartStaticPropertyFloat LifespanVariance { get; }

        public PixelpartStaticPropertyFloat SizeVariance { get; }

        public PixelpartStaticPropertyFloat3 RotationVariance { get; }

        public PixelpartStaticPropertyFloat3 AngularVelocityVariance { get; }

        public PixelpartStaticPropertyFloat VelocityVariance { get; }

        public PixelpartStaticPropertyFloat4 ColorVariance { get; }

        public PixelpartStaticPropertyFloat OpacityVariance { get; }

        [Obsolete("deprecated, use Count")]
        public PixelpartAnimatedPropertyFloat NumParticles => Count;

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