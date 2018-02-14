cbuffer event_data : register(b0)
{
	float4 events;
}

cbuffer MATERIAL_BUFFER : register(b1)
{
	float3 Diffuse;
	float3 Ambient;
	float3 Specular;
	float SpecExp;
	float Opacity;
}


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

int GetNrOfEvents();
float3 GetEvent(int event);
float3 GetBoatShading(PS_IN input);
float4 main(PS_IN input) : SV_TARGET
{
	float alpha = 1.0f;
	float3 ambient = Ambient;
	float3 diffuse = Diffuse;

	bool use_texture = input.tex.xy != -1.0f ?
	true : false;

	ambient = use_texture ?
	texture2d.Sample(ss, input.tex.xy) : ambient;

	// If events are active for a bounding box
	bool Are_there_active_events = events.r > 0.0f ?
	true : false;


	if (use_texture) // When blending is needed
	{
		alpha = ambient.rg > 0.8f && ambient.b == 0.0f ? // Blend if yellow
			0.5f : 1.0f;

		diffuse -= 0.2f; // Simple fix for ugly edges

		if (alpha != 1.0f) // If it's going to blend, turn it grey
		{
			ambient = float3(0.5f, 0.5f, 0.5f);
		}


		// Transparency in 3D text texture wont further shading (early exit)
		float4 s = texture2d.Sample(ss, input.tex.xy);
		if (s.a < 1.0f)
			return s;

	}
	else // Everything else
	{
		diffuse += GetBoatShading(input);
		ambient = 0.0f;
	}



	if (Are_there_active_events)
	{
		int nr_of_events = GetNrOfEvents();

		for (int i = 0; i < nr_of_events; i++)
		{
			if (
				((input.tex.x + input.tex.y) / 2.0f)
				<
				(1.0f / nr_of_events) * (i + 1)
				)
			{
				ambient = GetEvent(events[i]);
				diffuse = diffuse * 0.0f;
				if (input.tex.x < 0.05f || input.tex.y < 0.05f
					|| input.tex.x > 0.95f || input.tex.y > 0.95f)
				{
					ambient = ambient * 0.2f;
					alpha = 0.8f;
				}
				break;
			}
		}
	}
	else
	{
		//ambient = 0.0f;
	}


	// FOG
	//float depthValue = pow(input.pos.z + 0.01f, 35.0f);
	//
	//float limit = 0.3f;	 // Limit brightness
	//if (depthValue > limit)
	//depthValue = limit;

	return float4(saturate(diffuse + ambient), alpha);
}

float3 GetEvent(int event)
{
	float3 color[4];
	color[0] = float3(1.0f, 0.0f, 0.0f);	// FIRE
	color[1] = float3(0.0f, 1.0f, 0.0f);	// DEATH
	color[2] = float3(0.0f, 0.0f, 1.0f);	// WATER
	color[3] = float3(1.0f, 1.0f, 0.0f);	// CONTAMINATION

	return color[event - 1];
}

int GetNrOfEvents()
{
	int count = 0;

	for (int i = 0; i < 4; i++)
	{
		if (events[i] > 0.0f)
			count++;
	}

	return count;
}


float3 GetBoatShading(PS_IN input)
{
	float3 shading = (float3)0;

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
		shading += lights[i].color * lights[i].illu * 0.65f;
	}

	return shading;
	
}