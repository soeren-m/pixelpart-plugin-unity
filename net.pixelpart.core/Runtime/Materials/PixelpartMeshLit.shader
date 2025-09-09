Shader "Pixelpart/PixelpartMeshLit"
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
			"RenderType" = "Opaque"
		}

		CGPROGRAM
		#pragma surface surf Standard
		#pragma multi_compile_instancing

		#include "PixelpartCommon.cginc"

		struct Input
		{
			float2 uv_MainTexture;
		};

		sampler2D _MainTexture;

		float3 _Emission;
		float _Roughness;
		float _Metallic;

		int _ColorBlendMode;

		UNITY_INSTANCING_BUFFER_START(Props)
			UNITY_DEFINE_INSTANCED_PROP(fixed4, _Color)
		UNITY_INSTANCING_BUFFER_END(Props)

		void surf(Input IN, inout SurfaceOutputStandard OUT)
		{
			fixed4 vertexColor = UNITY_ACCESS_INSTANCED_PROP(Props, _Color);
			fixed4 color = tex2D(_MainTexture, IN.uv_MainTexture);
			color = Pixelpart_Blend(color, vertexColor, _ColorBlendMode);

			OUT.Albedo = color.rgb;
			OUT.Emission = _Emission;
			OUT.Smoothness = 1.0 - _Roughness;
			OUT.Metallic = _Metallic;
		}
		ENDCG
	}
}
