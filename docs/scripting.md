# Scripting

A Pixelpart effect played with a *PixelpartEffect* component in your scene can be modified at runtime with a script. This can be used to dynamically make an effect react on whatever is happening in your scene.

## Effect inputs

The following example shows how an effect input can be set with *C#*:

~~~csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Set effect input "Size" to 2.0
effect.SetInputFloat("Size", 2.0f);
~~~

## Triggers

The following example shows how a trigger can be activated with *C#*:

~~~csharp
// Get effect component of game object
var effect = GetComponent<PixelpartEffect>();

// Activate the trigger "Event"
effect.ActivateTrigger("Event");
~~~
