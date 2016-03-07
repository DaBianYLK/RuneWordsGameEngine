#ifndef __MATERIAL__
#define __MATERIAL__

#include "Texture.hlsli"

/*
材质基本属性
BaseColor
EmissiveColor
Normal
Metallic
Specular
Roughness
Opacity	
OpacityMask
*/

// 材质属性表达式，暂时分配4个字节表示，即32种表达式，编号0-31
// 对应XXX_EXPRESSION，其中XXX为材质的基本属性
#define EXP_CONSTANT					0
#define EXP_TEXTURE2D_SAMPLE			1

// 材质混合模式（需要与EBlendMode对应），对应MATERIAL_BLEND_MODE
#define	BLENDING_OPAQUE					0
#define BLENDING_TRANSLUCENT			1
#define BLENDING_ADDITIVE				2
#define BLENDING_MODULATE				3
#define BLENDING_MASKED					4

// 材质着色模式（需要与EShadingModel对应），对应MATERIAL_SHADING_MODE
#define SHADING_UNLIT					0
#define SHADING_DEFAULT					1

/* 由脚本设置的属性：
材质常量属性标志（用于简化着色器）
MATERIAL_NONMETAL
MATERIAL_FULLY_ROUGH

// 渲染状态相关
MATERIAL_TWO_SIDED
*/

/////////////////////////////////////////////////////////////////////
// 常量材质
struct MaterialConstant
{
#if BASE_COLOR_EXPRESSION == EXP_CONSTANT
	float3	baseColor;
#endif

#if EMISSIVE_COLOR_EXPRESSION == EXP_CONSTANT
	float3	emissiveColor;
#endif

#if NORMAL_EXPRESSION == EXP_CONSTANT
	float3	normal;
#endif

#if METALLIC_EXPRESSION == EXP_CONSTANT
	float	metallic;
#endif

#if SPECULAR_EXPRESSION == EXP_CONSTANT
	float	specular;
#endif

#if ROUGHNESS_EXPRESSION == EXP_CONSTANT
	float	roughness;
#endif

#if OPACITY_EXPRESSION == EXP_CONSTANT
	float	opacity;
#endif

#if OPACITY_MASK_EXPRESSION == EXP_CONSTANT
	float	opacityMask;
#endif
};

MaterialConstant g_Material;

/////////////////////////////////////////////////////////////////////
// 纹理材质
#if BASE_COLOR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_BaseColorTexture);
#endif

#if EMISSIVE_COLOR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_EmissiveColorTexture);
#endif

#if NORMAL_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_NormalTexture);
#endif

#if METALLIC_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_MetallicTexture);
#endif

#if SPECULAR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_SpecularTexture);
#endif

#if ROUGHNESS_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_RoughnessTexture);
#endif

#if OPACITY_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_OpacityTexture);
#endif

#if OPACITY_MASK_EXPRESSION == EXP_TEXTURE2D_SAMPLE
TEXTURE_DEFINE(g_OpacityMaskTexture);
#endif

/////////////////////////////////////////////////////////////////////
// 获取材质属性
float3 GetBaseColor(float2 texCoord)
{
#if BASE_COLOR_EXPRESSION == EXP_CONSTANT
	return	g_Material.baseColor;
#elif BASE_COLOR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float3)TEXTURE_SAMPLE(g_BaseColorTexture, texCoord);
#endif
}

float3 GetEmissiveColor(float2 texCoord)
{
#if EMISSIVE_COLOR_EXPRESSION == EXP_CONSTANT
	return	g_Material.emissiveColor;
#elif EMISSIVE_COLOR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float3)TEXTURE_SAMPLE(g_EmissiveColorTexture, texCoord);
#endif
}

float3 GetNormal(float2 texCoord)
{
#if NORMAL_EXPRESSION == EXP_CONSTANT
	return	g_Material.normal;
#elif NORMAL_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float3)TEXTURE_SAMPLE(g_NormalTexture, texCoord);
#endif
}

float GetMetallic(float2 texCoord)
{
#if METALLIC_EXPRESSION == EXP_CONSTANT
	return	g_Material.metallic;
#elif METALLIC_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float)TEXTURE_SAMPLE(g_MetallicTexture, texCoord);
#endif
}

float GetSpecular(float2 texCoord)
{
#if SPECULAR_EXPRESSION == EXP_CONSTANT
	return	g_Material.specular;
#elif SPECULAR_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float)TEXTURE_SAMPLE(g_SpecularTexture, texCoord);
#endif
}

float GetRoughness(float2 texCoord)
{
#if ROUGHNESS_EXPRESSION == EXP_CONSTANT
	return	g_Material.roughness;
#elif ROUGHNESS_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float)TEXTURE_SAMPLE(g_RoughnessTexture, texCoord);
#endif
}

float GetOpacity(float2 texCoord)
{
#if OPACITY_EXPRESSION == EXP_CONSTANT
	return	g_Material.opacity;
#elif OPACITY_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float)TEXTURE_SAMPLE(g_OpacityTexture, texCoord);
#endif
}

float GetOpacityMask(float2 texCoord)
{
#if OPACITY_MASK_EXPRESSION == EXP_CONSTANT
	return	g_Material.opacityMask;
#elif OPACITY_MASK_EXPRESSION == EXP_TEXTURE2D_SAMPLE
	return (float)TEXTURE_SAMPLE(g_OpacityMaskTexture, texCoord);
#endif
}


#endif // __MATERIAL__