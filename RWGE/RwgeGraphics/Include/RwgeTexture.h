#pragma once

#include <string>

struct IDirect3DTexture9;
class TextureInfo;
class RD3d9Device;

/*
DX中，纹理贴图的坐上角为纹理坐标的原点，X轴水平向右，Y轴竖直向下，具体如下图：
(0, 0)———(1, 0)
  |            |
  |            |
  |            |
(0, 1)———(1, 1)

*/

class Texture
{
public:
	Texture(TextureInfo* pInfo);
	~Texture();

	void Load(const RD3d9Device& device);
	IDirect3DTexture9* GetD3DTexturePtr() const { return m_pD3DTexture; };

private:
	TextureInfo* m_pInfo;

	IDirect3DTexture9* m_pD3DTexture;
};