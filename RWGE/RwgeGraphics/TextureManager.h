#pragma once

#include "TextureInfo.h"
#include <hash_map>
#include <Singleton.h>

class D3D9Device;
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

