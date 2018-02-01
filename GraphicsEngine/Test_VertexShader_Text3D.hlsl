struct VertexData {
	float4 pos		: POSITION0;
	float2 texCoord : TEXCOORD0;
};

struct PixelData {
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

PixelData main(VertexData input)
{
	PixelData output = (PixelData)0;

	output.pos = input.pos;
	output.texCoord = input.texCoord;

	return output;
}