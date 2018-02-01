struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer matrix_buffer : register (b0)
{
	matrix model_matrix;
}

float4 main( VS_IN input ) : SV_POSITION
{
	return mul(model_matrix, float4(input.pos, 1.0f));
}