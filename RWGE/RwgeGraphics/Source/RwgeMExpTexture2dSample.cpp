#include "RwgeMExpTexture2dSample.h"

MExpTexture2DSample::MExpTexture2DSample() 
	: m_uTexIndex(0)
	, m_pTextureInfo(nullptr)
{
	
}

MExpTexture2DSample::~MExpTexture2DSample()
{
	
}

unsigned char MExpTexture2DSample::GetExpressionID() const
{
	return m_uExpressionID;
}

unsigned MExpTexture2DSample::GetConstantParamSize() const
{
	return 0;
}

void MExpTexture2DSample::GetConstantParamData(void*& pParam, unsigned char& uSize) const
{
	pParam = nullptr;
	uSize = 0;
}

unsigned char MExpTexture2DSample::AddConstantParamToBuffer(void* pBuffer) const
{
	return 0;
}

void MExpTexture2DSample::SetTextureInfo(TextureInfo* pInfo)
{
	m_pTextureInfo = pInfo;
}

TextureInfo* MExpTexture2DSample::GetTextureInfo()
{
	return m_pTextureInfo;
}
