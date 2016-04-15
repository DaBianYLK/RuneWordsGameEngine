struct D3D9Material
{
	vector	diffuse;	/* Diffuse color RGBA */
	vector	ambient;	/* Ambient color RGB */
	vector	specular;	/* Specular 'shininess' */
	vector	emissive;	/* Emissive color RGB */
	float	power;		/* Sharpness if specular highlight */
};

struct D3D9Light
{
	int		type;             /* Type of light source */
	vector	diffuse;          /* Diffuse color of light */
	vector	specular;         /* Specular color of light */
	vector	ambient;          /* Ambient color of light */
	float3	position;         /* Position in world space */
	float3	direction;        /* Direction in world space */	// �˴��ķ������û�����Ĺ��߷����෴
	float	range;            /* Cutoff range */
	float	falloff;          /* Falloff */
	float	attenuation0;     /* Constant attenuation */
	float	attenuation1;     /* Linear attenuation */
	float	attenuation2;     /* Quadratic attenuation */
	float	theta;            /* Inner angle of spotlight cone */
	float	phi;              /* Outer angle of spotlight cone */
};

matrix g_ViewMatrix;
matrix g_WorldMatrix;
matrix g_WvpMatrix;			// World View Projection Matrix

D3D9Material	g_Material;
D3D9Light		g_Light;

struct VS_INPUT
{
	vector position : POSITION;
	vector normal   : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	vector position			: POSITION;
	vector sufaceColor		: COLOR0;
	vector specularColor	: COLOR1;
	float2 texCoord			: TEXCOORD0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, g_WvpMatrix);
	output.texCoord = input.texCoord;

	// ���㷨�߱任
	input.normal.w = 0.0f;
	input.normal = mul(input.normal, g_WorldMatrix);

	float3 viewVector = normalize((float3)(g_ViewMatrix[3]));

	// ������߷����붥�㷨�߷���ļн�����ֵ����ķ��������ͷ����������ǵ�λ������
	float dotNL = max(0.0f, dot(g_Light.direction, (float3)input.normal));
	float dotNH = max(dot((float3)input.normal, viewVector), 0.0f);
	output.sufaceColor
		= g_Material.emissive									// ���� Emissive
		+ (g_Material.ambient * g_Light.ambient)				// ����	Ambient
		+ (g_Material.diffuse * g_Light.diffuse * dotNL);		// ���� Diffuse
	//+ (g_Material.specular * g_Light.specular * (dotNL > 0.0f ? 1.0f : 0.0f) * pow(dotNH, g_Material.power));	// ���� Specular

	output.specularColor = g_Material.specular * g_Light.specular * (dotNL > 0.0f ? 1.0f : 0.0f) * pow(dotNH, g_Material.power);

	return output;
}