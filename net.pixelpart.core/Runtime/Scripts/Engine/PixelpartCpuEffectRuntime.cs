namespace Pixelpart
{
    internal class PixelpartCpuEffectRuntime : PixelpartEffectRuntime
    {
        private readonly IntPtr effectRuntimePtr;

        public PixelpartCpuEffectRuntime(IntPtr runtimePtr)
        {
            effectRuntimePtr = runtimePtr;
        }

        public void Advance(float dt,
            bool loop, float loopTime,
            float speed, float timeStep,
            uint seed, bool randomSeed)
        {
            PixelpartPlugin.PixelpartAdvanceEffect(effectRuntime, dt,
                loop, loopTime,
                speed, timeStep,
                seed, randomSeed);
        }

        public void Restart(bool clearParticles)
        {
            PixelpartPlugin.PixelpartRestartEffect(effectRuntimePtr, clearParticles);
        }

        public void SpawnParticles(uint particleEmitterId, uint particleTypeId, int count)
        {
            PixelpartPlugin.PixelpartSpawnParticles(effectRuntimePtr, particleEmitterId, particleTypeId, count);
        }

        public bool IsFinished()
        {
            return PixelpartPlugin.PixelpartIsEffectFinished(effectRuntimePtr);
        }
    }
}
