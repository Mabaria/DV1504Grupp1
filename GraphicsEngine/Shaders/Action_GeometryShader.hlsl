#define ICON_WIDTH 3
#define ICON_HEIGHT 3

#define TEX_WIDTH 1.0f / (float)ICON_WIDTH
#define TEX_HEIGHT 1.0f / (float)ICON_HEIGHT

#define ICON_SIZE 0.04f

cbuffer GSData : register(b0)
{
	float3 cameraPos;
	uint cameraData;
}

struct GSInput
{
	float4 pos : SV_POSITION;
	float4 wpos : POSITION;
	uint data : DATA;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

[maxvertexcount(4)]
void main(
	point GSInput input[1], 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float4 rightVec;
	float4 upVec;
	uint turn = (input[0].data >> 4) & 3;

	if (cameraData == 1)
	{
		float2 dir = normalize(cameraPos.xz - input[0].wpos.xz);
		float2 dir2 = normalize(float2(1.0f, 0.0f));
		for (uint i = 1; i < 4; i++)
		{
			if (dot(dir, dir2) > 0.707f)
			{
				turn = (turn - i) % 4;
				break;
			}
			dir2 = float2(dir2.y, -dir2.x);
		}
	}

	float size;
	if (input[0].pos.z < 3.f)
		size = 0.12f;
	else
		size = ICON_SIZE * input[0].pos.z;

	if (turn == 0)
	{
		rightVec = float4(size, 0.0f, 0.0f, 0.0f);
		upVec = float4(0.0f, size * 2.0f, 0.0f, 0.0f);
	}
	else if (turn == 1) {
		rightVec = float4(0.0f, -size * 2.0f, 0.0f, 0.0f);
		upVec = float4(size, 0.0f, 0.0f, 0.0f);
	}
	else if (turn == 2) {
		rightVec = float4(-size, 0.0f, 0.0f, 0.0f);
		upVec = float4(0.0f, -size * 2.0f, 0.0f, 0.0f);
	}
	else {
		rightVec = float4(0.0f, size * 2.0f, 0.0f, 0.0f);
		upVec = float4(-size, 0.0f, 0.0f, 0.0f);
	}

	uint iconData = input[0].data & 15;
	float2 texRectLow = float2(
		(float)(iconData % ICON_WIDTH) * TEX_WIDTH,
		(float)(iconData / ICON_HEIGHT) * TEX_HEIGHT);
	float2 texRectHigh = float2(
		texRectLow.x + TEX_WIDTH,
		texRectLow.y + TEX_HEIGHT);

	float4 UV = float4(texRectLow.xy, texRectHigh.xy);


	element.pos = input[0].pos - rightVec + upVec;
	element.tex = float2(UV.x, UV.y);
	output.Append(element);
	element.pos = input[0].pos + rightVec + upVec;
	element.tex = float2(UV.z, UV.y);
	output.Append(element);
	element.pos = input[0].pos - rightVec - upVec;
	element.tex = float2(UV.x, UV.w);
	output.Append(element);
	element.pos = input[0].pos + rightVec - upVec;
	element.tex = float2(UV.z, UV.w);
	output.Append(element);
}