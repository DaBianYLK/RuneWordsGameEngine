#pragma once

#include "RwgeTextureInfo.h"
#include <hash_map>
#include <RwgeSingleton.h>

class RD3d9Device;
struct IDirect3DDevice9;

/*
Texture与Shader类似，因为需要依赖于Device生成实例，所以TextureManager只管理纹理信息
*/

class TextureManager : public Singleton<TextureManager>
{
public:
	TextureManager();
	~TextureManager();

	TextureInfo* GetTextureInfo(const std::string& strPath);

private:
	std::hash_map<std::string, TextureInfo*> m_hashTextures;
};

