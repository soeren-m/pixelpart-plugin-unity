# Scripting

A Pixelpart effect that is rendered using a @ref Pixelpart.PixelpartEffect component in your scene can be controlled and modified at runtime with scripts. This can be used to dynamically change the effect and let it react to other objects in the scene.

## General

The @ref Pixelpart.PixelpartEffect component provides methods to control the effect simulation. You can also change the properties visible in the inspector from a script. The following example shows how to restart an effect and then simulate it in slow motion:

```csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Restart effect and play in slow-motion
effect.RestartEffect();
effect.Playing = true;
effect.Speed = 0.5;
```

- @ref Pixelpart.PixelpartEffect.RestartEffect - Restart the effect from the beginning.
- @ref Pixelpart.PixelpartEffect.Playing - Whether is the effect is playing or paused.
- @ref Pixelpart.PixelpartEffect.Loop - If enabled, the effect is repeated after the time specified in `LoopTime`.
- @ref Pixelpart.PixelpartEffect.LoopTime - Time in seconds after which the effect is repeated. Only has an effect if `Loop` is enabled.
- @ref Pixelpart.PixelpartEffect.WarmupTime - Time in seconds the effect is pre-simulated before being rendered. This value impacts performance and should be kept as low as possible.
- @ref Pixelpart.PixelpartEffect.Speed - Multiplier for the playback speed of the effect. For example, setting `Speed` to *0.5* shows the effect in slow motion.
- @ref Pixelpart.PixelpartEffect.FrameRate - How many iterations are simulated per second. Can be used to improve performance for complex effects.
- @ref Pixelpart.PixelpartEffect.Seed - Seed used to initialize the effect simulation. This seed is used if `RandomSeed` is not enabled.
- @ref Pixelpart.PixelpartEffect.RandomSeed - Whether to use a random seed to initialize the effect simulation.
- @ref Pixelpart.PixelpartEffect.EffectScale - Multiplier for the size of the effect. Adjust this value if the effect appears too small or too large in the scene.
- @ref Pixelpart.PixelpartEffect.FlipH - Whether the effect is flipped horizontally.
- @ref Pixelpart.PixelpartEffect.FlipV - Whether the effect is flipped vertically.

You might also want to get notified when an effect is finished and no particles are visible anymore. You can do so by subscribing to the @ref Pixelpart.PixelpartEffect.Finished event. Of course this only works for effects without repeating emitters.

```csharp
public void Awake()
{
    // Get effect component of game object
    var effect = GetComponent<PixelpartEffect>();

    // Get notified when effect is finished
    effect.Finished += OnFinished;
}

private void OnFinished(object sender, EventArgs e)
{
    // Effect is now finished
}
```

## Inputs

Effect inputs are the primary technique to dynamically modify effect properties. The following example shows how the value of an effect input, which has been defined in the Pixelpart editor, can be changed from a script:

```csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Set effect input "Size" to 2.0
effect.SetInputFloat("Size", 2.0f);
```

Depending on the data type, several *set* methods are available to change the value of effect inputs:

- @ref Pixelpart.PixelpartEffect.SetInputBool
- @ref Pixelpart.PixelpartEffect.SetInputInt
- @ref Pixelpart.PixelpartEffect.SetInputFloat
- @ref Pixelpart.PixelpartEffect.SetInputFloat2
- @ref Pixelpart.PixelpartEffect.SetInputFloat3
- @ref Pixelpart.PixelpartEffect.SetInputFloat4

To retrieve the currently set input value, use the corresponding *get* methods:

- @ref Pixelpart.PixelpartEffect.GetInputBool
- @ref Pixelpart.PixelpartEffect.GetInputInt
- @ref Pixelpart.PixelpartEffect.GetInputFloat
- @ref Pixelpart.PixelpartEffect.GetInputFloat2
- @ref Pixelpart.PixelpartEffect.GetInputFloat3
- @ref Pixelpart.PixelpartEffect.GetInputFloat4

## Triggers

Triggers are used to make effects react to events in your game. The following example shows how a trigger can be activated from a script using the @ref Pixelpart.PixelpartEffect.ActivateTrigger method:

```csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Activate the trigger "MyTrigger"
effect.ActivateTrigger("MyTrigger");
```

Use @ref Pixelpart.PixelpartEffect.IsTriggerActivated to check if a trigger has already been activated.

## Events

In contrast to triggers, events are used to make the game react to something happening in the effect, for example to play an audio clip when an emitter starts producing particles. You can subscribe to @ref Pixelpart.PixelpartEffect.EffectEvent to get notified when effect events occur:

```csharp
public void Awake()
{
    // Get effect component of game object
    var effect = GetComponent<PixelpartEffect>();

    // Get notified about effect events
    effect.EffectEvent += OnEffectEvent;
}

private void OnEffectEvent(object sender, PixelpartEffectEventArgs e)
{
    if (e.EventName == "MyEvent")
    {
        // Do something, e.g. play an audio clip
    }
}
```
