/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	用于接收渲染结果并保存渲染图像的缓冲对象，它可以是BackBuffer、Texture等等
	2.	如果开发者没有手动为RenderTarget设置Viewport，RWGE游戏引擎会用一个覆盖整个RenderTarget的 DefaultViewport执行
		渲染；如果开发者手动为RenderTarget设置了Viewport，则忽略DefaultViewport ，只使用开发者设置的Viewport执行渲染
	3.	通常情况下，RenderTarget对开发者是隐藏的，开发者不需要直接操作RenderTarget，设置Viewport等操作是通过应用层的
		AppWindow实现的
	4.	Surface 是一段可以存放像素数据的内存缓冲区，某些类型的渲染目标使用了双缓冲机制，它们可能会具有多个 Surface
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
	virtual void Present() {};						// 只有SwapChain和Device需要实现Present

	virtual IDirect3DSurface9*		GetD3dSurface() = 0;	// 返回的Surface一定是可读写的（Device或SwapChain返回索引为0的BackBuffer）
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

	RD3d9Viewport				m_DefaultViewport;		// 默认Viewport
	std::list<RD3d9Viewport*>	m_listViewports;		// 特别指定的Viewport
};