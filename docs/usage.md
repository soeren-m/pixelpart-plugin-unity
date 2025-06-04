# Usage

## Importing effects

Save the effect you created in *Pixelpart* as a *.ppfx* file and place it into the *Assets* folder of your Unity project. The plugin automatically imports these *.ppfx* files as Pixelpart effect assets.

## Playing effects

To play an effect, attach the *Pixelpart/Effect* component to a game object in your scene and move the imported effect asset onto the *Effect Asset* field in the component's inspector window. The selected effect is rendered when running the game. If the effect *does not* appear, make sure you have set an appropriate *Effect Scale* value in the node's inspector.

![Effect inspector](./images/play.png)

You can adjust these properties in the inspector:

Property | Description
-------- | -----------
**Effect Asset** | The effect to be played.
**Playing** | Whether is the effect is playing or paused.
**Loop** | If enabled, the effect is repeated after the time specified in **Loop Time**.
**Loop Time** | Time in seconds after which the effect is repeated. Only has an effect if **Loop** is enabled.
**Warmup Time** | Time in seconds the effect is pre-simulated before being rendered. This value impacts performance and should be kept as low as possible.
**Speed** | Multiplier for the playback speed of the effect. For example, setting **Speed** to *0.5* shows the effect in slow motion.
**Frame Rate** | How many iterations are simulated per second. Can be used to improve performance for complex effects.
**Inputs** | Effect input values.
**Effect Scale** | Multiplier for the size of the effect. Adjust this value if the effect appears too small or too large in the scene.
**Flip H** | Whether the effect is flipped horizontally.
**Flip V** | Whether the effect is flipped vertically.
