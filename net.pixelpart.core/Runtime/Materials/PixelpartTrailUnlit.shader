Shader "Pixelpart/PixelpartTrailUnlit"
{
	Properties
	{
		[Enum(Off,0,On,1)] _ZWrite ("Z Write", Float) = 0
		[Enum(UnityEngine.Rendering.BlendOp)] _BlendOp ("Blend Operation", Float) = 0
		[Enum(UnityEngine.Rendering.BlendMode)] _SrcBlendMode ("Src Blend Mode", Float) = 0
		[Enum(UnityEngine.Rendering.BlendMode)] _DstBlendMode ("Dst Blend Mode", Float) = 0

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
			"IgnoreProjector" = "True"
			"PreviewType" = "Plane"
		}

		Lighting Off
		Cull Off
		ZWrite [_ZWrite]
		BlendOp [_BlendOp]
		Blend [_SrcBlendMode] [_DstBlendMode]

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "PixelpartCommon.cginc"

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 additional : TEXCOORD2;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				float2 uv : TEXCOORD0;
				float4 projectedPosition : TEXCOORD1;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.color = IN.color;
				OUT.uv = IN.uv;
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -UnityObjectToViewPos(IN.vertex).z;

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
				fixed4 color = tex2D(_MainTexture, IN.uv);
				color = Pixelpart_Blend(color, IN.color, _ColorBlendMode);
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