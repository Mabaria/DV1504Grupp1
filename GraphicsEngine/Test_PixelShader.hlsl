struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	float4 color = float4((input.pos.x + 1.0f) / 2.0f, (input.pos.y + 1.0f) / 2.0f ,0.0f,1.0f);
	//color *= dot(color, float4(input.nor, 1.0f));
	return color;
}