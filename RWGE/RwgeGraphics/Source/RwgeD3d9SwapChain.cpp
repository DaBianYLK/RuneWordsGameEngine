#include "RwgeD3d9SwapChain.h"

#include <RwgeAppWindow.h>
#include <RwgeD3d9Device.h>
#include <RwgeGraphics.h>

using namespace RwgeAppWindow;

RD3d9SwapChain::RD3d9SwapChain(const RAppWindow& window)
{
	IDirect3DDevice9* pD3dDevice = RD3d9Device::GetInstance().GetD3dDevice();

	m_D3dPresentParam.Windowed = true;										// SwapChain不能设置为全屏，否则会创建失败
	m_D3dPresentParam.BackBufferWidth = window.GetWidth();					// 设置为0时，SwapChain会取一个随机值
	m_D3dPresentParam.BackBufferHeight = window.GetHeight();				// 设置为0时，SwapChain会取一个随机值
	m_D3dPresentParam.hDeviceWindow = window.GetHandle();
	m_D3dPresentParam.MultiSampleType = D3DMULTISAMPLE_NONE;				// SwapChain的这个参数没有效果
	m_D3dPresentParam.MultiSampleQuality = 0;								// SwapChain的这个参数没有效果,但定义为3或3以上时会创建SwapChain失败

	pD3dDevice->CreateAdditionalSwapChain(&m_D3dPresentParam, &m_pD3dSwapChain);

	m_pD3dSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pD3dSurface);
}

RD3d9SwapChain::~RD3d9SwapChain()
{
	D3d9SafeRelease(m_pD3dSwapChain);
}

void RD3d9SwapChain::Resize(int s32Width, int s32Height, EDisplayMode mode)
{
	D3d9SafeRelease(m_pD3dSwapChain);

	m_D3dPresentParam.BackBufferWidth = s32Width;							// 设置为0时，SwapChain会取一个随机值
	m_D3dPresentParam.BackBufferHeight = s32Height;							// 设置为0时，SwapChain会取一个随机值
	m_D3dPresentParam.Windowed = true;										// SwapChain不能设置为全屏，否则会创建失败

	IDirect3DDevice9* pD3dDevice = RD3d9Device::GetInstance().GetD3dDevice();
	pD3dDevice->CreateAdditionalSwapChain(&m_D3dPresentParam, &m_pD3dSwapChain);
}
