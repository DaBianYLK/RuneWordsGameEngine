#include "RwgeTextureManager.h"

#include "RwgeD3d9Texture.h"

using namespace std;
using namespace Rwge;

RTextureManager::RTextureManager()
{

}

RTextureManager::~RTextureManager()
{
}

RD3d9Texture* RTextureManager::GetTexture(const Rwge::tstring& strPath)
{
	map<tstring, RD3d9Texture>::iterator itTexture = m_mapTextures.find(strPath);
	if (itTexture != m_mapTextures.end())
	{
		return &(itTexture->second);
	}

	// 没有找到就尝试从文件中加载
	map<tstring, RD3d9Texture>::_Pairib result = m_mapTextures.insert(make_pair(strPath, RD3d9Texture()));
	RwgeAssert(result.second);
	RD3d9Texture& texture = result.first->second;
	if (!texture.Load(strPath.c_str()))
	{
		m_mapTextures.erase(result.first);
		return nullptr;
	}

	return &texture;
}