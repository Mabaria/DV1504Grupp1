struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWorld : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer matrix_buffer : register (b0)
{
	matrix world_matrix;
	matrix vp_matrix;
}

VS_OUT main( VS_IN input )
{
	VS_OUT output;
	
	output.posWorld = mul(world_matrix, float4(input.pos, 1.0f));
	output.pos = mul(vp_matrix, output.posWorld);
	output.nor = input.nor;
	output.tex = input.tex;
	return output;
}