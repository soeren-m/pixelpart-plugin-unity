namespace Pixelpart
{
    internal interface PixelpartEffectRuntime
    {
        void Advance(float dt, bool loop, float loopTime, float speed, float timeStep, uint seed, bool randomSeed);
    }
}
