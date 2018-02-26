Texture2D iconTex		: register(t0);
Texture2D numberTex		: register(t1);
sampler ss				: register(s0);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 iconTex : TEXCOORD0;
	float2 numberTex : TEXCOORD1;
};

float4 main(PS_IN input) : SV_TARGET
{
	// Blend both textures together. There may be a built in function
	// to do this, but I'm not sure.
	float4 iconColor = iconTex.Sample(ss, input.iconTex);
	float4 numberColor = numberTex.Sample(ss, input.numberTex);
	return float4(iconColor.xyz * (1.0f - numberColor.w) +
		numberColor.xyz * numberColor.w,
		saturate(iconColor.w + numberColor.w));
}