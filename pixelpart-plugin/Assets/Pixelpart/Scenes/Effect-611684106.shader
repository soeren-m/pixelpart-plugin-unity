Shader "Pixelpart/Effect-611684106"
{
Properties
{
[MaterialToggle] PixelSnap ("Pixel Snap", Float) = 0
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
Blend [_SrcBlendMode] [_DstBlendMode]

Pass
{
CGPROGRAM
#pragma vertex vert
#pragma fragment frag
#pragma multi_compile _ PIXELSNAP_ON
#include "UnityCG.cginc"

static const int VALUE_BUFFER_SIZE = 100;

struct appdata_t
{
float4 vertex : POSITION;
float4 color : COLOR;
float2 uv : TEXCOORD0;
float4 velocity : TEXCOORD1;
float4 force : TEXCOORD2;
float4 info : TEXCOORD3;
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
OUT.life = IN.info.x;
OUT.id = IN.info.y;
#ifdef PIXELSNAP_ON
OUT.vertex = UnityPixelSnap(OUT.vertex);
#endif
return OUT;
}

sampler2D _Texture0;
sampler2D _Texture1;
sampler2D _Texture2;
sampler2D _Texture3;
sampler2D _Texture4;
sampler2D _Texture5;
sampler2D _Texture6;
sampler2D _Texture7;
float _EffectTime;
float _ObjectTime;

float3 simplex_permute(float3 x) {
return fmod(((x * 34.0) + 1.0) * x, 289.0);
}
float simplex_noise(float2 v) {
const float4 C = float4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
float2 i = floor(v + dot(v, C.yy));
float2 x0 = v - i + dot(i, C.xx);
float2 i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
float4 x12 = x0.xyxy + C.xxzz;
x12.xy -= i1;
i = fmod(i, 289.0);
float3 p = simplex_permute(simplex_permute(i.y + float3(0.0, i1.y, 1.0)) + i.x + float3(0.0, i1.x, 1.0));
float3 m = max(0.5 - float3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
m = m * m;
m = m * m;
float3 x = 2.0 * frac(p * C.www) - 1.0;
float3 h = abs(x) - 0.5;
float3 ox = floor(x + 0.5);
float3 a0 = x - ox;
m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
float3 g;
g.x = a0.x * x0.x + h.x * x0.y;
g.yz = a0.yz * x12.xz + h.yz * x12.yw;
return 130.0 * dot(m, g);
}

fixed4 frag(v2f IN) : SV_Target
{

float2 var0 = IN.uv;
float4 var2 = tex2D(_Texture0, var0);
float4 var4 = IN.color;
float4 var6 = float4(1.0, 1.0, 1.0, 1.0);
if(0 == 0) { var6 = var2 * var4; }
else if(0 == 1) { var6.rgb = clamp(var2.rgb + var4.rgb, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0)); var6.a = var2.a * var4.a; }
else if(0 == 2) { var6.rgb = clamp(var2.rgb - var4.rgb, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0)); var6.a = var2.a * var4.a; }
else if(0 == 3) { var6.rgb = abs(var2.rgb - var4.rgb); var6.a = var2.a * var4.a; }
else if(0 == 4) { var6.rgb = float3(1.0, 1.0, 1.0) - (float3(1.0, 1.0, 1.0) - var2.rgb) * (float3(1.0, 1.0, 1.0) - var4.rgb); var6.a = var2.a * var4.a; }
else if(0 == 5) { var6.r = (var2.r < 0.5) ? (2.0 * var2.r * var4.r) : (1.0 - 2.0 * (1.0 - var2.r) * (1.0 - var4.r)); var6.r = (var2.g < 0.5) ? (2.0 * var2.g * var4.g) : (1.0 - 2.0 * (1.0 - var2.g) * (1.0 - var4.g)); var6.b = (var2.b < 0.5) ? (2.0 * var2.b * var4.b) : (1.0 - 2.0 * (1.0 - var2.b) * (1.0 - var4.b)); var6.a = var2.a * var4.a; }
else if(0 == 6) { var6.rgb = max(var2.rgb, var4.rgb); var6.a = var2.a * var4.a; }
else if(0 == 7) { var6.rgb = min(var2.rgb, var4.rgb); var6.a = var2.a * var4.a; }
else if(0 == 8) { var6.rgb = var4.rgb; var6.a = var2.a * var4.a; }
fixed4 out_Color = var6;
out_Color.rgb *= out_Color.a;
return out_Color;
}
ENDCG
}
}
}