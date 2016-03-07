#include "Material.h"

#include "ShaderManager.h"

Material::Material()
	: m_bTwoSided(false)
	, m_fOpacityMaskClipValue(0.0f)
	, m_BlendMode(BM_Opaque)
	, m_ShadingMode(SM_Default)
	, m_bConstantBufferOutOfDate(true)
	, m_uConstantBufferSize(0)
	, m_bTextureListOutOfDate(true)
	, m_bMaterialKeyOutOfDate(true)
	, m_u64MaterialKey(0)
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

void Material::UpdateTextureList() const
{
	m_listTextures.clear();

	Texture* pTexture = m_BaseColor.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_EmissiveColor.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_Normal.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_Metallic.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_Specular.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_Roughness.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_Opacity.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	pTexture = m_OpacityMask.GetTexture();
	if (pTexture != nullptr)	m_listTextures.push_back(pTexture);

	m_bTextureListOutOfDate = false;
}

const std::list<Texture*>& Material::GetTextureList() const
{
	if (m_bTextureListOutOfDate)
	{
		UpdateTextureList();
	}

	return m_listTextures;
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
