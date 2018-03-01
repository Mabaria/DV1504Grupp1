struct VS_IN
{
	float3 pos : POSITION;
	uint data : DATA;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 wpos : POSITION;
	uint data : DATA;
};

cbuffer view_buffer : register (b1)
{
	matrix view;
}

cbuffer proj_buffer : register (b2)
{
	matrix proj;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.pos = mul(mul(proj, view),float4(input.pos.xyz, 1.0f));
	output.wpos = float4(input.pos.xyz, 1.0f);
	output.data = input.data;
	return output;
}