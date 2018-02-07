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
	//float3 ambient		= 0.32f;
	float3 ambient		= 0.15f;
	float3 diffuse		= 0.0f;

	bool use_texture	= input.tex.xy != -1.0f ? 
		true : false;

	ambient	= use_texture ?
		texture2d.Sample(ss, input.tex.xy) : ambient;
	

	if (use_texture) // When writing fonts
	{
		alpha = ambient.rg > 0.8f && ambient.b == 0.0f ? // Blend if yellow
			0.5f : 1.0f;

		diffuse -= 0.2f; // Simple fix for ugly edges
	}
	else // Everything else
	{
		Light lights[4];

		// Top light
		lights[0].dir = float3(0.0f, 1.0f, 0.0f);
		lights[0].color = float3(0.45f, 0.45f, 0.45f);
		lights[0].illu = 1.0f;


		// Front light
		lights[2].dir = float3(0.0f, 0.0f, 1.0f);
		lights[2].color = float3(0.15f, 0.1f, 0.05f);
		lights[2].illu = 1.0f;


		// Left light
		lights[1].dir = float3(1.0f, 0.0f, 0.0f);
		lights[1].color = float3(0.11f, 0.09f, 0.08f);
		lights[1].illu = 1.0f;


		// Back light
		lights[3].dir = float3(0.0f, 0.0f, -1.0f);
		lights[3].color = float3(0.05f, 0.06f, 0.1f);
		lights[3].illu = 1.0f;

		for (int i = 0; i < 4; i++)
		{
			lights[i].illu = saturate(dot(lights[i].dir, input.nor));
			diffuse += lights[i].color * lights[i].illu * 0.65f;
		}
	}

	if (alpha < 1.0f)
	{
		ambient = float3(1.0f, 0.0f, 0.0f);
	}

	float s = pow(input.pos.z, 35.0f);

	float limit = 0.3f;
	if (s > limit)
		s = limit;

	return float4(saturate(diffuse + ambient + s.xxx), alpha);
}