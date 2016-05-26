/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-05
	DESC :	
	1.	Texture������TextureManager������ɾ��
	2.	DX�У�������ͼ�����Ͻ�Ϊ���������ԭ�㣬X ��ˮƽ���ң�Y ����ֱ���£���������ͼ��
		(0, 0)������(1, 0)
		  |            |
		  |            |
		  |            |
		(0, 1)������(1, 1)
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <tchar.h>
#include <RwgeTString.h>
#include <RwgeObject.h>

class RD3d9Device;
struct IDirect3DTexture9;

class RD3d9Texture : public RObject
{
public:
	RD3d9Texture();
	~RD3d9Texture();

	bool Load(const TCHAR* szPath);
	IDirect3DTexture9* GetD3DTexture() const { return m_pD3DTexture; };

private:
	Rwge::tstring		m_strFilePath;

	IDirect3DTexture9*	m_pD3DTexture;
};