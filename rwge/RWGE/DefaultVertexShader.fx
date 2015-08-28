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

	// 对光线与法向量执行视图变换
	LightDirection.w = 0.0f;
	input.normal.w = 0.0f;
	LightDirection = mul(LightDirection, ViewMatrix);
	input.normal = mul(input.normal, ViewMatrix);

	// 计算光线方向与顶点法线方向的夹角余弦值（光的方向向量和法线向量都是单位向量）
	float cos = dot(LightDirection, input.normal);
	if (cos > 0.0f) {
		cos = 0.0f;
	}
	else {
		cos = -cos;
	}
	output.diffuse = (MaterialAmbient * LightAmbient) + (cos * (MaterialDiffuse * LightDiffuse));
	output.texCoord = input.texCoord;

	return output;
}