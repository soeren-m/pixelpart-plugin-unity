using System;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

namespace Pixelpart {
public interface PixelpartParticleRenderer {
	void Draw(Camera camera, Transform transform, Vector3 scale, int layer);
}
}