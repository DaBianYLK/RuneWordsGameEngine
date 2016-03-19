#include "Material.h"

#include "Shader.h"
#include "ShaderManager.h"

Material::Material() : 
	m_bTwoSided(false), 
	m_fOpacityMaskClipValue(0.0f), 
	m_BlendMode(BM_Opaque), 
	m_ShadingMode(SM_Default), 
	m_bConstantBufferOutOfDate(true), 
	m_uConstantBufferSize(0), 
	m_bTextureListOutOfDate(true), 
	m_bMaterialKeyOutOfDate(true), 
	m_u64MaterialKey(0), 
	m_pShaderType(nullptr)
{
	
}


Material::~Material()
{
}

void Material::UpdateConstantBuffer() const
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

void Material::GetConstantBuffer(unsigned char*& pBuffer, unsigned char& uSize) const
{
	if (m_bConstantBufferOutOfDate)
	{
		UpdateConstantBuffer();
	}

	pBuffer = m_ConstantBuffer;
	uSize = m_uConstantBufferSize;
}

void Material::UpdateTextureInfoList() const
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

const std::list<TextureInfo*>& Material::GetTextureInfoList() const
{
	if (m_bTextureListOutOfDate)
	{
		UpdateTextureInfoList();
	}

	return m_listTextureInfos;
}

void Material::UpdateMaterialKey() const
{
	m_u64MaterialKey = ShaderManager::GetMaterialKey(this);
	m_bMaterialKeyOutOfDate = false;
}

unsigned long long Material::GetMaterialKey() const
{
	if (m_bMaterialKeyOutOfDate)
	{
		UpdateMaterialKey();
	}

	return m_u64MaterialKey;
}

void Material::SetShaderType(ShaderType* pShaderType)
{
	m_pShaderType = pShaderType;
}

ShaderType* Material::GetShaderType() const
{
	return m_pShaderType;
}