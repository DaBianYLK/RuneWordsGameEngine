#include "RwgeD3D9Device.h"

#include "RwgeWindow.h"
#include "RwgeAssert.h"
#include "RwgeRenderSystem.h"
#include "RwgeApplication.h"

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
const DWORD					DefaultFlags					= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
const unsigned int			DefaultFullScreenRefreshRate	= D3DPRESENT_RATE_DEFAULT;
const unsigned int			DefaultPresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;	// ��ֱͬ����������D3DPRESENT_INTERVAL_ONE���رգ�D3DPRESENT_INTERVAL_IMMEDIATE

D3D9Device::D3D9Device(const RWindow& window)
{
	SetDefaultParam();
	Init(window);
}

D3D9Device::D3D9Device(D3D9Device&& device) :
	m_DeviceType(device.m_DeviceType), 
	m_PresentParam(device.m_PresentParam), 
	m_uAdapterID(device.m_uAdapterID), 
	m_uVertexProcessType(device.m_uVertexProcessType), 
	m_pDevice(device.m_pDevice)
{
	device.m_pDevice = nullptr;
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

bool D3D9Device::Init(const RWindow& window)
{
	IDirect3D9* pD3D9 = g_RenderSystem.GetD3D9Ptr();
	if (!pD3D9)
	{
		return false;
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
	const_cast<RWindow&>(window).Resize(0, 0, 1920, 1080);

	m_PresentParam.BackBufferWidth = window.GetWidth();
	m_PresentParam.BackBufferHeight = window.GetHeight();
	//m_PresentParam.BackBufferWidth = 1440;
	//m_PresentParam.BackBufferHeight = 900;
	m_PresentParam.hDeviceWindow = hWnd;
	m_PresentParam.Windowed = !window.IsFullScreen();		// ��ʹ�ô���ģʽʱ�����ָ����֡����ߴ�С����Ļ�ߴ磬�ᵼ��Device����ʧ��
	
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
			ErrorBox("Create Direct3D9 device failed.");
			return false;
		}
	}

	RWindow* pW = RApplication::GetInstance().CreateDisplayWindow();
	//pW->Resize(0, 0, 1440, 900);
	pW->SetFullScreen();
	pW->Resize(0, 0, 1440, 900);
	pW->Show();
	
	// Use the current display mode.  ʹ�õ�ǰ����ʾģʽ
	D3DDISPLAYMODE mode;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	m_PresentParam.Windowed = true;										// SwapChain��������Ϊȫ��������ᴴ��ʧ��
	m_PresentParam.BackBufferWidth = 1920;								// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_PresentParam.BackBufferHeight = 1080;								// ����Ϊ0ʱ��SwapChain��ȡһ�����ֵ
	m_PresentParam.hDeviceWindow = pW->GetHandle();
	m_PresentParam.MultiSampleType = D3DMULTISAMPLE_NONE;				// SwapChain���������û��Ч��
	m_PresentParam.MultiSampleQuality = 0;								// SwapChain���������û��Ч��,������Ϊ3��3����ʱ�ᴴ��SwapChainʧ��
	m_pDevice->CreateAdditionalSwapChain(&m_PresentParam, &m_pSwapChain);

	LPDIRECT3DSURFACE9 pBack = NULL;
	m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBack);
	m_pDevice->SetRenderTarget(0, pBack);
	pBack->Release();

	return true;
}

bool D3D9Device::Release()
{
	D3D9SafeRelease(m_pDevice);

	return true;
}
