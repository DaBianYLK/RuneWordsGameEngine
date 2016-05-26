/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	
	1.	Texture仅能由TextureManager创建与删除
	2.	DX中，纹理贴图的坐上角为纹理坐标的原点，X 轴水平向右，Y 轴竖直向下，具体如下图：
		(0, 0)———(1, 0)
		  |            |
		  |            |
		  |            |
		(0, 1)———(1, 1)
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