#include "ME_Texture2DSample.h"

Texture2DSample::Texture2DSample() 
	: m_pTexture(nullptr)
	, m_uTexIndex(0)
{
	
}

Texture2DSample::~Texture2DSample()
{
	
}

unsigned char Texture2DSample::GetExpressionID()
{
	return m_uExpressionID;
}

void Texture2DSample::GetConstantParamData(void*& pParam, unsigned char& uSize) const
{
	pParam = nullptr;
	uSize = 0;
}

unsigned char Texture2DSample::AddConstantParamToBuffer(void* pBuffer) const
{
	return 0;
}

Texture* Texture2DSample::GetTexture()
{
	return m_pTexture;
}
