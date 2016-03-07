#pragma once

#include "MaterialExpression.h"
#include "Texture.h"
#include <string>

class Texture2DSample : public MaterialExpression
{
public:
	Texture2DSample();
	virtual ~Texture2DSample();

	unsigned char GetExpressionID() override;

	void GetConstantParamData(void*& pParam, unsigned char& uSize) const override;
	unsigned char AddConstantParamToBuffer(void* pBuffer) const override;

	Texture* GetTexture() override;

private:
	static const unsigned char m_uExpressionID = 1;

	std::string m_strTexNameInHLSL;
	unsigned int m_uTexIndex;

	Texture* m_pTexture;
};
