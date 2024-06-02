Shader "Pixelpart/PixelpartMeshUnlitAlpha"
{
	Properties
	{
		_MainTexture ("Main Texture", 2D) = "white" {}

		_Emission ("Emission", Vector) = (0, 0, 0, 0)

		_ColorBlendMode ("Color Blend Mode", Int) = 0

		_SoftParticles ("Soft Particles", Int) = 0
		_SoftParticleTransition ("Soft Particle Transition", Float) = 0.0
		_DistanceFade ("Distance Fade", Int) = 0
		_DistanceFadeTransition ("Distance Fade Transition", Float) = 0.0
	}

	SubShader
	{
		Tags
		{
			"RenderType" = "Transparent"
			"Queue" = "Transparent"
		}

		Lighting Off
		Cull Back
		ZWrite Off
		BlendOp Add
		Blend SrcAlpha OneMinusSrcAlpha

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
				float4 projectedPosition : TEXCOORD1;
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
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -UnityObjectToViewPos(IN.vertex).z;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			UNITY_DECLARE_DEPTH_TEXTURE(_CameraDepthTexture);
			sampler2D _MainTexture;

			float3 _Emission;

			int _ColorBlendMode;

			bool _SoftParticles;
			float _SoftParticleTransition;
			bool _DistanceFade;
			float _DistanceFadeTransition;

			fixed4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				fixed4 vertexColor = UNITY_ACCESS_INSTANCED_PROP(Props, _Color);
				fixed4 color = tex2D(_MainTexture, IN.uv);
				color = Pixelpart_Blend(color, vertexColor, _ColorBlendMode);
				color.rgb += _Emission;

				float softParticleBlendFactor = _SoftParticles
					? Pixelpart_SoftParticleFade(_CameraDepthTexture, IN.projectedPosition, _SoftParticleTransition)
					: 1.0;
				color.a *= softParticleBlendFactor;

				float distanceBlendFactor = _DistanceFade
					? Pixelpart_DistanceFade(IN.projectedPosition.z, _DistanceFadeTransition)
					: 1.0;
				color.a *= distanceBlendFactor;

				return color;
			}
			ENDCG
		}
	}
}