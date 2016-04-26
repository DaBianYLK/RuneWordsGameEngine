#include "RwgeViewport.h"

#include "RwgeCamera.h"
#include "RwgeD3dx9Extension.h"
#include "RwgeD3d9RenderTarget.h"

using namespace D3dx9Extension;

Viewport::Viewport(IDirect3DDevice9* pDevice, unsigned uX, unsigned uY, unsigned uWidth, unsigned uHeight): 
	m_pCamera			(nullptr),
	m_BackgroundColor	(D3DColorBlack)
{
	m_pDevice = pDevice;
	m_D3D9ViewportParam = { uX, uY, uWidth, uHeight, 0.0f, 1.0f };
	m_Rect = { uX, uY, uX + uWidth, uY + uHeight };			// Rect定义了Viewport在屏幕中所占矩形区域的的左上角顶点和右下角顶点
}

Viewport::~Viewport()
{
	
}

void Viewport::Update()
{
	Enable();
	Clear();

	if (m_pCamera)
	{
		m_pCamera->RenderScene(this);
	}
}

void Viewport::Enable() const
{
	m_pDevice->SetViewport(&m_D3D9ViewportParam);
}

void Viewport::Clear() const
{
	// Clear函数参数说明：清除区域数量、区域数组指针、缓冲区标志、重置后颜色、重置后深度值、重置后模板值
	m_pDevice->Clear(1, &m_Rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, m_D3D9ViewportParam.MaxZ, 0);
}

void Viewport::SetRect(unsigned uX, unsigned uY, unsigned uWidth, unsigned uHeight)
{
	m_D3D9ViewportParam = { uX, uY, uWidth, uHeight, 0.0f, 1.0f };
	m_Rect = { uX, uY, uX + uWidth, uY + uHeight };
}

void Viewport::SetBackgroundColor(const D3DCOLOR& color)
{
	m_BackgroundColor = color;
}

void Viewport::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Camera* Viewport::GetCamera() const
{
	return m_pCamera;
}

RenderTarget* Viewport::GetRenderTarget() const
{
	return m_pRenderTarget;
}

bool Viewport::operator==(const Viewport& viewport) const
{
	return this == &viewport;
}
