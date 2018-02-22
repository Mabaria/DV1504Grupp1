Texture2D texture2d		: register(t0);
sampler ss				: register(s0);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	//uint data : DATA;
};

float4 main(PS_IN input) : SV_TARGET
{
	return texture2d.Sample(ss, input.tex);
	//return float4(1.0f, 0.0f, 1.0f, 1.0f);
}