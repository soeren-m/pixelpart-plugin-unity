using System;
using UnityEngine;

namespace Pixelpart
{
    internal interface PixelpartParticleMesh
    {
        public void Update(Camera camera, Transform transform, Vector3 scale);

        public void Render(Camera camera, Transform transform, int layer);
    }
}
