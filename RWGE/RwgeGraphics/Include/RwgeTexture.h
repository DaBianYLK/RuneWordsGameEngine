#pragma once

#include <string>

struct IDirect3DTexture9;
class TextureInfo;
class RD3d9Device;

/*
DX�У�������ͼ�����Ͻ�Ϊ���������ԭ�㣬X��ˮƽ���ң�Y����ֱ���£���������ͼ��
(0, 0)������(1, 0)
  |            |
  |            |
  |            |
(0, 1)������(1, 1)

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