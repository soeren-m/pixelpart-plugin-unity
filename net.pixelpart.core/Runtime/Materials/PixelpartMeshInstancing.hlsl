UNITY_INSTANCING_BUFFER_START(Props)
	UNITY_DEFINE_INSTANCED_PROP(float4, _Color)
UNITY_INSTANCING_BUFFER_END(Props)

void Pixelpart_InstanceColor_float(out float4 color) {
	color = UNITY_ACCESS_INSTANCED_PROP(Props, _Color);
}
