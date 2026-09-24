namespace Pixelpart
{
    internal class PixelpartGpuEffectRuntime : PixelpartEffectRuntime
    {
        struct GpuParticle
        {
            public Vector3 Position;
            public Vector3 GlobalPosition;
            public Vector3 Velocity;
            public Vector3 Force;
            public float Life;
        }

        class GpuBufferCollection
        {
            public ComputeBuffer ParticleBuffer { get; set; }
        }

        private const MaxParticleCount = 1000;

        private static ComputeShader generationComputeShader;

        private static ComputeShader simulationComputeShader;

        private static int generationKernelId = 0;

        private static int simulationKernelId = 0;

        private static uint simulationKernelGroupSize = 0;

        private readonly PixelpartParticleEmissionPair[] particleEmissionPairs;

        private readonly GpuBufferCollection[] gpuBuffers;

        public PixelpartGpuEffectRuntime(IntPtr effectResourcePtr)
        {
            var emissionPairCount = PixelpartPlugin.PixelpartGetEffectParticleEmissionPairCount(effectResourcePtr);

            particleEmissionPairs = new PixelpartParticleEmissionPair[emissionPairCount];  
            PixelpartPlugin.PixelpartGetEffectParticleEmissionPairs(effectResourcePtr, particleEmissionPairs);

            gpuBuffers = new GpuBufferCollection[emissionPairCount];

            for (var emissionPairIndex = 0; emissionPairIndex < emissionPairCount; emissionPairIndex++)
            {
                gpuBuffers[emissionPairIndex] = new GpuBufferCollection
                {
                    ParticleBuffer = new ComputeBuffer(MaxParticleCount, sizeof(GpuParticle))
                };
            }

            // TODO: init with some dummy data
        }

        public void Advance(float dt, bool loop, float loopTime, float speed, float timeStep, uint seed, bool randomSeed)
        {
            simulationComputeShader.SetFloat("deltaTime", Time.deltaTime);

            for (var emissionPairIndex = 0; emissionPairIndex < emissionPairCount; emissionPairIndex++)
            {
                var emissionPair = particleEmissionPairs[emissionPairIndex];
                var gpuBufferCollection = gpuBuffers[emissionPairIndex];

                simulationComputeShader.SetBuffer(simulationKernelId, "particleBuffer", gpuBufferCollection.ParticleBuffer);

                simulationComputeShader.Dispatch(simulationKernelId,
                    Mathf.CeilToInt((float)MaxParticleCount / (float)simulationKernelGroupSize), 1, 1);
            }
        }

        public ComputeBuffer GetParticleBuffer(uint emitterId, uint typeId)
        {
            // TODO
            return gpuBuffers[0].ParticleBuffer;
        }

        public int GetParticleCount(uint emitterId, uint typeId)
        {
            // TODO
            return MaxParticleCount;
        }

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterAssembliesLoaded)]
        private static void LoadComputeShaders()
        {
            generationComputeShader = Resources.Load<ComputeShader>("GenerationComputeShader");
            simulationComputeShader = Resources.Load<ComputeShader>("SimulationComputeShader");

            generationKernelId = generationComputeShader.FindKernel("PixelpartGeneration");
            simulationKernelId = simulationComputeShader.FindKernel("PixelpartSimulation");

            simulationComputeShader.GetKernelThreadGroupSizes(simulationKernelId, out simulationKernelGroupSize, out _, out _);
        }
    }
}
