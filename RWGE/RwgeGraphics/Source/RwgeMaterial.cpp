#include "RwgeMaterial.h"

#include "RwgeShader.h"
#include "RwgeShaderManager.h"

RMaterial::RMaterial() : 
	m_bTwoSided(false), 
	m_fOpacityMaskClipValue(0.0f), 
	m_BlendMode(BM_Opaque), 
	m_ShadingMode(SM_Default), 
	m_bConstantBufferOutOfDate(true), 
	m_uConstantBufferSize(0), 
	m_bTextureListOutOfDate(true), 
	m_bMaterialKeyOutOfDate(true),
	m_pShaderType(nullptr)
{
	
}


RMaterial::~RMaterial()
{
}

void RMaterial::UpdateConstantBuffer() const
{
	m_uConstantBufferSize = 0;

	m_uConstantBufferSize += m_BaseColor.AddConstantParamToBuffer(m_ConstantBuffer);
	m_uConstantBufferSize += m_EmissiveColor.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_Normal.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_Metallic.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_Specular.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_Roughness.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_Opacity.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);
	m_uConstantBufferSize += m_OpacityMask.AddConstantParamToBuffer(m_ConstantBuffer + m_uConstantBufferSize);

	m_bConstantBufferOutOfDate = false;
}

void RMaterial::GetConstantBuffer(unsigned char*& pBuffer, unsigned char& uSize) const
{
	if (m_bConstantBufferOutOfDate)
	{
		UpdateConstantBuffer();
	}

	pBuffer = m_ConstantBuffer;
	uSize = m_uConstantBufferSize;
}

void RMaterial::UpdateTextureInfoList() const
{
	m_listTextureInfos.clear();

	TextureInfo* pTextureInfo = m_BaseColor.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_EmissiveColor.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_Normal.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_Metallic.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_Specular.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_Roughness.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_Opacity.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	pTextureInfo = m_OpacityMask.GetTextureInfo();
	if (pTextureInfo != nullptr)	m_listTextureInfos.push_back(pTextureInfo);

	m_bTextureListOutOfDate = false;
}

const std::list<TextureInfo*>& RMaterial::GetTextureInfoList() const
{
	if (m_bTextureListOutOfDate)
	{
		UpdateTextureInfoList();
	}

	return m_listTextureInfos;
}

void RMaterial::UpdateMaterialKey() const
{
	m_MaterialKey.SetBaseColorKey(m_BaseColor.GetExpressionID());
	m_MaterialKey.SetEmissiveColorKey(m_EmissiveColor.GetExpressionID());
	m_MaterialKey.SetNormalKey(m_Normal.GetExpressionID());
	m_MaterialKey.SetMetallicKey(m_Metallic.GetExpressionID());
	m_MaterialKey.SetSpecularKey(m_Specular.GetExpressionID());
	m_MaterialKey.SetRoughnessKey(m_Roughness.GetExpressionID());
	m_MaterialKey.SetOpacityKey(m_Opacity.GetExpressionID());
	m_MaterialKey.SetOpacityMaskKey(m_OpacityMask.GetExpressionID());
	m_MaterialKey.SetBlendModeKey(m_BlendMode);
	m_MaterialKey.SetShadingModeKey(m_ShadingMode);
	m_MaterialKey.SetTwoSidedKey(m_bTwoSided);
	m_MaterialKey.SetNonMetalKey(IsNonMetal());
	m_MaterialKey.SetFullyRoughKey(IsFullyRough());

	m_bMaterialKeyOutOfDate = false;
}

const MaterialKey& RMaterial::GetMaterialKey() const
{
	if (m_bMaterialKeyOutOfDate)
	{
		UpdateMaterialKey();
	}

	return m_MaterialKey;
}

void RMaterial::SetShaderType(ShaderType* pShaderType)
{
	m_pShaderType = pShaderType;
}

ShaderType* RMaterial::GetShaderType() const
{
	return m_pShaderType;
}

bool RMaterial::IsNonMetal() const
{
	return m_Metallic.GetExpression() == nullptr && m_Metallic.GetConstant() == 0.0f;
}

bool RMaterial::IsFullyRough() const
{
	return m_Roughness.GetExpression() == nullptr && m_Roughness.GetConstant() == 1.0f;
}
