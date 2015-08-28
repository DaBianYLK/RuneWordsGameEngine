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

	// �Թ����뷨����ִ����ͼ�任
	LightDirection.w = 0.0f;
	input.normal.w = 0.0f;
	LightDirection = mul(LightDirection, ViewMatrix);
	input.normal = mul(input.normal, ViewMatrix);

	// ������߷����붥�㷨�߷���ļн�����ֵ����ķ��������ͷ����������ǵ�λ������
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