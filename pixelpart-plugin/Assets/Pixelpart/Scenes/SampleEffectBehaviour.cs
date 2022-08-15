using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SampleEffectBehaviour : MonoBehaviour {
	void Update() {
		pixelpart.PixelpartEffect effect = gameObject.GetComponent<pixelpart.PixelpartEffect>();

		if(effect != null) {
			pixelpart.PixelpartParticleEmitter emitter = effect.GetParticleEmitterByIndex(0);

			if(emitter != null) {
				emitter.GetParticleOpacity().Clear();
				emitter.GetParticleOpacity().AddPoint(0.0f, 1.0f);
				emitter.GetParticleOpacity().AddPoint(1.0f, 0.0f);
			}
		}
	}
}
