using System;

namespace Pixelpart
{
    public class PixelpartCollider : PixelpartNode
    {
        public PixelpartStaticPropertyBool KillOnContact { get; }

        public PixelpartAnimatedPropertyFloat Bounce { get; }

        public PixelpartAnimatedPropertyFloat Friction { get; }

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