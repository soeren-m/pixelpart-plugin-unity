Shader "Pixelpart/Default"
{
	Properties
	{
		[PerRendererData] _MainTex ("Sprite Texture", 2D) = "white" {}
		_Color ("Color", Color) = (1,1,1,1)
		[MaterialToggle] PixelSnap ("Pixel Snap", Float) = 0
		_BlendOp ("Blend Operation", Float) = 0
		_SrcBlendMode ("Src Blend Mode", Float) = 0
		_DstBlendMode ("Dst Blend Mode", Float) = 0
		_ColorMode ("Color Mode", Int) = 0
		_AlphaThreshold ("Alpha Threshold", Float) = 0
	}

	SubShader
	{
		Tags
		{ 
			"Queue"="Transparent" 
			"IgnoreProjector"="True" 
			"RenderType"="Transparent" 
			"PreviewType"="Plane"
			"CanUseSpriteAtlas"="True"
		}

		Cull Off
		Lighting Off
		ZWrite Off
		BlendOp [_BlendOp]
		Blend [_SrcBlendMode] [_DstBlendMode]

		Pass
		{
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile _ PIXELSNAP_ON
			#include "UnityCG.cginc"
			
			struct appdata_t
			{
				float4 vertex   : POSITION;
				float4 color    : COLOR;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f
			{
				float4 vertex   : SV_POSITION;
				fixed4 color    : COLOR;
				float2 texcoord  : TEXCOORD0;
			};
			
			fixed4 _Color;

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.texcoord = IN.texcoord;
				OUT.color = IN.color * _Color;
				#ifdef PIXELSNAP_ON
				OUT.vertex = UnityPixelSnap (OUT.vertex);
				#endif

				return OUT;
			}

			sampler2D _MainTex;
			sampler2D _AlphaTex;
			float _AlphaSplitEnabled;
			float _AlphaThreshold;
			int _ColorMode;

			fixed4 SampleSpriteTexture (float2 uv)
			{
				fixed4 color = tex2D (_MainTex, uv);

#if UNITY_TEXTURE_ALPHASPLIT_ALLOWED
				if (_AlphaSplitEnabled)
					color.a = tex2D (_AlphaTex, uv).r;
#endif //UNITY_TEXTURE_ALPHASPLIT_ALLOWED

				return color;
			}

			fixed4 Blend(fixed4 color1, fixed4 color2)
			{
				fixed4 outColor = fixed4(1, 1, 1, 1);

				if(_ColorMode == 0) {
					outColor = color1 * color2;
				}
				else if(_ColorMode == 1) {
					outColor.rgb = clamp(color1.rgb + color2.rgb, fixed3(0, 0, 0), fixed3(1, 1, 1));
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 2) {
					outColor.rgb = clamp(color1.rgb - color2.rgb, fixed3(0, 0, 0), fixed3(1, 1, 1));
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 3) {
					outColor.rgb = abs(color1.rgb - color2.rgb);
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 4) {
					outColor.rgb = fixed3(1, 1, 1) - (fixed3(1, 1, 1) - color1.rgb) * (fixed3(1, 1, 1) - color2.rgb);
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 5) {
					outColor.r = (color1.r < 0.5) ? (2.0 * color1.r * color2.r) : (1.0 - 2.0 * (1.0 - color1.r) * (1.0 - color2.r));
					outColor.g = (color1.g < 0.5) ? (2.0 * color1.g * color2.g) : (1.0 - 2.0 * (1.0 - color1.g) * (1.0 - color2.g));
					outColor.b = (color1.b < 0.5) ? (2.0 * color1.b * color2.b) : (1.0 - 2.0 * (1.0 - color1.b) * (1.0 - color2.b));
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 6) {
					outColor.rgb = max(color1.rgb, color2.rgb);
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 7) {
					outColor.rgb = min(color1.rgb, color2.rgb);
					outColor.a = color1.a * color2.a;
				}
				else if(_ColorMode == 8) {
					outColor.rgb = color2.rgb;
					outColor.a = color1.a;
				}

				return outColor;
			}

			fixed4 frag(v2f IN) : SV_Target
			{
				fixed4 textureColor = SampleSpriteTexture (IN.texcoord);
				fixed4 finalColor = Blend (textureColor, IN.color);
				if(finalColor.a <= _AlphaThreshold) {
					discard;
				}

				finalColor.rgb *= finalColor.a;

				return finalColor;
			}
		ENDCG
		}
	}
}