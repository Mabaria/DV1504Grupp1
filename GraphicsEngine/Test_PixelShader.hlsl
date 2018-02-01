Texture2D font3D	: register(t0);
sampler ss			: register(s0);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	bool use_texture = input.tex.xy != -1.0f ? true : false;

	float3 ambient = use_texture ? 
		font3D.Sample(ss, input.tex.xy) : float3(1.0f, 1.0f, 1.0f);

	float blend = 1.0f;
	if (use_texture)
	{
		blend = ambient.xyz == 1.0f ?
			1.0f : 0.0f;
	}

	

	return float4(ambient, blend);
}