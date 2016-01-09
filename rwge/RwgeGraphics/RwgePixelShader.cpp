#include "RwgePixelShader.h"


RwgePixelShader::RwgePixelShader()
{
	m_hTextureSampler = m_pConstantTable->GetConstantByName(0, "TextureSampler");

	unsigned int count;
	m_pConstantTable->GetConstantDesc(m_hTextureSampler, &m_TextureDesc, &count);
}


RwgePixelShader::~RwgePixelShader()
{
}

void RwgePixelShader::SetTexture(IDirect3DTexture9* pTexture)
{
	m_pDevice->SetTexture(m_TextureDesc.RegisterIndex, pTexture);
}