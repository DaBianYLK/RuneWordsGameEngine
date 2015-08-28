sampler TextureSampler;

struct PS_INPUT
{
	float2 texCoord	: TEXCOORD0;
	vector light	: COLOR0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.diffuse = tex2D(TextureSampler, input.texCoord) * input.light;

	return output;
}