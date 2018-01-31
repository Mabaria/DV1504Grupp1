struct VertexData {
	float4 pos		: POSITION0;
	float2 texCoord : TEXCOORD0;
};

struct FragmentData {
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

FragmentData main(VertexData input)
{
	FragmentData output = (FragmentData)0;

	output.pos = input.pos;
	output.texCoord = input.texCoord;

	return output;
}