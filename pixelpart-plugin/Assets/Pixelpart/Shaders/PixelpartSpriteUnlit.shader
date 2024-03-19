Shader "Pixelpart/PixelpartSpriteUnlit"
{
	Properties
	{
		_BlendOp ("Blend Operation", Float) = 0
		_SrcBlendMode ("Src Blend Mode", Float) = 0
		_DstBlendMode ("Dst Blend Mode", Float) = 0
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
		Blend [_SrcBlendMode] [_DstBlendMode] // TODO

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 force : TEXCOORD2;
				float4 additional : TEXCOORD3;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				float2 uv : TEXCOORD0;
				float3 velocity : TEXCOORD1;
				float3 force : TEXCOORD2;
				float life : TEXCOORD3;
				float id : TEXCOORD4;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;
				OUT.color = IN.color;
				OUT.velocity = IN.velocity.xyz;
				OUT.force = IN.force.xyz;
				OUT.life = IN.additional.x;
				OUT.id = IN.additional.y;

				return OUT;
			}

			sampler2D _MainTexture;

			float _EffectTime;
			float _ObjectTime;

			fixed4 frag(v2f IN) : SV_Target
			{

			}
			ENDCG
		}
	}
}