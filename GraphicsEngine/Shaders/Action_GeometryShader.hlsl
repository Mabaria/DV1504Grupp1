#define ICON_WIDTH 3
#define ICON_HEIGHT 3

#define TEX_WIDTH 1.0f / (float)ICON_WIDTH
#define TEX_HEIGHT 1.0f / (float)ICON_HEIGHT

#define ICON_SIZE 0.2f

cbuffer GSData : register(b0)
{
	float4 cameraPos;
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

//float4 RotateUV(float2 low, float2 high, uint turns);

[maxvertexcount(4)]
void main(
	point GSInput input[1], 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float4 rightVec;
	float4 upVec;
	uint turn = 3;

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
	//if (dot(cameraPos.xy, input[0].pos.xy) < 0)
	//	turn = 1;

	if (turn == 0)
	{
		rightVec = float4(ICON_SIZE, 0.0f, 0.0f, 0.0f);
		upVec = float4(0.0f, ICON_SIZE * 2.0f, 0.0f, 0.0f);
	}
	else if (turn == 1) {
		rightVec = float4(0.0f, -ICON_SIZE * 2.0f, 0.0f, 0.0f);
		upVec = float4(ICON_SIZE, 0.0f, 0.0f, 0.0f);
	}
	else if (turn == 2) {
		rightVec = float4(-ICON_SIZE, 0.0f, 0.0f, 0.0f);
		upVec = float4(0.0f, -ICON_SIZE * 2.0f, 0.0f, 0.0f);
	}
	else {
		rightVec = float4(0.0f, ICON_SIZE * 2.0f, 0.0f, 0.0f);
		upVec = float4(-ICON_SIZE, 0.0f, 0.0f, 0.0f);
	}

	uint iconData = input[0].data & 15;
	//float texWidth = 1.0f / (float)ICON_WIDTH;
	//float texHeight = 1.0f / (float)ICON_HEIGHT;
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

	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.pos = input[i];
	//	output.Append(element);
	//}
}

//float4 RotateUV(float2 low, float2 high, uint turns)
//{
//	if (turns == 0)
//		return float4(low.xy, high.xy);
//	else if (turns == 1)
//		return float4(high.x, low.y, low.x, high.y);
//	else if (turns == 2)
//		return float4(high.xy, low.xy);
//	else
//		return float4(low.x, high.y, high.x, low.y);
//}