/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	���ڽ�����Ⱦ�����������Ⱦͼ��Ļ��������������BackBuffer��Texture�ȵȡ�
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