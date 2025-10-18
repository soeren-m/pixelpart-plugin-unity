using System;
using Pixelpart;
using UnityEngine;

public class SampleBehavior : MonoBehaviour
{
    private PixelpartEffect effect;

    public void Awake()
    {
        effect = GetComponent<PixelpartEffect>();

        // Print information about particle types
        var particleType = effect.GetParticleTypeAtIndex(0);
        var particleTypeIndex = 0;
        while (particleType != null)
        {
            PrintParticleTypeInfo(particleType);

            particleTypeIndex++;
            particleType = effect.GetParticleTypeAtIndex(particleTypeIndex);
        }

        // Print information about effect nodes
        var node = effect.GetNodeAtIndex(0);
        var nodeIndex = 0;
        while (node != null)
        {
            PrintNodeInfo(node);

            nodeIndex++;
            node = effect.GetNodeAtIndex(nodeIndex);
        }
    }

    public void OnEnable()
    {
        effect.Finished += OnEffectFinished;
    }

    public void OnDisable()
    {
        effect.Finished -= OnEffectFinished;
    }

    private void OnEffectFinished(object sender, EventArgs e)
    {
        Debug.Log("Effect finished");
    }

    private static void PrintParticleTypeInfo(PixelpartParticleType particleType)
    {
        Debug.Log("Particle type \"" + particleType.Name + "\":" +
            " Id=" + particleType.Id +
            " ParentId=" + particleType.ParentId);
    }

    private static void PrintNodeInfo(PixelpartNode node)
    {
        if (node == null)
        {
            return;
        }

        var typeString = "Node";
        if (node is PixelpartGroupNode) typeString = "GroupNode";
        else if (node is PixelpartParticleEmitter) typeString = "ParticleEmitter";
        else if (node is PixelpartAttractionField) typeString = "AttractionField";
        else if (node is PixelpartAccelerationField) typeString = "AccelerationField";
        else if (node is PixelpartVectorField) typeString = "VectorField";
        else if (node is PixelpartNoiseField) typeString = "NoiseField";
        else if (node is PixelpartDragField) typeString = "DragField";
        else if (node is PixelpartLineCollider) typeString = "LineCollider";
        else if (node is PixelpartPlaneCollider) typeString = "PlaneCollider";
        else if (node is PixelpartDirectionalLightSource) typeString = "DirectionalLightSource";
        else if (node is PixelpartPointLightSource) typeString = "PointLightSource";
        else if (node is PixelpartSpotLightSource) typeString = "SpotLightSource";

        Debug.Log("Node \"" + node.Name + "\":" +
            " Id=" + node.Id +
            " ParentId=" + node.ParentId +
            " Type=" + typeString);
    }
}
