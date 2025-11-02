# Scripting

A Pixelpart effect played with a @ref Pixelpart.PixelpartEffect component in your scene can be modified at runtime with a script. This can be used to dynamically make an effect react to whatever is happening in your scene.

## Effect inputs

Effect inputs are the primary technique to dynamically modify effect properties. The following example shows how the value of an effect input, which has been defined in the Pixelpart editor, can be changed from a *C#* script:

~~~csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Set effect input "Size" to 2.0
effect.SetInputFloat("Size", 2.0f);
~~~

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

Triggers are used to make effects react to events in your game. The following example shows how a trigger can be activated from a *C#* script using the @ref Pixelpart.PixelpartEffect.ActivateTrigger method:

~~~csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Activate the trigger "Event"
effect.ActivateTrigger("Event");
~~~

Use @ref Pixelpart.PixelpartEffect.IsTriggerActivated to check if a trigger has already been activated.
