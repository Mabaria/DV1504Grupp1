Texture2D font		: register(t0);
sampler ss			: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	float alpha		= 1.0f;
	float illuminate = 1.0f;

	bool use_font	= input.tex.xy != -1.0f ? 
		true : false;

	float3 ambient	= use_font ?
		font.Sample(ss, input.tex.xy) : float3(1.0f, 1.0f, 1.0f);

	if (use_font)
	{
		alpha =	ambient.rg == 1.0f && ambient.b == 0.0f ? // Blend if yellow
			0.0f : 1.0f;

		ambient -= 0.5f; // Simple fix for ugly edges
	}
	else
	{
		float3 light_direction = float3(0.0f, 1.0f, 0.0f);
		illuminate = dot(light_direction, input.nor);
		illuminate = saturate(illuminate);
	}

	return float4(illuminate * ambient + 0.2f, alpha);
}