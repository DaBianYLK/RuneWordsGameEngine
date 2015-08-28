matrix ViewMatrix;
matrix ViewProjectionMatrix;

vector MaterialAmbient;
vector MaterialDiffuse;

vector LightDiffuse;
vector LightAmbient;
vector LightDirection;

struct VS_INPUT
{
	vector position : POSITION;
	vector normal   : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	float2 texCoord : TEXCOORD0;
	vector diffuse  : COLOR0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//
	// Transform position to homogeneous clip space
	// and store in the output.position member. 
	//
	output.position = mul(input.position, ViewProjectionMatrix);

	//
	// Transform lights and normals to view space.  Set w
	// componentes to zero since we're transforming vectors 
	// here and not points.
	//
	LightDirection.w = 0.0f;
	input.normal.w = 0.0f;
	LightDirection = mul(LightDirection, ViewMatrix);
	input.normal = mul(input.normal, ViewMatrix);

	//
	// Compute cosine of the angle between light and normal.
	//
	float s = dot(LightDirection, input.normal);

	//
	// Recall that if the angle between the surface and light
	// is greater than 90 degrees the surface recieves no light.
	// Thus, if the angle is greater than 90 degrees we set
	// s to zero so that the surface will not be lit.
	//
	if (s < 0.0f) {
		s = 0.0f;
	}

	//
	// Ambient light reflected is computed by performing a 
	// component wise multiplication with the ambient material
	// vector and the ambient light intensity vector.
	//
	// Diffuse light reflected is computed by performing a 
	// component wise multiplication with the diffuse material
	// vector and the diffuse light intensity vector.  Further
	// we scale each component by the shading scalar s, which
	// shades the color based on how much light the vertex received
	// from the light source.
	//
	// The sum of both the ambient and diffuse components gives
	// us our final vertex color.
	// 

	output.diffuse = (MaterialAmbient * LightAmbient) +
		(s * (LightDiffuse * MaterialDiffuse));

	output.texCoord = input.texCoord;

	return output;
}