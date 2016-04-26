#include "RwgeD3d9Device.h"

#include "RwgeAppWindow.h"
#include "RwgeAssert.h"
#include "RwgeD3d9RenderSystem.h"
#include <RwgeLog.h>

using namespace RwgeAppWindow;

const unsigned int			DefaultAdapterID				= D3DADAPTER_DEFAULT;					// �Կ�������ID
const D3DDEVTYPE			DefaultDeviceType				= D3DDEVTYPE_HAL;						// D3Dģʽ��HAL��Ӳ��������REF���������
const D3DFORMAT				DefaultBackBufferFormat			= D3DFMT_A8R8G8B8;						// ��̨�����ʽ
const unsigned int			DefaultBackBufferCount			= 1;									// ��̨��������ֻ��Ϊ0��1
const D3DMULTISAMPLE_TYPE	DefaultMultiSampleType			= D3DMULTISAMPLE_NONE;					// ���ز������������
const DWORD					DefaultMultiSampleQuality		= 0;									// ����ƺ�û��
const D3DSWAPEFFECT			DefaultSwapEffect				= D3DSWAPEFFECT_DISCARD;
const bool					DefaultEnableAutoDepthStencil	= true;
const D3DFORMAT				DefaultAutoDepthStencilFormat	= D3DFMT_D24S8;							// �����ģ�建���ʽ
const D3DFORMAT				DefaultAutoDepthStencilFormat2	= D3DFMT_D16;							// ��ѡ�����ģ�建���ʽ
const DWORD					DefaultFlags					= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
const unsigned int			DefaultFullScreenRefreshRate	= D3DPRESENT_RATE_DEFAULT;
const unsigned int			DefaultPresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;		// ��ֱͬ����������ONE���رգ�IMMEDIATE

RD3d9Device::RD3d9Device(const RAppWindow& window) :
	m_pD3dDevice(nullptr)
{
	IDirect3D9* pD3D9 = RD3d9RenderSystem::GetInstance().GetD3d9();

	m_u32AdapterID = DefaultAdapterID;
	m_DeviceType = DefaultDeviceType;

	// ============================== ����Ĭ����ʾ���� ==============================
	m_D3dPresentParam.BackBufferFormat				= DefaultBackBufferFormat;
	m_D3dPresentParam.BackBufferCount				= DefaultBackBufferCount;
	m_D3dPresentParam.MultiSampleType				= DefaultMultiSampleType;
	m_D3dPresentParam.MultiSampleQuality			= DefaultMultiSampleQuality;
	m_D3dPresentParam.SwapEffect					= DefaultSwapEffect;
	m_D3dPresentParam.EnableAutoDepthStencil		= DefaultEnableAutoDepthStencil;
	m_D3dPresentParam.AutoDepthStencilFormat		= DefaultAutoDepthStencilFormat;
	m_D3dPresentParam.Flags							= DefaultFlags;
	m_D3dPresentParam.FullScreen_RefreshRateInHz	= DefaultFullScreenRefreshRate;
	m_D3dPresentParam.PresentationInterval			= DefaultPresentationInterval;
	m_D3dPresentParam.BackBufferWidth				= window.GetWidth();
	m_D3dPresentParam.BackBufferHeight				= window.GetHeight();
	m_D3dPresentParam.hDeviceWindow					= window.GetHandle();
	m_D3dPresentParam.Windowed						= window.IsTrueFullScreen();	// ע�⣺ȫ��ʱ�������̨����С����Ļ�ߴ�ᵼ��Device����ʧ��

	// ============================== ���BackBufferFormat�Ƿ���� ==============================
	HRESULT hResult = pD3D9->CheckDeviceType(
		m_u32AdapterID, 
		m_DeviceType, 
		m_D3dPresentParam.BackBufferFormat, 
		m_D3dPresentParam.BackBufferFormat, 
		m_D3dPresentParam.Windowed);

	if (FAILED(hResult))
	{
		RwgeLog("Failed to create D3D Device using default back buffer format.");

		D3DDISPLAYMODE d3dDisplayMode;
		hResult = pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dDisplayMode);
		if (FAILED(hResult))
		{
			RwgeErrorBox("Failed to get graphics adapter's display mode. Create D3D Device failed.");
			return;
		}

		m_D3dPresentParam.BackBufferFormat = d3dDisplayMode.Format;
	}
	
	// ============================== ���Ӳ�����㴦���Ƿ���� ==============================
	hResult = pD3D9->GetDeviceCaps(m_u32AdapterID, m_DeviceType, &m_D3dDeviceCapabilites);
	if (FAILED(hResult))
	{
		RwgeErrorBox("Failed to get D3D Device Capabilites. Create D3D Device failed.");
		return;
	}

	if (m_D3dDeviceCapabilites.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT && m_D3dDeviceCapabilites.VertexShaderVersion >= D3DVS_VERSION(1, 1))
	{
		m_u32VertexProcessType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		m_u32VertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// ============================== ���Դ���Device ==============================
	hResult = pD3D9->CreateDevice(
		m_u32AdapterID,						// primary adapter
		m_DeviceType,						// device type
		m_D3dPresentParam.hDeviceWindow,	// window associated with device
		m_u32VertexProcessType,				// vertex processing
		&m_D3dPresentParam,					// present parameters
		&m_pD3dDevice);						// return created device

	if (FAILED(hResult))
	{
		RwgeLog("Failed to create D3D Device with 32-bit Depth / Stencil Buffer. Try again with 16-bit Depth Buffer.");

		m_D3dPresentParam.AutoDepthStencilFormat = DefaultAutoDepthStencilFormat2;

		hResult = pD3D9->CreateDevice(
			m_u32AdapterID,
			m_DeviceType,
			m_D3dPresentParam.hDeviceWindow,
			m_u32VertexProcessType,
			&m_D3dPresentParam,
			&m_pD3dDevice);

		if (FAILED(hResult))
		{
			RwgeErrorBox("Create D3D Device failed.");
			return;
		}
	}
}

RD3d9Device::~RD3d9Device()
{
	D3d9SafeRelease(m_pD3dDevice);
}

void RD3d9Device::Resize(int s32Width, int s32Height, EDisplayMode mode)
{
	m_D3dPresentParam.BackBufferWidth	= s32Width;
	m_D3dPresentParam.BackBufferHeight	= s32Height;
	m_D3dPresentParam.Windowed			= mode == EDM_TrueFullScreen;

	HRESULT hResult = m_pD3dDevice->Reset(&m_D3dPresentParam);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("D3D Device reset failed!"));
	}
}
