#include "TextureManager.h"

using namespace std;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
}

TextureInfo* TextureManager::GetTextureInfo(const std::string& strPath)
{
	auto itTexturePair = m_hashTextures.find(strPath);
	if (itTexturePair != m_hashTextures.end())
	{
		return itTexturePair->second;
	}

	// û���ҵ��ͼ��뵽��������
	TextureInfo* pTextureInfo = new TextureInfo(strPath);

	m_hashTextures.insert(make_pair(strPath, pTextureInfo));

	return pTextureInfo;
}
