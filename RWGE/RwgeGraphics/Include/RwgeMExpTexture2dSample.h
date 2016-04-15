#pragma once

#include "RwgeMaterialExpression.h"
#include <string>

class TextureInfo;

class MExpTexture2DSample : public MaterialExpression
{
public:
	MExpTexture2DSample();
	virtual ~MExpTexture2DSample();

	unsigned char GetExpressionID() const override;
	unsigned int GetConstantParamSize() const override;

	void GetConstantParamData(void*& pParam, unsigned char& uSize) const override;
	unsigned char AddConstantParamToBuffer(void* pBuffer) const override;

	void SetTextureInfo(TextureInfo* pInfo);
	TextureInfo* GetTextureInfo() override;

private:
	static const unsigned char m_uExpressionID = 1;

	std::string m_strTexNameInHLSL;
	unsigned int m_uTexIndex;		// ToDo: 纹理ID由Material来生成和管理更好

	TextureInfo* m_pTextureInfo;
};
