#include "RwgeD3d9Viewport.h"

#include "RwgeCamera.h"
#include "RwgeD3dx9Extension.h"
#include "RwgeD3d9RenderTarget.h"
#include "RwgeGraphics.h"
#include "RwgeD3d9Device.h"
#include <RwgeLog.h>
#include "RwgeCamera.h"

using namespace RwgeD3dx9Extension;

RD3d9Viewport::RD3d9Viewport(unsigned u32X, unsigned u32Y, unsigned u32Width, unsigned u32Height) :
	m_D3dBackgroundColor(D3dColorBlack),
	m_pCamera(nullptr),
	m_pRenderTarget(nullptr)
{
	m_D3dViewport.X			= u32X;				// Viewport左上角顶点X轴坐标
	m_D3dViewport.Y			= u32Y;				// Viewport左上角顶点Y轴坐标
	m_D3dViewport.Width		= u32Width;			// Viewport宽度
	m_D3dViewport.Height	= u32Height;		// Viewport高度
	m_D3dViewport.MinZ		= 0.0f;				// Viewport最小深度值
	m_D3dViewport.MaxZ		= 1.0f;				// Viewport最大深度值

	m_D3dRect.x1			= u32X;
	m_D3dRect.y1			= u32Y;
	m_D3dRect.x2			= u32X + u32Width;
	m_D3dRect.y2			= u32Y + u32Height;
}

RD3d9Viewport::~RD3d9Viewport()
{
	
}

void RD3d9Viewport::Move(unsigned u32X, unsigned u32Y)
{
	m_D3dViewport.X			= u32X;
	m_D3dViewport.Y			= u32Y;
	m_D3dRect.x1			= u32X;
	m_D3dRect.y1			= u32Y;
}

void RD3d9Viewport::Resize(unsigned u32Width, unsigned u32Height)
{
	m_D3dViewport.Width		= u32Width;
	m_D3dViewport.Height	= u32Height;
	m_D3dRect.x2			= m_D3dRect.x1 + u32Width;
	m_D3dRect.y2			= m_D3dRect.y1 + u32Height;
}

void RD3d9Viewport::SetRect(unsigned u32X, unsigned u32Y, unsigned u32Width, unsigned u32Height)
{
	m_D3dViewport.X			= u32X;
	m_D3dViewport.Y			= u32Y;
	m_D3dViewport.Width		= u32Width;
	m_D3dViewport.Height	= u32Height;

	m_D3dRect.x1			= u32X;
	m_D3dRect.y1			= u32Y;
	m_D3dRect.x2			= u32X + u32Width;
	m_D3dRect.y2			= u32Y + u32Height;
}

void RD3d9Viewport::UpdateCamera(RD3d9RenderQueue& renderQueue)
{
	if (m_pCamera != nullptr)
	{
		m_pCamera->GetSceneShot(this, renderQueue);
	}
}