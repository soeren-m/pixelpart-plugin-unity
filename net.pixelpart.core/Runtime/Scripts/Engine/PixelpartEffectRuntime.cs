namespace Pixelpart
{
    internal interface PixelpartEffectRuntime
    {
        void Advance(float dt,
            bool loop, float loopTime,
            float speed, float timeStep,
            uint seed, bool randomSeed);

        void Restart(bool clearParticles);

        void SpawnParticles(uint particleEmitterId, uint particleTypeId, int count);

        // TODO: inputs
        // TODO: triggers
        // TODO: nodes
        // TODO: particle types

        bool IsFinished();
    }
}
