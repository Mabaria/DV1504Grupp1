cbuffer MATERIAL_BUFFER : register(b1)
{
	float3 Diffuse;
	float3 Ambient;
	float3 Specular;
	float SpecExp;
	float Opacity;
}

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


float3 GetBoatShading(PS_IN input);

float4 main(PS_IN input) : SV_TARGET
{
	float3 diffuse = Diffuse;
	diffuse += GetBoatShading(input);

	return float4(diffuse, 1.0f);
}


float3 GetBoatShading(PS_IN input)
{
	float3 shading = (float3)0;

	Light lights[4];

	// Top light
	lights[0].dir = float3(0.0f, 1.0f, 0.0f);
	lights[0].color = float3(0.25f, 0.25f, 0.25f);
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
		shading += lights[i].color * lights[i].illu * 0.65f;
	}

	return shading;
}