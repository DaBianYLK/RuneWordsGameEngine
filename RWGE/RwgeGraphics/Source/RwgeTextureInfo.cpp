#include "RwgeTextureInfo.h"

#include "RwgeRenderTarget.h"

TextureInfo::TextureInfo(const std::string strPath) :
	m_strFilePath(strPath)
{
}

TextureInfo::~TextureInfo()
{
}

const std::string& TextureInfo::GetTextureFilePath() const
{
	return m_strFilePath;
}