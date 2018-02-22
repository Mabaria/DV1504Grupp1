cbuffer MATERIAL_BUFFER : register(b1)
{
	float3 Diffuse;
	float3 Ambient;
	float3 Specular;
	float SpecExp;
	float Opacity;
}

Texture2D text3d		: register(t0);
sampler ss				: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	float4 color = text3d.Sample(ss, input.tex);
	float4 ambient = color.a == 1 ?
		color : float4(0.2f, 0.2f, 0.2f, 0.5f);

	return ambient;
}

