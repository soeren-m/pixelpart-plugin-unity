#ifndef PIXELPART_CUSTOM_MATERIAL_COMMON_HDRP_HLSL
#define PIXELPART_CUSTOM_MATERIAL_COMMON_HDRP_HLSL

float Pixelpart_Mod(float x, float y) {
	return x - y * floor(x / y);
}
float2 Pixelpart_Mod(float2 x, float2 y) {
	return x - y * floor(x / y);
}
float3 Pixelpart_Mod(float3 x, float3 y) {
	return x - y * floor(x / y);
}

#if defined(UNITY_SINGLE_PASS_STEREO)
float2 Pixelpart_TransformStereoScreenSpaceTex(float2 uv, float w)
{
    float4 scaleOffset = unity_StereoScaleOffset[unity_StereoEyeIndex];
    return uv.xy * scaleOffset.xy + scaleOffset.zw * w;
}
#endif

float4 Pixelpart_ComputeNonStereoScreenPos(float4 pos) {
	float4 o = pos * 0.5f;
	o.xy = float2(o.x, o.y*_ProjectionParams.x) + o.w;
	o.zw = pos.zw;
	return o;
}

float4 Pixelpart_ComputeScreenPos(float4 pos) {
	float4 o = Pixelpart_ComputeNonStereoScreenPos(pos);
#if defined(UNITY_SINGLE_PASS_STEREO)
	o.xy = Pixelpart_TransformStereoScreenSpaceTex(o.xy, pos.w);
#endif

	return o;
}

float Pixelpart_NoiseRandom(float x) {
	return frac(sin(x) * 43758.5453123);
}
float Pixelpart_Noise1d(float x) {
	float fl = floor(x);
	float fc = frac(x);

	return lerp(Pixelpart_NoiseRandom(fl), Pixelpart_NoiseRandom(fl + 1.0), fc);
}

float3 Pixelpart_NoisePermute289(float3 x) {
	return Pixelpart_Mod(((x * 34.0) + 1.0) * x, 289.0);
}
float Pixelpart_Noise2d(float2 v) {
	const float4 C = float4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);

	float2 i = floor(v + dot(v, C.yy));
	float2 x0 = v - i + dot(i, C.xx);
	float2 i1 = (x0.x > x0.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
	float4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	i = Pixelpart_Mod(i, 289.0);

	float3 p = Pixelpart_NoisePermute289(Pixelpart_NoisePermute289(i.y + float3(0.0, i1.y, 1.0)) + i.x + float3(0.0, i1.x, 1.0));
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

float2 Pixelpart_SpriteSheetAnimation(float2 textureCoord, float life, int origin, uint startFrame, uint numFrames, uint rowNumber, uint columnNumber, float duration, bool animLoop) {
	uint animationFrame = startFrame;
	animationFrame += animLoop
		? uint(Pixelpart_Mod(life / duration * float(numFrames), float(numFrames)))
		: clamp(uint(life / duration * float(numFrames)), 0, numFrames - 1);

	switch(origin) {
		case 0:
			return float2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				float2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 1:
			return float2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				float2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 2:
			return float2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				float2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 3:
			return float2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				float2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		default:
			return textureCoord;
	}
}

float4 Pixelpart_Blend(float4 colorA, float4 colorB, int blendMode) {
	float4 result = colorA;

	switch(blendMode) {
		case 0:
			result = colorA * colorB;
			break;
		case 1:
			result.rgb = clamp(colorA.rgb + colorB.rgb, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0));
			result.a = colorA.a * colorB.a;
			break;
		case 2:
			result.rgb = clamp(colorA.rgb - colorB.rgb, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0));
			result.a = colorA.a * colorB.a;
			break;
		case 3:
			result.rgb = abs(colorA.rgb - colorB.rgb);
			result.a = colorA.a * colorB.a;
			break;
		case 4:
			result.rgb = float3(1.0, 1.0, 1.0) - (float3(1.0, 1.0, 1.0) - colorA.rgb) * (float3(1.0, 1.0, 1.0) - colorB.rgb);
			result.a = colorA.a * colorB.a;
			break;
		case 5:
			result.r = (colorA.r < 0.5) ? (2.0 * colorA.r * colorB.r) : (1.0 - 2.0 * (1.0 - colorA.r) * (1.0 - colorB.r));
			result.g = (colorA.g < 0.5) ? (2.0 * colorA.g * colorB.g) : (1.0 - 2.0 * (1.0 - colorA.g) * (1.0 - colorB.g));
			result.b = (colorA.b < 0.5) ? (2.0 * colorA.b * colorB.b) : (1.0 - 2.0 * (1.0 - colorA.b) * (1.0 - colorB.b));
			result.a = colorA.a * colorB.a;
			break;
		case 6:
			result.rgb = max(colorA.rgb, colorB.rgb);
			result.a = colorA.a * colorB.a;
			break;
		case 7:
			result.rgb = min(colorA.rgb, colorB.rgb);
			result.a = colorA.a * colorB.a;
			break;
		case 8:
			result.rgb = colorB.rgb;
			result.a = colorA.a * colorB.a;
			break;
		default:
			break;
	}

	return result;
}

float Pixelpart_SoftParticleFade(float4 fragCoord, float transitionSize) {
	float sceneDepth = LinearEyeDepth(SampleCameraDepth(fragCoord.xy / fragCoord.w), _ZBufferParams);
	float depthDistance = clamp(sceneDepth - fragCoord.z, 0.0, 1.0);

	return smoothstep(0.0, transitionSize, depthDistance);
}

float Pixelpart_DistanceFade(float fragDepth, float transitionSize) {
	return smoothstep(0.0, transitionSize, fragDepth);
}

#endif
