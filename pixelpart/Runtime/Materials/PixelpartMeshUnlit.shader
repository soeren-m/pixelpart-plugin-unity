Shader "Pixelpart/PixelpartMeshUnlit"
{
	Properties
	{
		_MainTexture ("Main Texture", 2D) = "white" {}

		_Emission ("Emission", Vector) = (0, 0, 0, 0)

		_ColorBlendMode ("Color Blend Mode", Int) = 0
	}

	SubShader
	{
		Tags
		{
			"RenderType" = "Opaque"
		}

		Lighting Off
		Cull Back
		ZWrite On
		Blend Off

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_instancing

			#include "PixelpartCommon.cginc"

			struct appdata_t
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			UNITY_INSTANCING_BUFFER_START(Props)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
			UNITY_INSTANCING_BUFFER_END(Props)

			v2f vert(appdata_t IN)
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			sampler2D _MainTexture;

			float3 _Emission;

			int _ColorBlendMode;

			fixed4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				fixed4 vertexColor = UNITY_ACCESS_INSTANCED_PROP(Props, _Color);
				fixed4 color = tex2D(_MainTexture, IN.uv);
				color = Pixelpart_Blend(color, vertexColor, _ColorBlendMode);
				color.rgb += _Emission;

				return color;
			}
			ENDCG
		}
	}
}