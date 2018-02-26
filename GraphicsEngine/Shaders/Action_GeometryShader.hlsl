#define ICON_WIDTH 3
#define ICON_HEIGHT 3

#define TEX_WIDTH 1.0f / (float)ICON_WIDTH
#define TEX_HEIGHT 1.0f / (float)ICON_HEIGHT

#define ICON_SIZE 0.025f

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
	float2 iconTex : TEXCOORD0;
	float2 numberTex : TEXCOORD1;
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
	// Retrieves the turn data from the data chunk
	uint turn = (input[0].data >> 4) & 7;

	// If we're not paning and the icon is not stationary, rotate icon
	// depending on what direction the camera is looking
	if (cameraData == 1 && turn != 4)
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

	// This section is a test to base the size of the action icon
	// on the Z-distance of the camera. Will most likely be scrapped to
	// make picking easier and more manageble.
	float size;
	if (input[0].pos.z < 3.f)
		size = 0.07f;
	else
		size = ICON_SIZE * input[0].pos.z;

	// Define up and right vectors depending on rotation
	if (turn == 0 || turn == 4)
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

	// Extract what icon image should be displayed, then generate the UV
	// coordinates for the pixel shader stage.
	uint iconData = input[0].data & 15;
	float2 iconTexRectLow = float2(
		(float)(iconData % ICON_WIDTH) * TEX_WIDTH,
		(float)(iconData / ICON_HEIGHT) * TEX_HEIGHT);
	float2 iconTexRectHigh = float2(
		iconTexRectLow.x + TEX_WIDTH,
		iconTexRectLow.y + TEX_HEIGHT);

	float4 iconUV = float4(iconTexRectLow.xy, iconTexRectHigh.xy);

	// Same as above but for numbers. In case no number should be displayed,
	// have the UVs to be 0.0f.
	float4 numberUV;
	iconData = (input[0].data >> 9) & 15;
	if (iconData == 0)
		numberUV = float4(0.0f, 0.0f, 0.0f, 0.0f);
	else
	{
		iconData--;
		iconTexRectLow = float2(
			(float)(iconData % ICON_WIDTH) * TEX_WIDTH,
			(float)(iconData / ICON_HEIGHT) * TEX_HEIGHT);
		iconTexRectHigh = float2(
			iconTexRectLow.x + TEX_WIDTH,
			iconTexRectLow.y + TEX_HEIGHT);

		numberUV = float4(iconTexRectLow.xy, iconTexRectHigh.xy);
	}

	// Puzzle all info together into four vertices, making a quad
	element.pos = input[0].pos - rightVec + upVec;
	element.iconTex = float2(iconUV.x, iconUV.y);
	element.numberTex = float2(numberUV.x, numberUV.y);
	output.Append(element);
	element.pos = input[0].pos + rightVec + upVec;
	element.iconTex = float2(iconUV.z, iconUV.y);
	element.numberTex = float2(numberUV.z, numberUV.y);
	output.Append(element);
	element.pos = input[0].pos - rightVec - upVec;
	element.iconTex = float2(iconUV.x, iconUV.w);
	element.numberTex = float2(numberUV.x, numberUV.w);
	output.Append(element);
	element.pos = input[0].pos + rightVec - upVec;
	element.iconTex = float2(iconUV.z, iconUV.w);
	element.numberTex = float2(numberUV.z, numberUV.w);
	output.Append(element);
}