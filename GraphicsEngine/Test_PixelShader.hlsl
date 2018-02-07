Texture2D texture2d		: register(t0);
sampler ss				: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD;
};

struct Light
{
	float3 dir;			// Direction for light
	float3 color;		// Color of light
	float illu;			// Amount to light up
};

float4 main(PS_IN input) : SV_TARGET
{
	float alpha			= 1.0f;
	float ambient		= 0.4f;

	bool use_texture	= input.tex.xy != -1.0f ? 
		true : false;

	float3 diffuse	= use_texture ?
		texture2d.Sample(ss, input.tex.xy) : float3(0.2f, 0.2f, 0.2f);
	

	if (use_texture) // When writing fonts
	{
		alpha = diffuse.rg > 0.8f && diffuse.b == 0.0f ? // Blend if yellow
			0.6f : 1.0f;


		diffuse -= 0.5f; // Simple fix for ugly edges
	}
	else // Everything else
	{
		Light lights[4];
		lights[0].dir = float3(0.0f, 1.0f, 0.0f);
		lights[0].color = float3(0.31f, 0.31f, 0.30f);
		lights[0].illu = 1.0f;

		lights[1].dir = float3(1.0f, 0.0f, 0.0f);
		lights[1].color = float3(0.05f, 0.05f, 0.12f);
		lights[1].illu = 1.0f;

		lights[2].dir = float3(0.0f, 0.0f, 1.0f);
		lights[2].color = float3(0.15f, 0.1f, 0.05f);
		lights[2].illu = 1.0f;

		lights[3].dir = float3(0.0f, 0.0f, -1.0f);
		lights[3].color = float3(0.10f, 0.05f, 0.08f);
		lights[3].illu = 1.0f;

		for (int i = 0; i < 4; i++)
		{
			lights[i].illu = saturate(dot(lights[i].dir, input.nor));
			diffuse += lights[i].color * lights[i].illu * 0.8f;
		}
	}

	if (alpha < 1.0f)
	{
		return float4(1.0f, 0.0f, 0.0f, alpha);
	}

	return float4(saturate(diffuse + ambient), alpha);
}