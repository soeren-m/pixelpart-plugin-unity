Shader "Pixelpart/PixelpartMeshLitAlpha"
{
	Properties
	{
		_MainTexture ("Main Texture", 2D) = "white" {}

		_Emission ("Emission", Vector) = (0, 0, 0, 0)
		_Roughness ("Roughness", Float) = 0.5
		_Metallic ("Metallic", Float) = 0.0

		_ColorBlendMode ("Color Blend Mode", Int) = 0
	}

	SubShader
	{
		Tags
		{
			"RenderType" = "Transparent"
			"Queue" = "Transparent"
		}

		CGPROGRAM
		#pragma surface surf Standard alpha:blend vertex:vert
		#pragma multi_compile_instancing

		#include "PixelpartCommon.cginc"

		struct Input
		{
			float2 uv_MainTexture;
			float4 projectedPosition;
		};

		void vert(inout appdata_full IN, out Input OUT)
		{
			UNITY_INITIALIZE_OUTPUT(Input, OUT);

			OUT.projectedPosition = ComputeScreenPos(UnityObjectToClipPos(IN.vertex));
			OUT.projectedPosition.z = -UnityObjectToViewPos(IN.vertex).z;
		}

		UNITY_DECLARE_DEPTH_TEXTURE(_CameraDepthTexture);
		sampler2D _MainTexture;

		float3 _Emission;
		float _Roughness;
		float _Metallic;

		int _ColorBlendMode;

		bool _SoftParticles;
		float _SoftParticleTransition;
		bool _DistanceFade;
		float _DistanceFadeTransition;

		UNITY_INSTANCING_BUFFER_START(Props)
			UNITY_DEFINE_INSTANCED_PROP(fixed4, _Color)
		UNITY_INSTANCING_BUFFER_END(Props)

		void surf(Input IN, inout SurfaceOutputStandard OUT)
		{
			fixed4 vertexColor = UNITY_ACCESS_INSTANCED_PROP(Props, _Color);
			fixed4 color = tex2D(_MainTexture, IN.uv_MainTexture);
			color = Pixelpart_Blend(color, vertexColor, _ColorBlendMode);

			float softParticleBlendFactor = _SoftParticles
				? Pixelpart_SoftParticleFade(_CameraDepthTexture, IN.projectedPosition, _SoftParticleTransition)
				: 1.0;
			color.a *= softParticleBlendFactor;

			float distanceBlendFactor = _DistanceFade
				? Pixelpart_DistanceFade(IN.projectedPosition.z, _DistanceFadeTransition)
				: 1.0;
			color.a *= distanceBlendFactor;

			OUT.Albedo = color.rgb;
			OUT.Alpha = color.a;
			OUT.Emission = _Emission;
			OUT.Smoothness = 1.0 - _Roughness;
			OUT.Metallic = _Metallic;
		}
		ENDCG
	}
}