struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer model_buffer : register (b0)
{
	matrix model;
}

cbuffer view_buffer : register (b1)
{
	matrix view;
}

cbuffer proj_buffer : register (b2)
{
	matrix proj;
}

VS_OUT main( VS_IN input )
{
	VS_OUT output;
	matrix vm		= mul(view, model);
	matrix mvp		= mul(proj, vm);
	output.worldPos = mul(model, float4(input.pos, 1.0f));
	output.pos		= mul(mvp, float4(input.pos, 1.0f));
	output.nor		= input.nor;
	output.tex		= input.tex;
	return output;
}