Texture2D font3D	: register(t0);
sampler ss			: register(s0);

struct PixelData {
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelData input) : SV_TARGET0
{

	float3 rgb = font3D.Sample(ss, input.texCoord.xy);
	float g = rgb.g;

	float blend = rgb.y > 0.0f ? 1.0f : 0.0f;


	if (font3D.Sample(ss, input.texCoord.xy + 0.008f).g == 0.0f)
	{
		blend = 1.0f;
	}

	if (g != 1.0f)
		blend = (blend == 0.0f) ? 0.8f : 0.0f;

	return float4(rgb.bbg + input.texCoord.yxy, blend);
}