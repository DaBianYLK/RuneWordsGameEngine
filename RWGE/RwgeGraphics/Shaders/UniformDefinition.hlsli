#ifndef __UNIFORM_DEFINITION__
#define __UNIFORM_DEFINITION__

#include "Material.hlsli"
#include "Light.hlsli"
#include "BRDF.hlsli"
#include "SH.hlsli"


struct PrimitiveTransform
{
	matrix matWorld;
	matrix matWorldViewProj;
};

//shared texture		g_EnvironmentCubeMap;
//shared samplerCUBE	g_EnvCubeMapSampler = sampler_state 
//{
//	Texture = <g_EnvironmentCubeMap>;
//	MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//};


// shared�ؼ�������ͨ��Effect Pool�ڲ�ͬ��shader�乲����
shared PrimitiveTransform g_Transform;

// ��Ȼ����������ʹ��float3�Ϳ��ԣ���ʵ����float3Ҳ��Ҫռ��һ��float4�Ĵ�����ֱ�Ӷ���Ϊvector���Ա���CPUִ�����ݶ��룬�Ӷ��������ݴ���
// ToDo����Ҫ�Ա���ɫ����vectorת��Ϊfloat3ʱ�Ƿ����ʹ�ö����ָ��
shared vector g_vecOppositeView;			// ָ���������������ӵ㷢������������ķ�����

// ��Դ����
#if LIGHT_TYPE == DIRECTIONAL_LIGHT
	shared DirectionalLight g_Light;
#elif LIGHT_TYPE == POINT_LIGHT
	shared PointLight		g_Light
#endif

// ���ʳ���
Material g_Material;

// �����壬ShaderModel 3.0֧��ͬʱ��16������
#if TEXTURE_COUNT >= 1
	TEXTURE_DEFINE(g_Texture_1);
#endif

#if TEXTURE_COUNT >= 2
	TEXTURE_DEFINE(g_Texture_2);
#endif

#if TEXTURE_COUNT >= 3
	TEXTURE_DEFINE(g_Texture_3);
#endif

#if TEXTURE_COUNT >= 4
	TEXTURE_DEFINE(g_Texture_4);
#endif

#if TEXTURE_COUNT >= 5
	TEXTURE_DEFINE(g_Texture_5);
#endif

#if TEXTURE_COUNT >= 6
	TEXTURE_DEFINE(g_Texture_6);
#endif

#if TEXTURE_COUNT >= 7
	TEXTURE_DEFINE(g_Texture_7);
#endif

#if TEXTURE_COUNT >= 8
	TEXTURE_DEFINE(g_Texture_8);
#endif

#if TEXTURE_COUNT >= 9
	TEXTURE_DEFINE(g_Texture_9);
#endif

#if TEXTURE_COUNT >= 10
	TEXTURE_DEFINE(g_Texture_10);
#endif

#if TEXTURE_COUNT >= 11
	TEXTURE_DEFINE(g_Texture_11);
#endif

#if TEXTURE_COUNT >= 12
	TEXTURE_DEFINE(g_Texture_12);
#endif

#if TEXTURE_COUNT >= 13
	TEXTURE_DEFINE(g_Texture_13);
#endif

#if TEXTURE_COUNT >= 14
	TEXTURE_DEFINE(g_Texture_14);
#endif

#if TEXTURE_COUNT >= 15
	TEXTURE_DEFINE(g_Texture_15);
#endif

#if TEXTURE_COUNT >= 16
	TEXTURE_DEFINE(g_Texture_16);
#endif


#endif // __UNIFORM_DEFINITION__