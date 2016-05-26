/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	通过将Viewport与Camera绑定，可以实现渲染模块与场景模块的连接
	2.	Viewport实际上是Device BackBuffer 的一个子集，它划定了一块矩形缓冲区，将光栅化后的渲染图像存放到这块缓冲区中
	3.	因为RWGE引擎保证了窗口与DeviceBackBuffer的大小一定是一样的，所以可以认为Viewport也是窗口的一个子集
	4.	一个或多个Viewport可以绑定于同一个Camera，且它们的生命周期是互相独立的
	5.	Viewport是独立的对象，它可以被设置到RenderTarget中，限制渲染的显示区域
	6.	Viewport只是一个D3D 参数，并不是COM 对象，可以随意创建、复制与删除，也正因为如此，它只有被设置到RenderTarget
		中才有意义
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include "d3d9.h"
#include <RwgeObject.h>

class RCamera;
class RD3d9RenderTarget;
class RD3d9RenderQueue;

class RD3d9Viewport : public RObject
{
public:
	RD3d9Viewport(unsigned int u32X, unsigned int u32Y, unsigned int u32Width, unsigned int u32Height);
	~RD3d9Viewport();

	void Move(unsigned int u32X, unsigned int u32Y);
	void Resize(unsigned int u32Width, unsigned int u32Height);
	void SetRect(unsigned int u32X, unsigned int u32Y, unsigned int u32Width, unsigned int u32Height);

	void UpdateCamera(RD3d9RenderQueue& renderQueue);

	FORCE_INLINE const D3DVIEWPORT9*	GetD3dViewport()		const					{ return &m_D3dViewport; };
	FORCE_INLINE const D3DRECT&			GetD3dRect()			const					{ return m_D3dRect; };
	FORCE_INLINE void					SetBackgroundColor(const D3DCOLOR& d3dColor)	{ m_D3dBackgroundColor = d3dColor; };
	FORCE_INLINE const D3DCOLOR& 		GetBackgroundColor()	const					{ return m_D3dBackgroundColor; }
	FORCE_INLINE void					SetCamera(RCamera* pCamera)						{ m_pCamera = pCamera; };
	FORCE_INLINE RCamera*				GetCamera()				const					{ return m_pCamera; };
	FORCE_INLINE void					SetRenderTarget(RD3d9RenderTarget* pTarget)		{ m_pRenderTarget = pTarget; };
	FORCE_INLINE RD3d9RenderTarget*		GetRenderTarget()		const					{ return m_pRenderTarget; };
	FORCE_INLINE float					GetMaxZ()				const					{ return m_D3dViewport.MaxZ; };

private:
	D3DVIEWPORT9		m_D3dViewport;
	D3DRECT				m_D3dRect;					// 为Clear缓存的参数
	D3DCOLOR			m_D3dBackgroundColor;

	RCamera*			m_pCamera;					// Viewport绑定的Camera
	RD3d9RenderTarget*	m_pRenderTarget;			// Viewport从属的RenderTarget
};