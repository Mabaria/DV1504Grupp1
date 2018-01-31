struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

float4 main( VS_IN input ) : SV_POSITION
{
	return float4(input.pos.x, input.pos.y, input.pos.z, 1.0f);
}