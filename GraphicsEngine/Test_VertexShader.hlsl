struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.0f);
	output.nor = input.nor;
	output.tex = input.tex;

	return output;
}