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
#define EXP_CONSTANT					0
#define EXP_TEXTURE2D_SAMPLE			1

// 材质混合模式
#define	BLENDING_OPAQUE					0
#define BLENDING_TRANSLUCENT			1
#define BLENDING_ADDITIVE				2
#define BLENDING_MODULATE				3
#define BLENDING_MASKED					4

/* 由脚本设置的属性：
材质常量属性标志（用于简化着色器）
MATERIAL_NONMETAL
MATERIAL_FULLY_ROUGH
*/

/////////////////////////////////////////////////////////////////////
// 常量材质
struct MaterialConstant
{
#if BASE_COLOR_EXPRESSION == EXP_CONSTANT
	float3	baseColor;
#endif

#ifdef EMISSIVE_COLOR_CONSTANT
	float3	emissiveColor;
#endif

#ifdef NORMAL_CONSTANT
	float3	normal;
#endif

#ifdef METALLIC_CONSTANT
	float	metallic;
#endif

#ifdef SPECULAR_CONSTANT
	float	specular;
#endif

#ifdef ROUGHNESS_CONSTANT
	float	roughness;
#endif

#ifdef OPACITY_CONSTANT
	float	opacity;
#endif

#ifdef OPACITY_MASK_CONSTANT
	float	opacityMask;
#endif
};

MaterialConstant g_Material;

/////////////////////////////////////////////////////////////////////
// 纹理材质
#ifdef BASE_COLOR_TEXTURE
TEXTURE_DEFINE(g_BaseColorTexture);
#endif

#ifdef EMISSIVE_COLOR_TEXTURE
TEXTURE_DEFINE(g_EmissiveColorTexture);
#endif

#ifdef NORMAL_TEXTURE
TEXTURE_DEFINE(g_NormalTexture);
#endif

#ifdef METALLIC_TEXTURE
TEXTURE_DEFINE(g_MetallicTexture);
#endif

#ifdef SPECULAR_TEXTURE
TEXTURE_DEFINE(g_SpecularTexture);
#endif

#ifdef ROUGHNESS_TEXTURE
TEXTURE_DEFINE(g_RoughnessTexture);
#endif

#ifdef OPACITY_TEXTURE
TEXTURE_DEFINE(g_OpacityTexture);
#endif

#ifdef OPACITY_MASK_TEXTURE
TEXTURE_DEFINE(g_OpacityMaskTexture);
#endif

/////////////////////////////////////////////////////////////////////
// 获取材质属性
float3 GetBaseColor(float2 texCoord)
{
#ifdef BASE_COLOR_CONSTANT
	return	g_Material.baseColor;
#endif

#ifdef BASE_COLOR_TEXTURE
	return (float3)TEXTURE_SAMPLE(g_BaseColorTexture, texCoord);
#endif
}

float3 GetEmissiveColor(float2 texCoord)
{
#ifdef EMISSIVE_COLOR_CONSTANT
	return	g_Material.emissiveColor;
#endif

#ifdef EMISSIVE_COLOR_TEXTURE
	return (float3)TEXTURE_SAMPLE(g_EmissiveColorTexture, texCoord);
#endif
}

float3 GetNormal(float2 texCoord)
{
#ifdef NORMAL_CONSTANT
	return	g_Material.normal;
#endif

#ifdef NORMAL_TEXTURE
	return (float3)TEXTURE_SAMPLE(g_NormalTexture, texCoord);
#endif
}

float GetMetallic(float2 texCoord)
{
#ifdef METALLIC_CONSTANT
	return	g_Material.metallic;
#endif

#ifdef METALLIC_TEXTURE
	return (float)TEXTURE_SAMPLE(g_MetallicTexture, texCoord);
#endif
}

float GetSpecular(float2 texCoord)
{
#ifdef SPECULAR_CONSTANT
	return	g_Material.specular;
#endif

#ifdef SPECULAR_TEXTURE
	return (float)TEXTURE_SAMPLE(g_SpecularTexture, texCoord);
#endif
}

float GetRoughness(float2 texCoord)
{
#ifdef ROUGHNESS_CONSTANT
	return	g_Material.roughness;
#endif

#ifdef ROUGHNESS_TEXTURE
	return (float)TEXTURE_SAMPLE(g_RoughnessTexture, texCoord);
#endif
}

float GetOpacity(float2 texCoord)
{
#ifdef OPACITY_CONSTANT
	return	g_Material.opacity;
#endif

#ifdef OPACITY_TEXTURE
	return (float)TEXTURE_SAMPLE(g_OpacityTexture, texCoord);
#endif
}

float GetOpacityMask(float2 texCoord)
{
#ifdef OPACITY_MASK_CONSTANT
	return	g_Material.opacityMask;
#endif

#ifdef OPACITY_MASK_TEXTURE
	return (float)TEXTURE_SAMPLE(g_OpacityMaskTexture, texCoord);
#endif
}


#endif // __MATERIAL__