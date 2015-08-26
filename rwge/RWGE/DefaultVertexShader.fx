matrix ViewMatrix;
matrix ViewProjMatrix;

vector AmbientMtrl;
vector DiffuseMtrl;

vector LightDirection;

vector DiffuseLightIntensity = { 0.0f, 0.0f, 1.0f, 1.0f };
vector AmbientLightIntensity = { 0.0f, 0.0f, 0.2f, 1.0f };

struct VS_INPUT
{
	vector position : POSITION;
	vector normal   : NORMAL;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse  : COLOR;
};

VS_OUTPUT Main(VS_INPUT input)
{
	// zero out all members of the output instance.
	VS_OUTPUT output = (VS_OUTPUT)0;

	//
	// Transform position to homogeneous clip space
	// and store in the output.position member. 
	//
	output.position = mul(input.position, ViewProjMatrix);

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
	if (s < 0.0f)
		s = 0.0f;

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

	output.diffuse = (AmbientMtrl * AmbientLightIntensity) +
		(s * (DiffuseLightIntensity * DiffuseMtrl));

	return output;
}