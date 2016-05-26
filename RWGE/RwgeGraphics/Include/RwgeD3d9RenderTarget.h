/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	���ڽ�����Ⱦ�����������Ⱦͼ��Ļ��������������BackBuffer��Texture�ȵ�
	2.	���������û���ֶ�ΪRenderTarget����Viewport��RWGE��Ϸ�������һ����������RenderTarget�� DefaultViewportִ��
		��Ⱦ������������ֶ�ΪRenderTarget������Viewport�������DefaultViewport ��ֻʹ�ÿ��������õ�Viewportִ����Ⱦ
	3.	ͨ������£�RenderTarget�Կ����������صģ������߲���Ҫֱ�Ӳ���RenderTarget������Viewport�Ȳ�����ͨ��Ӧ�ò��
		AppWindowʵ�ֵ�
	4.	Surface ��һ�ο��Դ���������ݵ��ڴ滺������ĳЩ���͵���ȾĿ��ʹ����˫������ƣ����ǿ��ܻ���ж�� Surface
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeObject.h>
#include <RwgeNoCopy.h>
#include <RwgeAppWindowListener.h>
#include "RwgeD3d9Viewport.h"
#include <list>

struct IDirect3DSurface9;

class RD3d9RenderTarget : 
	public RObject,
	public NoCopy<RD3d9RenderTarget>
{
	friend class RD3d9RenderSystem;

protected:
	RD3d9RenderTarget(int s32Width, int s32Height);
	virtual ~RD3d9RenderTarget();

public:
	virtual void Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode);
	virtual void Present() {};						// ֻ��SwapChain��Device��Ҫʵ��Present

	virtual IDirect3DSurface9*		GetD3dSurface() = 0;	// ���ص�Surfaceһ���ǿɶ�д�ģ�Device��SwapChain��������Ϊ0��BackBuffer��
	FORCE_INLINE void				ReleaseD3dSurface()								{ RwgeSafeRelease(m_pD3dSurface); };
	FORCE_INLINE const D3DRECT&		GetD3dRect()				const				{ return m_DefaultViewport.GetD3dRect(); };
	FORCE_INLINE const D3DCOLOR&	GetBackgroundColor()		const				{ return m_DefaultViewport.GetBackgroundColor(); }
	FORCE_INLINE void				SetBackgroundColor(const D3DCOLOR& d3dColor)	{ m_DefaultViewport.SetBackgroundColor(d3dColor); };
	FORCE_INLINE void				SetDefaultCamera(RCamera* pCamera)				{ m_DefaultViewport.SetCamera(pCamera); };
	FORCE_INLINE RCamera*			GetDefaultCamera()			const				{ return m_DefaultViewport.GetCamera(); };
	FORCE_INLINE float				GetMaxZ()					const				{ return m_DefaultViewport.GetMaxZ(); };
	FORCE_INLINE const RD3d9Viewport* GetDefaultViewport()		const				{ return &m_DefaultViewport; };
	FORCE_INLINE bool				IsUsingDefaultViewport()	const				{ return m_listViewports.empty(); };

	void AddViewport(RD3d9Viewport* pViewport); 
	void RemoveViewport(RD3d9Viewport* pViewport);

protected:
	IDirect3DSurface9*			m_pD3dSurface;

	RD3d9Viewport				m_DefaultViewport;		// Ĭ��Viewport
	std::list<RD3d9Viewport*>	m_listViewports;		// �ر�ָ����Viewport
};