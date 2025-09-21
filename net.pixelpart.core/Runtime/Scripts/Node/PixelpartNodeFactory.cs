using System;

namespace Pixelpart
{
    /// <summary>
    /// Factory for creating a <see cref="PixelpartNode"/>.
    /// </summary>
    public static class PixelpartNodeFactory
    {
        /// <summary>
        /// Create a node object from its ID in the effect.
        /// </summary>
        /// <param name="effectRuntime">Effect runtime</param>
        /// <param name="nodeId">Node ID</param>
        /// <returns>Created node</returns>
        public static PixelpartNode CreateNode(IntPtr effectRuntime, uint nodeId)
        {
            var nodeTypeIndex = Plugin.PixelpartNodeGetType(effectRuntime, nodeId);
            if (nodeTypeIndex < 0)
            {
                return null;
            }

            switch ((PixelpartNodeType)nodeTypeIndex)
            {
                case PixelpartNodeType.GroupNode:
                    return new PixelpartGroupNode(effectRuntime, nodeId);
                case PixelpartNodeType.ParticleEmitter:
                    return new PixelpartParticleEmitter(effectRuntime, nodeId);
                case PixelpartNodeType.AttractionField:
                    return new PixelpartAttractionField(effectRuntime, nodeId);
                case PixelpartNodeType.AccelerationField:
                    return new PixelpartAccelerationField(effectRuntime, nodeId);
                case PixelpartNodeType.VectorField:
                    return new PixelpartVectorField(effectRuntime, nodeId);
                case PixelpartNodeType.NoiseField:
                    return new PixelpartNoiseField(effectRuntime, nodeId);
                case PixelpartNodeType.DragField:
                    return new PixelpartDragField(effectRuntime, nodeId);
                case PixelpartNodeType.LineCollider:
                    return new PixelpartLineCollider(effectRuntime, nodeId);
                case PixelpartNodeType.PlaneCollider:
                    return new PixelpartPlaneCollider(effectRuntime, nodeId);
                case PixelpartNodeType.DirectionalLightSource:
                    return new PixelpartDirectionalLightSource(effectRuntime, nodeId);
                case PixelpartNodeType.PointLightSource:
                    return new PixelpartPointLightSource(effectRuntime, nodeId);
                case PixelpartNodeType.SpotLightSource:
                    return new PixelpartSpotLightSource(effectRuntime, nodeId);
                default:
                    return null;
            }
        }
    }
}
