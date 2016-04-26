#pragma once

#include "RwgeTextureInfo.h"
#include <hash_map>
#include <RwgeSingleton.h>

class RD3d9Device;
struct IDirect3DDevice9;

/*
Texture��Shader���ƣ���Ϊ��Ҫ������Device����ʵ��������TextureManagerֻ����������Ϣ
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

