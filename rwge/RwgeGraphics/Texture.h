#pragma once

#include <string>

struct IDirect3DTexture9;

class Texture
{
public:
	Texture();
	~Texture();

	IDirect3DTexture9* GetTexturePtr() const { return pTexture; };

private:
	std::string m_strFilePath;

	IDirect3DTexture9* pTexture;
};