#include "RwgeMaterial.h"

#include "RwgeD3d9Texture.h"
#include "RwgeD3d9Shader.h"
#include "RwgeD3d9ShaderManager.h"
#include <RwgeMExpConstant.h>

using namespace std;

#define Member(MaterialAttribute)							(m_p##MaterialAttribute)
#define GetMemberConstantCount(MaterialAttribute)			(Member(MaterialAttribute)->GetConstantCount())
#define GetMemberConstants(MaterialAttribute)				(Member(MaterialAttribute)->GetConstants())
#define GetMemberTextureCount(MaterialAttribute)			(Member(MaterialAttribute)->GetTextureCount())
#define GetMemberTextures(MaterialAttribute)				(Member(MaterialAttribute)->GetTextures())
#define GetMemberExpressionID(MaterialAttribute)			(Member(MaterialAttribute)->GetExpressionID())
#define DeleteExpression(MaterialAttribute)					delete Member(MaterialAttribute)
#define SetExpression(MaterialAttribute, pInExpression)		{ DeleteExpression(MaterialAttribute); Member(MaterialAttribute) = (pInExpression); }

#define u16ConstantCount(MaterialAttribute)					(u16##MaterialAttribute##ConstantCount)
#define u8TextureCount(MaterialAttribute)					(u8##MaterialAttribute##TextureCount)

RMaterial::RMaterial() : 
	m_pBaseColor			(new MExpConstantColor()),
	m_pEmissiveColor		(new MExpConstantColor()),
	m_pNormal				(new MExpConstantVector()),
	m_pMetallic				(new MExpConstantScalar()),
	m_pSpecular				(new MExpConstantScalar()),
	m_pRoughness			(new MExpConstantScalar()),
	m_pOpacity				(new MExpConstantScalar()),
	m_pOpacityMask			(new MExpConstantScalar()),
	m_fOpacityMaskClipValue	(0.0f), 
	m_bTwoSided				(false), 
	m_BlendMode				(EBM_Opaque), 
	m_ShadingMode			(ESM_Default), 
	m_u16ConstantCount		(0),
	m_aryConstants			(nullptr),
	m_u8TextureCount		(0),
	m_aryTextures			(nullptr),
	m_pCachedShader			(nullptr)
{

}

RMaterial::~RMaterial()
{
	if (m_aryConstants != nullptr)
	{
		delete[] m_aryConstants;
	}

	if (m_aryTextures != nullptr)
	{
		delete[] m_aryTextures;
	}

	DeleteExpression(BaseColor);
	DeleteExpression(EmissiveColor);
	DeleteExpression(Normal);
	DeleteExpression(Metallic);
	DeleteExpression(Specular);
	DeleteExpression(Roughness);
	DeleteExpression(Opacity);
	DeleteExpression(OpacityMask);
}

void RMaterial::SetBaseColorExpression(MaterialColorExpression* pExpression)
{
	SetExpression(BaseColor, pExpression);
}

void RMaterial::SetEmissiveColorExpression(MaterialColorExpression* pExpression)
{
	SetExpression(EmissiveColor, pExpression);
}

void RMaterial::SetNormalExpression(MaterialVectorExpression* pExpression)
{
	SetExpression(Normal, pExpression);
}

void RMaterial::SetMetallicExpression(MaterialScalarExpression* pExpression)
{
	SetExpression(Metallic, pExpression);
}

void RMaterial::SetSpecularExpression(MaterialScalarExpression* pExpression)
{
	SetExpression(Specular, pExpression);
}

void RMaterial::SetRoughnessExpression(MaterialScalarExpression* pExpression)
{
	SetExpression(Roughness, pExpression);
}

void RMaterial::SetOpacityExpression(MaterialScalarExpression* pExpression)
{
	SetExpression(Opacity, pExpression);
}

void RMaterial::SetOpacityMaskExpression(MaterialScalarExpression* pExpression)
{
	SetExpression(OpacityMask, pExpression);
}

bool RMaterial::IsNonMetal() const
{
	return Member(Metallic)->GetExpressionID() == EME_Constant && (*Member(Metallic)->GetConstants()) == 0.0f;
}

bool RMaterial::IsFullyRough() const
{
	return Member(Roughness)->GetExpressionID() == EME_Constant && (*Member(Roughness)->GetConstants()) == 1.0f;
}

void RMaterial::Update()
{
	// ==================================== 更新常量数据 ====================================
	unsigned short u16ConstantCount(BaseColor)		= GetMemberConstantCount(BaseColor);
	unsigned short u16ConstantCount(EmissiveColor)	= GetMemberConstantCount(EmissiveColor);
	unsigned short u16ConstantCount(Normal)			= GetMemberConstantCount(Normal);
	unsigned short u16ConstantCount(Metallic)		= GetMemberConstantCount(Metallic);
	unsigned short u16ConstantCount(Specular)		= GetMemberConstantCount(Specular);
	unsigned short u16ConstantCount(Roughness)		= GetMemberConstantCount(Roughness);
	unsigned short u16ConstantCount(Opacity)		= GetMemberConstantCount(Opacity);
	unsigned short u16ConstantCount(OpacityMask)	= GetMemberConstantCount(OpacityMask);

	m_u16ConstantCount	= u16ConstantCount(BaseColor)
						+ u16ConstantCount(EmissiveColor)
						+ u16ConstantCount(Normal)
						+ u16ConstantCount(Metallic)
						+ u16ConstantCount(Specular)
						+ u16ConstantCount(Roughness)
						+ u16ConstantCount(Opacity)
						+ u16ConstantCount(OpacityMask);

	if (m_aryConstants != nullptr)
	{
		delete[] m_aryConstants;
	}

	m_aryConstants = new float[m_u16ConstantCount];
	float* pConstantPos = m_aryConstants;

	RwgeCopyMemory(pConstantPos, GetMemberConstants(BaseColor),		u16ConstantCount(BaseColor) * sizeof(float));
	pConstantPos += u16ConstantCount(BaseColor);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(EmissiveColor),	u16ConstantCount(EmissiveColor) * sizeof(float));
	pConstantPos += u16ConstantCount(EmissiveColor);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(Normal),		u16ConstantCount(Normal) * sizeof(float));
	pConstantPos += u16ConstantCount(Normal);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(Metallic),		u16ConstantCount(Metallic) * sizeof(float));
	pConstantPos += u16ConstantCount(Metallic);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(Specular),		u16ConstantCount(Specular) * sizeof(float));
	pConstantPos += u16ConstantCount(Specular);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(Roughness),		u16ConstantCount(Roughness) * sizeof(float));
	pConstantPos += u16ConstantCount(Roughness);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(Opacity),		u16ConstantCount(Opacity) * sizeof(float));
	pConstantPos += u16ConstantCount(Opacity);

	RwgeCopyMemory(pConstantPos, GetMemberConstants(OpacityMask),	u16ConstantCount(OpacityMask) * sizeof(float));

	// ==================================== 更新纹理数据 ====================================
	unsigned short u8TextureCount(BaseColor) = GetMemberTextureCount(BaseColor);
	unsigned short u8TextureCount(EmissiveColor) = GetMemberTextureCount(EmissiveColor);
	unsigned short u8TextureCount(Normal) = GetMemberTextureCount(Normal);
	unsigned short u8TextureCount(Metallic) = GetMemberTextureCount(Metallic);
	unsigned short u8TextureCount(Specular) = GetMemberTextureCount(Specular);
	unsigned short u8TextureCount(Roughness) = GetMemberTextureCount(Roughness);
	unsigned short u8TextureCount(Opacity) = GetMemberTextureCount(Opacity);
	unsigned short u8TextureCount(OpacityMask) = GetMemberTextureCount(OpacityMask);

	// 将所有的材质属性表达式的纹理收集到一个纹理队列中
	unsigned short u16TextureCount	= u8TextureCount(BaseColor)
									+ u8TextureCount(EmissiveColor)
									+ u8TextureCount(Normal)
									+ u8TextureCount(Metallic)
									+ u8TextureCount(Specular)
									+ u8TextureCount(Roughness)
									+ u8TextureCount(Opacity)
									+ u8TextureCount(OpacityMask);

	RD3d9Texture** aryTextures = new RD3d9Texture*[u16TextureCount];
	RD3d9Texture** pTexturePos = aryTextures;

	RwgeCopyMemory(pTexturePos, GetMemberTextures(BaseColor), u8TextureCount(BaseColor) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(BaseColor);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(EmissiveColor), u8TextureCount(EmissiveColor) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(EmissiveColor);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(Normal), u8TextureCount(Normal) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(Normal);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(Metallic), u8TextureCount(Metallic) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(Metallic);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(Specular), u8TextureCount(Specular) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(Specular);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(Roughness), u8TextureCount(Roughness) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(Roughness);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(Opacity), u8TextureCount(Opacity) * sizeof(RD3d9Texture*));
	pTexturePos += u8TextureCount(Opacity);

	RwgeCopyMemory(pTexturePos, GetMemberTextures(OpacityMask), u8TextureCount(OpacityMask) * sizeof(RD3d9Texture*));

	// 整理纹理队列中重复的纹理
	RTexturesToTextureUnitsMap texturesToTextureUnitsMap(u16TextureCount);
	unsigned char u8TextureUnitIndex = 0;
	map<RD3d9Texture*, unsigned char> mapTextureToTextureUnit;

	for (unsigned char i = 0; i < u16TextureCount; ++i)
	{
		map<RD3d9Texture*, unsigned char>::_Pairib result = mapTextureToTextureUnit.insert(make_pair(aryTextures[i], u8TextureUnitIndex));

		// 如果插入成功，则为纹理分配一个新的纹理单元
		if (result.second)
		{
			texturesToTextureUnitsMap[i] = u8TextureUnitIndex;
			++u8TextureUnitIndex;
		}
		// 如果插入失败，则与重复的纹理共用纹理单元
		else
		{
			texturesToTextureUnitsMap[i] = result.first->second;
		}
	}

	// 将整理后的纹理储存到纹理队列中
	m_u8TextureCount = mapTextureToTextureUnit.size();
	RwgeAssert(m_u8TextureCount <= 16);		// Shader Model 3.0 支持同时绑定的纹理数量上限为16

	if (m_aryTextures != nullptr)
	{
		delete[] m_aryTextures;
	}

	m_aryTextures = new RD3d9Texture*[m_u8TextureCount];
	for (pair<RD3d9Texture* const, unsigned char>& pairTextureToTextureUnit : mapTextureToTextureUnit)
	{
		m_aryTextures[pairTextureToTextureUnit.second] = pairTextureToTextureUnit.first;
	}

	// 将纹理与纹理单元的映射关系数组存储到表中，就算数组为空，也会得到一个有效的HashKey
	unsigned int u32Hash = RShaderKey::InsertTexturesToTextureUnitsMap(texturesToTextureUnitsMap);

	delete[] aryTextures;

	// ==================================== 更新MaterialKey ====================================
	m_MaterialKey.SetBaseColorKey(GetMemberExpressionID(BaseColor));
	m_MaterialKey.SetEmissiveColorKey(GetMemberExpressionID(EmissiveColor));
	m_MaterialKey.SetNormalKey(GetMemberExpressionID(Normal));
	m_MaterialKey.SetMetallicKey(GetMemberExpressionID(Metallic));
	m_MaterialKey.SetSpecularKey(GetMemberExpressionID(Specular));
	m_MaterialKey.SetRoughnessKey(GetMemberExpressionID(Roughness));
	m_MaterialKey.SetOpacityKey(GetMemberExpressionID(Opacity));
	m_MaterialKey.SetOpacityMaskKey(GetMemberExpressionID(OpacityMask));
	m_MaterialKey.SetBlendModeKey(m_BlendMode);
	m_MaterialKey.SetShadingModeKey(m_ShadingMode);
	m_MaterialKey.SetTwoSidedKey(m_bTwoSided);
	m_MaterialKey.SetNonMetalKey(IsNonMetal());
	m_MaterialKey.SetFullyRoughKey(IsFullyRough()); 
	m_MaterialKey.SetTextureCountKey(m_u8TextureCount);
	m_MaterialKey.SetTextureMapHashKey(u32Hash);
}