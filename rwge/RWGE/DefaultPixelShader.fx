sampler TextureSampler;

struct PS_INPUT
{
	vector surfaceColor		: COLOR0; 
	vector specularColor	: COLOR1;
	float2 texCoord			: TEXCOORD0;
};

struct PS_OUTPUT
{
	vector surfaceColor : COLOR0;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.surfaceColor = tex2D(TextureSampler, input.texCoord) * input.surfaceColor + input.specularColor;

	return output;
}