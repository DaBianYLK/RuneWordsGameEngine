#include "D3D9Device.h"

#include "DisplayWindow.h"
#include "AssertUtil.h"

const D3DDEVTYPE			DefaultDeviceType				= D3DDEVTYPE_HAL;
const unsigned int			DefaultAdapterID				= D3DADAPTER_DEFAULT;
const D3DFORMAT				DefaultBackBufferFormat			= D3DFMT_A8R8G8B8;
const unsigned int			DefaultBackBufferCount			= 1;
const D3DMULTISAMPLE_TYPE	DefaultMultiSampleType			= D3DMULTISAMPLE_NONE;
const DWORD					DefaultMultiSampleQuality		= 0;
const D3DSWAPEFFECT			DefaultSwapEffect				= D3DSWAPEFFECT_DISCARD;
const bool					DefaultEnableAutoDepthStencil	= true;
const D3DFORMAT				DefaultAutoDepthStencilFormat	= D3DFMT_D24S8;
const D3DFORMAT				DefaultAutoDepthStencilFormat2	= D3DFMT_D16;
const DWORD					DefaultFlags					= 0;
const unsigned int			DefaultFullScreenRefreshRate	= D3DPRESENT_RATE_DEFAULT;
const unsigned int			DefaultPresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;	// 垂直同步，开启：D3DPRESENT_INTERVAL_ONE，关闭：D3DPRESENT_INTERVAL_IMMEDIATE

D3D9Device::D3D9Device(const DisplayWindow& window)
{
	SetDefaultParam();
	Init(window);
}


D3D9Device::~D3D9Device()
{
}

void D3D9Device::SetDefaultParam()
{
	m_DeviceType								= DefaultDeviceType;
	m_uAdapterID								= DefaultAdapterID;
	m_PresentParam.BackBufferFormat				= DefaultBackBufferFormat;
	m_PresentParam.BackBufferCount				= DefaultBackBufferCount;
	m_PresentParam.MultiSampleType				= DefaultMultiSampleType;
	m_PresentParam.MultiSampleQuality			= DefaultMultiSampleQuality;
	m_PresentParam.SwapEffect					= DefaultSwapEffect;
	m_PresentParam.EnableAutoDepthStencil		= DefaultEnableAutoDepthStencil;
	m_PresentParam.AutoDepthStencilFormat		= DefaultAutoDepthStencilFormat;
	m_PresentParam.Flags						= DefaultFlags;
	m_PresentParam.FullScreen_RefreshRateInHz	= DefaultFullScreenRefreshRate;
	m_PresentParam.PresentationInterval			= DefaultPresentationInterval;
}

bool D3D9Device::Init(const DisplayWindow& window)
{
	IDirect3D9* pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D9)
	{
		MessageBox(nullptr, "Initialize Direct3D-9 failed.", nullptr, 0);
		return;
	}

	D3DCAPS9 caps;
	pD3D9->GetDeviceCaps(m_uAdapterID, m_DeviceType, &caps);

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		m_uVertexProcessType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		m_uVertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	HWND hWnd = window.GetHandle();

	m_PresentParam.BackBufferWidth = window.GetWidth();
	m_PresentParam.BackBufferHeight = window.GetHeight();
	m_PresentParam.hDeviceWindow = hWnd;
	m_PresentParam.Windowed = !window.IsFullScreen();
	
	HRESULT result = pD3D9->CreateDevice(
		m_uAdapterID,					// primary adapter
		m_DeviceType,					// device type
		hWnd,							// window associated with device
		m_uVertexProcessType,			// vertex processing
		&m_PresentParam,				// present parameters
		&m_pDevice);					// return created device

	if (FAILED(result))
	{
		// try again using a 16-bit depth buffer
		m_PresentParam.AutoDepthStencilFormat = DefaultAutoDepthStencilFormat2;

		result = pD3D9->CreateDevice(
			m_uAdapterID,
			m_DeviceType,
			hWnd,
			m_uVertexProcessType,
			&m_PresentParam,
			&m_pDevice);

		if (FAILED(result))
		{
			pD3D9->Release();
			ErrorBox("Create Direct3D9 device failed.");
			return;
		}
	}

	pD3D9->Release();

	//m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}

bool D3D9Device::Release()
{
	D3D9SafeRelease(m_pDevice);

	return true;
}
