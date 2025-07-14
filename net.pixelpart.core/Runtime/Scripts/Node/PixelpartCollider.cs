using System;

namespace Pixelpart
{
    /// <summary>
    /// Node particles collide with.
    /// </summary>
    /// <remarks>
    /// When hitting a collider, particles bounce off its surface.
    /// </remarks>
    public class PixelpartCollider : PixelpartNode
    {
        /// <summary>
        /// Whether to kill particles on contact with the collider.
        /// </summary>
        public PixelpartStaticPropertyBool KillOnContact { get; }

        /// <summary>
        /// How strongly particles bounce back from the collider.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Bounce { get; }

        /// <summary>
        /// How much particles are slowed down when sliding down the collider.
        /// </summary>
        public PixelpartAnimatedPropertyFloat Friction { get; }

        /// <summary>
        /// Construct <see cref="PixelpartCollider"/>.
        /// </summary>
        /// <param name="effectRuntimePtr">Effect runtime</param>
        /// <param name="id">Node ID</param>
        public PixelpartCollider(IntPtr effectRuntimePtr, uint id) : base(effectRuntimePtr, id)
        {
            KillOnContact = new PixelpartStaticPropertyBool(
                Plugin.PixelpartColliderGetKillOnContact(effectRuntimePtr, id));
            Bounce = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartColliderGetBounce(effectRuntimePtr, id));
            Friction = new PixelpartAnimatedPropertyFloat(
                Plugin.PixelpartColliderGetFriction(effectRuntimePtr, id));
        }
    }
}