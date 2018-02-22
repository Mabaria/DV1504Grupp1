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

cbuffer HOVER_BUFFER : register(b2)
{
	bool is_hover_effect_active;
}

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float3 worldPos : POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD;
};

int GetNrOfEvents();
bool IsInsideStep(float2 texCoord, int step, int nr_of_events);
bool IsAtEdge(float2 texCoord);
float3 GetEvent(int event);

float4 main(PS_IN input) : SV_TARGET
{
	float alpha = Opacity;
	float3 ambient = Ambient;
	float3 diffuse = Diffuse;

	// If events are active for a bounding box
	bool are_there_active_events = events.r > 0.0f ?
		true : false;


	if (are_there_active_events)
	{
		int nr_of_events = GetNrOfEvents();
		alpha = 0.5f;

		for (int i = 0; i < nr_of_events; i++)
		{
			if (IsInsideStep(input.tex, i, nr_of_events))
			{
				ambient = GetEvent(events[i]);
				diffuse = diffuse * 0.0f;
				
				break;
			}
		}

		if (IsAtEdge(input.tex))
		{
			diffuse -= 0.8f;
			alpha = 0.3f;
		}
	}
	else
	{
		alpha = 0.0f;
	}

	if (is_hover_effect_active)
	{
		if (IsAtEdge(input.tex))
		{
			ambient = 0.0f;
			diffuse = float4(0.4f, 0.0f, 0.0f, 1.0f);
			alpha = 1.0f;
		}
	}

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

bool IsInsideStep(float2 texCoord, int step, int nr_of_events)
{
	return 
		((texCoord.x + texCoord.y) / 2.0f) 
		< 
		((1.0f / nr_of_events) * (step + 1));
}

bool IsAtEdge(float2 texCoord)
{
	return (texCoord.x < 0.05f || texCoord.y < 0.05f
		|| texCoord.x > 0.95f || texCoord.y > 0.95f);
}