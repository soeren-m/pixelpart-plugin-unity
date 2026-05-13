# Scripting

A Pixelpart effect that is rendered using a @ref Pixelpart.PixelpartEffect component in your scene can be modified at runtime with scripts. This can be used to dynamically change the effect and let it react to other objects in the scene.

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

// Activate the trigger "Event"
effect.ActivateTrigger("Event");
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
