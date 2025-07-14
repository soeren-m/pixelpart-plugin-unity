using System;
using UnityEngine;

namespace Pixelpart
{
    internal static class PixelpartShaderGenerator
    {
        public static string GenerateShaderCode(string shaderName, string mainCode, string parameterCode, BlendModeType blendMode, LightingModeType lightingMode, bool instancing)
        {
            if (lightingMode != LightingModeType.Unlit)
            {
                Debug.LogError("[Pixelpart] Custom shaders for lit materials are not supported");
                return string.Empty;
            }

#if PIXELPART_USE_URP
            var shaderCode = instancing ? InstancedUnlitShaderTemplateURP : UnlitShaderTemplateURP;
#elif PIXELPART_USE_HDRP
            var shaderCode = instancing ? InstancedUnlitShaderTemplateHDRP : UnlitShaderTemplateHDRP;
#else
            var shaderCode = instancing ? InstancedUnlitShaderTemplate : UnlitShaderTemplate;
#endif

            var renderType = string.Empty;
            var queue = string.Empty;
            var cull = string.Empty;
            var zWrite = string.Empty;
            var blendOp = string.Empty;
            var srcBlendMode = string.Empty;
            var dstBlendMode = string.Empty;

            if (blendMode == BlendModeType.Off)
            {
                renderType = "Opaque";
                queue = "Geometry";
                cull = "Back";
                zWrite = "On";
            }
            else
            {
                renderType = "Transparent";
                queue = "Transparent";
                cull = "Off";
                zWrite = "Off";
            }

#if PIXELPART_USE_HDRP
            renderType = "HDUnlitShader";
#endif

            switch (blendMode)
            {
                case BlendModeType.Normal:
                    blendOp = "Add";
                    srcBlendMode = "SrcAlpha";
                    dstBlendMode = "OneMinusSrcAlpha";
                    break;
                case BlendModeType.Additive:
                    blendOp = "Add";
                    srcBlendMode = "SrcAlpha";
                    dstBlendMode = "One";
                    break;
                case BlendModeType.Subtractive:
                    blendOp = "ReverseSubtract";
                    srcBlendMode = "SrcAlpha";
                    dstBlendMode = "One";
                    break;
                default:
                    blendOp = "Add";
                    srcBlendMode = "One";
                    dstBlendMode = "Zero";
                    break;
            }

            shaderCode = shaderCode.Replace("{name}", shaderName);
            shaderCode = shaderCode.Replace("{rendertype}", renderType);
            shaderCode = shaderCode.Replace("{queue}", queue);
            shaderCode = shaderCode.Replace("{cull}", cull);
            shaderCode = shaderCode.Replace("{zwrite}", zWrite);
            shaderCode = shaderCode.Replace("{blendop}", blendOp);
            shaderCode = shaderCode.Replace("{srcblendmode}", srcBlendMode);
            shaderCode = shaderCode.Replace("{dstblendmode}", dstBlendMode);
            shaderCode = shaderCode.Replace("{main}", mainCode);
            shaderCode = shaderCode.Replace("{parameter}", parameterCode);
            shaderCode = shaderCode.Replace("{property}", GenerateShaderPropertyCode(parameterCode));

            return shaderCode;
        }

        private static string GenerateShaderPropertyCode(string parameterCode)
        {
            var result = string.Empty;
            var parameterLines = parameterCode.Split(new string[] { "\n" }, StringSplitOptions.RemoveEmptyEntries);

            foreach (var parameterLine in parameterLines)
            {
                var lineTokens = parameterLine.Split(new string[] { " " }, StringSplitOptions.RemoveEmptyEntries);
                if (lineTokens.Length < 2)
                {
                    continue;
                }

                var parameterType = lineTokens[0];
                var parameterName = lineTokens[1].Remove(lineTokens[1].Length - 1);

                var displayName = parameterName.Substring(1);

                var typeName = "Int";
                var defaultValue = "0";

                switch (parameterType)
                {
                    case "bool":
                    case "int":
                        typeName = "Int";
                        defaultValue = "0";
                        break;
                    case "float":
                        typeName = "Float";
                        defaultValue = "0.0";
                        break;
                    case "float2":
                    case "float3":
                    case "float4":
                        typeName = "Vector";
                        defaultValue = "(0, 0, 0, 0)";
                        break;
                    default:
                        break;
                }

                var propertyLine =
                    parameterName + " (\"" + displayName + "\", " + typeName + ") = " + defaultValue + "\n";

                result += propertyLine;
            }

            return result;
        }

#pragma warning disable 0414
        private static readonly string UnlitShaderTemplate = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Lighting Off
		Cull {cull}
		ZWrite {zwrite}
		BlendOp {blendop}
		Blend {srcblendmode} {dstblendmode}

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCommon.cginc""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 additional : TEXCOORD2;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				float2 uv : TEXCOORD0;
				float3 velocity : TEXCOORD1;
				float life : TEXCOORD2;
				float id : TEXCOORD3;
				float3 worldPosition : TEXCOORD4;
				float3 worldNormal : TEXCOORD5;
				float4 projectedPosition : TEXCOORD6;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;
				OUT.color = IN.color;
				OUT.velocity = IN.velocity.xyz;
				OUT.life = IN.additional.x;
				OUT.id = IN.additional.y;
				OUT.worldPosition = mul(unity_ObjectToWorld, IN.vertex).xyz;
				OUT.worldNormal = UnityObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -UnityObjectToViewPos(IN.vertex).z;

				return OUT;
			}

			float _EffectTime;
			float _ObjectTime;

			UNITY_DECLARE_DEPTH_TEXTURE(_CameraDepthTexture);
			sampler2D _Texture0;
			sampler2D _Texture1;
			sampler2D _Texture2;
			sampler2D _Texture3;
			sampler2D _Texture4;
			sampler2D _Texture5;
			sampler2D _Texture6;
			sampler2D _Texture7;

{parameter}

			fixed4 frag(v2f IN) : SV_Target
			{
				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = mul(UNITY_MATRIX_V, float4(IN.worldPosition, 1.0)).xyz;
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = mul(UNITY_MATRIX_V, float4(IN.worldNormal, 0.0)).xyz;
				fixed4 in_Color = IN.color;
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = IN.velocity;
				float in_Life = IN.life;
				int in_ObjectId = int(IN.id);
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDCG
		}
	}
}";

        private static readonly string InstancedUnlitShaderTemplate = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Lighting Off
		Cull {cull}
		ZWrite {zwrite}
		BlendOp {blendop}
		Blend {srcblendmode} {dstblendmode}

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_instancing

			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCommon.cginc""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
				float3 worldPosition : TEXCOORD1;
				float3 worldNormal : TEXCOORD2;
				float4 projectedPosition : TEXCOORD3;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			UNITY_INSTANCING_BUFFER_START(PixelpartProps)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Velocity)
				UNITY_DEFINE_INSTANCED_PROP(float, _Life)
				UNITY_DEFINE_INSTANCED_PROP(float, _ObjectId)
			UNITY_INSTANCING_BUFFER_END(PixelpartProps)

			v2f vert(appdata_t IN)
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.uv = IN.uv;
				OUT.worldPosition = mul(unity_ObjectToWorld, IN.vertex).xyz;
				OUT.worldNormal = UnityObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -UnityObjectToViewPos(IN.vertex).z;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			float _EffectTime;
			float _ObjectTime;

			UNITY_DECLARE_DEPTH_TEXTURE(_CameraDepthTexture);
			sampler2D _Texture0;
			sampler2D _Texture1;
			sampler2D _Texture2;
			sampler2D _Texture3;
			sampler2D _Texture4;
			sampler2D _Texture5;
			sampler2D _Texture6;
			sampler2D _Texture7;

{parameter}

			fixed4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = mul(UNITY_MATRIX_V, float4(IN.worldPosition, 1.0)).xyz;
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = mul(UNITY_MATRIX_V, float4(IN.worldNormal, 0.0)).xyz;
				fixed4 in_Color = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Color);
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Velocity).xyz;
				float in_Life = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Life);
				int in_ObjectId = int(UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _ObjectId));
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDCG
		}
	}
}";

        private static readonly string UnlitShaderTemplateURP = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderPipeline"" = ""UniversalRenderPipeline""
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Cull {cull}
		ZWrite {zwrite}
		BlendOp {blendop}
		Blend {srcblendmode} {dstblendmode}

		Pass
		{
			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl""
			#include ""Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareDepthTexture.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonURP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 additional : TEXCOORD2;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				half4 color : COLOR;
				float2 uv : TEXCOORD0;
				float3 velocity : TEXCOORD1;
				float life : TEXCOORD2;
				float id : TEXCOORD3;
				float3 worldPosition : TEXCOORD4;
				float3 worldNormal : TEXCOORD5;
				float4 projectedPosition : TEXCOORD6;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.color = IN.color;
				OUT.velocity = IN.velocity.xyz;
				OUT.life = IN.additional.x;
				OUT.id = IN.additional.y;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = IN.color;
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = IN.velocity;
				float in_Life = IN.life;
				int in_ObjectId = int(IN.id);
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}
	}
}";

        private static readonly string InstancedUnlitShaderTemplateURP = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderPipeline"" = ""UniversalRenderPipeline""
			""DisableBatching"" = ""True""
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Cull {cull}
		ZWrite {zwrite}
		BlendOp {blendop}
		Blend {srcblendmode} {dstblendmode}

		Pass
		{
			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_instancing

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl""
			#include ""Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareDepthTexture.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonURP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
				float3 worldPosition : TEXCOORD1;
				float3 worldNormal : TEXCOORD2;
				float4 projectedPosition : TEXCOORD3;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			UNITY_INSTANCING_BUFFER_START(PixelpartProps)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Velocity)
				UNITY_DEFINE_INSTANCED_PROP(float, _Life)
				UNITY_DEFINE_INSTANCED_PROP(float, _ObjectId)
			UNITY_INSTANCING_BUFFER_END(PixelpartProps)

			v2f vert(appdata_t IN)
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Color);
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Velocity).xyz;
				float in_Life = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Life);
				int in_ObjectId = int(UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _ObjectId));
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}
	}
}";

        private static readonly string UnlitShaderTemplateHDRP = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderPipeline"" = ""HDRenderPipeline""
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Pass
		{
			Name ""DepthForwardOnly""
			Tags
			{
				""RenderPipeline"" = ""HDRenderPipeline""
				""LightMode"" = ""DepthForwardOnly""
				""RenderType"" = ""{rendertype}""
				""Queue"" = ""{queue}""
			}

			Cull {cull}
			ZWrite {zwrite}
			BlendOp {blendop}
			Blend {srcblendmode} {dstblendmode}

			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonHDRP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 additional : TEXCOORD2;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				half4 color : COLOR;
				float2 uv : TEXCOORD0;
				float3 velocity : TEXCOORD1;
				float life : TEXCOORD2;
				float id : TEXCOORD3;
				float3 worldPosition : TEXCOORD4;
				float3 worldNormal : TEXCOORD5;
				float4 projectedPosition : TEXCOORD6;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.color = IN.color;
				OUT.velocity = IN.velocity.xyz;
				OUT.life = IN.additional.x;
				OUT.id = IN.additional.y;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = Pixelpart_ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = IN.color;
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = IN.velocity;
				float in_Life = IN.life;
				int in_ObjectId = int(IN.id);
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}

		Pass
		{
			Name ""ForwardOnly""
			Tags
			{
				""RenderPipeline"" = ""HDRenderPipeline""
				""LightMode"" = ""ForwardOnly""
				""RenderType"" = ""{rendertype}""
				""Queue"" = ""{queue}""
			}

			Cull {cull}
			ZWrite {zwrite}
			BlendOp {blendop}
			Blend {srcblendmode} {dstblendmode}

			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonHDRP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float4 color : COLOR;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				float4 velocity : TEXCOORD1;
				float4 additional : TEXCOORD2;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				half4 color : COLOR;
				float2 uv : TEXCOORD0;
				float3 velocity : TEXCOORD1;
				float life : TEXCOORD2;
				float id : TEXCOORD3;
				float3 worldPosition : TEXCOORD4;
				float3 worldNormal : TEXCOORD5;
				float4 projectedPosition : TEXCOORD6;
			};

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.color = IN.color;
				OUT.velocity = IN.velocity.xyz;
				OUT.life = IN.additional.x;
				OUT.id = IN.additional.y;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = Pixelpart_ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = IN.color;
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = IN.velocity;
				float in_Life = IN.life;
				int in_ObjectId = int(IN.id);
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}
	}
}";

        private static readonly string InstancedUnlitShaderTemplateHDRP = @"Shader ""PixelpartCustom/{name}""
{
	Properties
	{
		_EffectTime (""Effect Time"", Float) = 0.0
		_ObjectTime (""Object Time"", Float) = 0.0

		_Texture0 (""Texture 0"", 2D) = ""white"" {}
		_Texture1 (""Texture 1"", 2D) = ""white"" {}
		_Texture2 (""Texture 2"", 2D) = ""white"" {}
		_Texture3 (""Texture 3"", 2D) = ""white"" {}
		_Texture4 (""Texture 4"", 2D) = ""white"" {}
		_Texture5 (""Texture 5"", 2D) = ""white"" {}
		_Texture6 (""Texture 6"", 2D) = ""white"" {}
		_Texture7 (""Texture 7"", 2D) = ""white"" {}

{property}
	}

	SubShader
	{
		Tags
		{
			""RenderPipeline"" = ""HDRenderPipeline""
			""DisableBatching"" = ""True""
			""RenderType"" = ""{rendertype}""
			""Queue"" = ""{queue}""
		}

		Pass
		{
			Name ""DepthForwardOnly""
			Tags
			{
				""RenderPipeline"" = ""HDRenderPipeline""
				""DisableBatching"" = ""True""
				""LightMode"" = ""DepthForwardOnly""
				""RenderType"" = ""{rendertype}""
				""Queue"" = ""{queue}""
			}

			Cull {cull}
			ZWrite {zwrite}
			BlendOp {blendop}
			Blend {srcblendmode} {dstblendmode}

			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_instancing

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonHDRP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
				float3 worldPosition : TEXCOORD1;
				float3 worldNormal : TEXCOORD2;
				float4 projectedPosition : TEXCOORD3;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			UNITY_INSTANCING_BUFFER_START(PixelpartProps)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Velocity)
				UNITY_DEFINE_INSTANCED_PROP(float, _Life)
				UNITY_DEFINE_INSTANCED_PROP(float, _ObjectId)
			UNITY_INSTANCING_BUFFER_END(PixelpartProps)

			v2f vert(appdata_t IN)
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = Pixelpart_ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Color);
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Velocity).xyz;
				float in_Life = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Life);
				int in_ObjectId = int(UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _ObjectId));
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}

		Pass
		{
			Name ""ForwardOnly""
			Tags
			{
				""RenderPipeline"" = ""HDRenderPipeline""
				""DisableBatching"" = ""True""
				""LightMode"" = ""ForwardOnly""
				""RenderType"" = ""{rendertype}""
				""Queue"" = ""{queue}""
			}

			Cull {cull}
			ZWrite {zwrite}
			BlendOp {blendop}
			Blend {srcblendmode} {dstblendmode}

			HLSLPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_instancing

			#include ""Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl""
			#include ""Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl""
			#include ""Packages/net.pixelpart/Runtime/Materials/PixelpartCustomMaterialCommonHDRP.hlsl""

			struct appdata_t
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float2 uv : TEXCOORD0;
				float3 worldPosition : TEXCOORD1;
				float3 worldNormal : TEXCOORD2;
				float4 projectedPosition : TEXCOORD3;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			UNITY_INSTANCING_BUFFER_START(PixelpartProps)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
				UNITY_DEFINE_INSTANCED_PROP(float4, _Velocity)
				UNITY_DEFINE_INSTANCED_PROP(float, _Life)
				UNITY_DEFINE_INSTANCED_PROP(float, _ObjectId)
			UNITY_INSTANCING_BUFFER_END(PixelpartProps)

			v2f vert(appdata_t IN)
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				v2f OUT;
				OUT.vertex = TransformObjectToHClip(IN.vertex.xyz);
				OUT.uv = IN.uv;
				OUT.worldPosition = TransformObjectToWorld(IN.vertex.xyz);
				OUT.worldNormal = TransformObjectToWorldNormal(IN.normal);
				OUT.projectedPosition = Pixelpart_ComputeScreenPos(OUT.vertex);
				OUT.projectedPosition.z = -TransformWorldToView(OUT.worldPosition).z;
				UNITY_TRANSFER_INSTANCE_ID(IN, OUT);

				return OUT;
			}

			TEXTURE2D(_Texture0);
			TEXTURE2D(_Texture1);
			TEXTURE2D(_Texture2);
			TEXTURE2D(_Texture3);
			TEXTURE2D(_Texture4);
			TEXTURE2D(_Texture5);
			TEXTURE2D(_Texture6);
			TEXTURE2D(_Texture7);
			SAMPLER(sampler_Texture0);
			SAMPLER(sampler_Texture1);
			SAMPLER(sampler_Texture2);
			SAMPLER(sampler_Texture3);
			SAMPLER(sampler_Texture4);
			SAMPLER(sampler_Texture5);
			SAMPLER(sampler_Texture6);
			SAMPLER(sampler_Texture7);

CBUFFER_START(UnityPerMaterial)
float _EffectTime;
float _ObjectTime;
{parameter}
CBUFFER_END

			half4 frag(v2f IN) : SV_Target
			{
				UNITY_SETUP_INSTANCE_ID(IN);

				float3 in_WorldPosition = IN.worldPosition;
				float3 in_ViewPosition = TransformWorldToView(IN.worldPosition);
				float3 in_WorldNormal = IN.worldNormal;
				float3 in_ViewNormal = TransformWorldToViewDir(IN.worldNormal);
				half4 in_Color = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Color);
				float2 in_TextureCoord = IN.uv;
				float3 in_Velocity = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Velocity).xyz;
				float in_Life = UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _Life);
				int in_ObjectId = int(UNITY_ACCESS_INSTANCED_PROP(PixelpartProps, _ObjectId));
				float4 in_ProjectedPosition = IN.projectedPosition;

{main}

				return out_Color;
			}
			ENDHLSL
		}
	}
}";
#pragma warning restore 0414
    }
}