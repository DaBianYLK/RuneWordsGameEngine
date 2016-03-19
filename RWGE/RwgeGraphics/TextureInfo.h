#pragma once

#include <string>

class Texture;
class RenderTarget;

/*
TextureInfo由TextureManager进行管理
*/

class TextureInfo
{
	friend class TextureManager;

private:
	TextureInfo(const std::string strPath);

public:
	~TextureInfo();

	const std::string& GetTextureFilePath() const;

private:
	std::string m_strFilePath;
};

