using UnityEngine;

namespace Pixelpart
{
    internal interface IPixelpartParticleRenderer
    {
        void Render(Camera camera, Transform transform, Vector3 effectScale, int layer);
    }
}
