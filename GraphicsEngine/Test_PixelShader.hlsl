struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	float3 light_direction = float3(0.0f, 1.0f, 0.0f);
	float illuminate = dot(light_direction, input.nor);

	illuminate = saturate(illuminate);

	return float4(illuminate.xxx + 0.2f, 1.0f);
}