using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Pixelpart;

public class SampleEffectBehaviour : MonoBehaviour {
	public void Start() {
		// Get effect component
		PixelpartEffect effect = gameObject.GetComponent<PixelpartEffect>();
		if(effect == null) {
			return;
		}

		// Get particle type by name
		PixelpartParticleType particleType = effect.FindParticleType("Particle");
		if(particleType == null) {
			return;
		}

		// Get particle emitter by name
		PixelpartParticleEmitter particleEmitter = effect.FindParticleEmitter("Emitter");
		if(particleEmitter == null) {
			return;
		}

		// Set particle property
		particleType.VelocityVariance = 0.2f;

		// Define animated property
		particleType.Opacity.Clear();
		particleType.Opacity.AddPoint(0.0f, 1.0f);
		particleType.Opacity.AddPoint(1.0f, 0.0f);

		// Set animated property to constant value
		particleEmitter.Spread.Set(360.0f);

		// Spawn some extra particles
		particleType.SpawnParticles(200);
	}
}
