/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	用于接收渲染结果并保存渲染图像的缓冲对象，它可以是BackBuffer、Texture等等。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>
#include <RwgeNoCopy.h>
#include <RwgeAppWindowListener.h>

struct IDirect3DSurface9;

class RD3d9RenderTarget : 
	public RObject,
	public NoCopy<RD3d9RenderTarget>
{
protected:
	RD3d9RenderTarget();
	virtual ~RD3d9RenderTarget();

public:
	FORCE_INLINE IDirect3DSurface9* GetD3dSurface() const { return m_pD3dSurface; }

	virtual void Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode) = 0;

protected:
	IDirect3DSurface9* m_pD3dSurface;
};