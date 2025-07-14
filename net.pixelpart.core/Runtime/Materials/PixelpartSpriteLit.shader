Shader "Pixelpart/PixelpartSpriteLit"
{
	Properties
	{
		[Enum(Off,0,On,1)] _ZWrite ("Z Write", Float) = 0
		[Enum(UnityEngine.Rendering.BlendOp)] _BlendOp ("Blend Operation", Float) = 0
		[Enum(UnityEngine.Rendering.BlendMode)] _SrcBlendMode ("Src Blend Mode", Float) = 0
		[Enum(UnityEngine.Rendering.BlendMode)] _DstBlendMode ("Dst Blend Mode", Float) = 0

		_MainTexture ("Main Texture", 2D) = "white" {}

		_Emission ("Emission", Vector) = (0, 0, 0, 0)
		_Roughness ("Roughness", Float) = 0.5
		_Metallic ("Metallic", Float) = 0.0

		_ColorBlendMode ("Color Blend Mode", Int) = 0

		_SpriteSheetRowNumber ("Sprite Sheet Row Number", Int) = 1
		_SpriteSheetColumnNumber ("Sprite Sheet Column Number", Int) = 1
		_SpriteSheetOrigin ("Sprite Sheet Origin", Int) = 0

		_SpriteAnimationNumFrames ("Sprite Animation Num Frames", Int) = 1
		_SpriteAnimationStartFrame ("Sprite Animation Start Frame", Int) = 0
		_SpriteAnimationDuration ("Sprite Animation Duration", Float) = 1.0
		_SpriteAnimationLoop ("Sprite Animation Loop", Int) = 0

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

		Cull Off
		ZWrite [_ZWrite]
		BlendOp [_BlendOp]
		Blend [_SrcBlendMode] [_DstBlendMode]

		CGPROGRAM
		#pragma target 3.0
		#pragma surface surf Standard alpha:blend vertex:vert

		#include "PixelpartCommon.cginc"

		struct Input
		{
			float4 color : COLOR;
			float2 uv_MainTexture;
			float4 additionalData;
		};

		void vert(inout appdata_full IN, out Input OUT)
		{
			UNITY_INITIALIZE_OUTPUT(Input, OUT);

			OUT.color = IN.color;
			OUT.additionalData = ComputeScreenPos(UnityObjectToClipPos(IN.vertex));
			OUT.additionalData.z = -UnityObjectToViewPos(IN.vertex).z;
			OUT.additionalData.w = IN.texcoord2.x;
		}

		UNITY_DECLARE_DEPTH_TEXTURE(_CameraDepthTexture);
		sampler2D _MainTexture;

		float3 _Emission;
		float _Roughness;
		float _Metallic;

		int _ColorBlendMode;

		int _SpriteSheetRowNumber;
		int _SpriteSheetColumnNumber;
		int _SpriteSheetOrigin;

		int _SpriteAnimationNumFrames;
		int _SpriteAnimationStartFrame;
		float _SpriteAnimationDuration;
		bool _SpriteAnimationLoop;

		bool _SoftParticles;
		float _SoftParticleTransition;
		bool _DistanceFade;
		float _DistanceFadeTransition;

		void surf(Input IN, inout SurfaceOutputStandard OUT)
		{
			float2 textureCoord = Pixelpart_SpriteSheetAnimation(IN.uv_MainTexture, IN.additionalData.w,
				_SpriteSheetOrigin, uint(_SpriteAnimationStartFrame), uint(_SpriteAnimationNumFrames),
				uint(_SpriteSheetRowNumber), uint(_SpriteSheetColumnNumber),
				_SpriteAnimationDuration, _SpriteAnimationLoop);

			float4 projectedPosition = float4(IN.additionalData.xyz, 1.0);

			fixed4 color = tex2D(_MainTexture, textureCoord);
			color = Pixelpart_Blend(color, IN.color, _ColorBlendMode);

			float softParticleBlendFactor = _SoftParticles
				? Pixelpart_SoftParticleFade(_CameraDepthTexture, projectedPosition, _SoftParticleTransition)
				: 1.0;
			color.a *= softParticleBlendFactor;

			float distanceBlendFactor = _DistanceFade
				? Pixelpart_DistanceFade(projectedPosition.z, _DistanceFadeTransition)
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