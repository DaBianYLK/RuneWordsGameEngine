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


// shared关键字用于通过Effect Pool在不同的shader间共享常量
shared PrimitiveTransform g_Transform;

// 虽然反视线向量使用float3就可以，但实际上float3也需要占用一个float4寄存器，直接定义为vector可以避免CPU执行数据对齐，从而加速数据传递
// ToDo：需要对比着色器将vector转换为float3时是否会有使用额外的指令
shared vector g_vecOppositeView;			// 指向视线向量（从视点发射出的向量）的反方向

// 光源常量
#if LIGHT_TYPE == DIRECTIONAL_LIGHT
	shared DirectionalLight g_Light;
#elif LIGHT_TYPE == POINT_LIGHT
	shared PointLight		g_Light
#endif

// 材质常量
Material g_Material;

// 纹理定义，ShaderModel 3.0支持同时绑定16张纹理
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