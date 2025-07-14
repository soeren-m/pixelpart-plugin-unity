namespace Pixelpart
{
    /// <summary>
    /// Types of nodes in a Pixelpart effect.
    /// </summary>
    public enum PixelpartNodeType : int
    {
        GroupNode = 0,
        ParticleEmitter = 1,
        AttractionField = 2,
        AccelerationField = 3,
        VectorField = 4,
        NoiseField = 5,
        DragField = 6,
        LineCollider = 7,
        PlaneCollider = 8,
        DirectionalLightSource = 9,
        PointLightSource = 10,
        SpotLightSource = 11
    }
}