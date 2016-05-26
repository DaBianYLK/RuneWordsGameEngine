#include "RwgeD3d9SwapChain.h"

#include <RwgeAppWindow.h>
#include "RwgeD3d9Device.h"
#include "RwgeGraphics.h"
#include <RwgeLog.h>
#include "RwgeD3dx9Extension.h"

using namespace RwgeAppWindow;
using namespace RwgeD3dx9Extension;

RD3d9SwapChain::RD3d9SwapChain(const RAppWindow& window) :
	RD3d9RenderTarget(window.GetWidth(), window.GetHeight())
{
	IDirect3DDevice9* pD3dDevice = RD3d9Device::GetInstance().GetD3dDevice();

	m_D3dPresentParam.Windowed = true;										// SwapChain��������Ϊȫ��������ᴴ��ʧ��
	m_D3dPresentParam.BackBufferWidth = window.GetWidth();					// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_D3dPresentParam.BackBufferHeight = window.GetHeight();				// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_D3dPresentParam.hDeviceWindow = window.GetHandle();
	m_D3dPresentParam.MultiSampleType = D3DMULTISAMPLE_NONE;				// SwapChain���������û��Ч��
	m_D3dPresentParam.MultiSampleQuality = 0;								// SwapChain���������û��Ч��,������Ϊ3��3����ʱ�ᴴ��SwapChainʧ��

	pD3dDevice->CreateAdditionalSwapChain(&m_D3dPresentParam, &m_pD3dSwapChain);

	// ============================== ��ȡRenderTarget ==============================
	m_pD3dSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pD3dSurface);
}

RD3d9SwapChain::~RD3d9SwapChain()
{
	RwgeSafeRelease(m_pD3dSwapChain);
}

void RD3d9SwapChain::Resize(int s32Width, int s32Height, EDisplayMode mode)
{
	RwgeSafeRelease(m_pD3dSwapChain);

	m_D3dPresentParam.BackBufferWidth = s32Width;							// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_D3dPresentParam.BackBufferHeight = s32Height;							// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_D3dPresentParam.Windowed = true;										// SwapChain��������Ϊȫ��������ᴴ��ʧ��

	IDirect3DDevice9* pD3dDevice = RD3d9Device::GetInstance().GetD3dDevice();
	pD3dDevice->CreateAdditionalSwapChain(&m_D3dPresentParam, &m_pD3dSwapChain);
	
	RD3d9RenderTarget::Resize(s32Width, s32Height, mode);
}

void RD3d9SwapChain::Present()
{
	HRESULT hResult = m_pD3dSwapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("SwapChain present failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

IDirect3DSurface9* RD3d9SwapChain::GetD3dSurface()
{
	if (m_pD3dSurface)
	{
		m_pD3dSurface->Release();
	}

	HRESULT hResult = m_pD3dSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pD3dSurface);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Get swap chain back buffer failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
	return m_pD3dSurface;
}
