Shader "Custom/TestShader"
{
    Properties
    {

    }

    SubShader
    {
        Pass
        {
            Tags
            {
                "RenderType" = "Opaque"
            }

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            #pragma target 4.5

            struct Particle
            {
                float3 position;
                float3 globalPosition;
                float3 velocity;
                float3 force;
                float life;
            };

            struct ps_input
            {
                float4 position : SV_POSITION;
            };

            StructuredBuffer<Particle> particleBuffer;

            ps_input vert(uint vertexId : SV_VertexID, const uint instanceId : SV_InstanceID)
            {
                ps_input o = (ps_input)0;
                o.position = UnityObjectToClipPos(float4(particleBuffer[instanceId].globalPosition, 1.0f));

                return o;
            }

            float4 frag(const ps_input i) : COLOR
            {
                return fixed4(1.0, 1.0, 1.0, 1.0);
            }

            ENDCG
        }
    }

    FallBack Off
}
