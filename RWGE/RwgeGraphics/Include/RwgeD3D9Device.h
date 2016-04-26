/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-09
	DESC :	
	1.	��װD3D Device����������ֻ��Ҫά��һ��Deviceʵ�����ɣ���˽�������Ϊ����
	2.	������ʾ�ֱ���
		A.	Viewport��Device��̨���壺Viewport����Device��̨�����ϻ�����һ���������ķֱ��ʲ��ɳ���Device��̨�����
			�ֱ��ʣ��������������ػᱻ����
		B.	��Ļ��Device��̨���壺ʹ��D3D ȫ��ģʽʱ��Device��̨����ķֱ���һ��Ҫ���ڻ������Ļ�ֱ��ʣ�����ᱨ��
		C.	�����ں�Device��̨���壺�Կ�������Device��̨����ķֱ��ʻ��ƻ��棬�ٰѻ�������ռ������������
		D.	SwapChain ��̨�����Device��̨���壺�Կ�������Device��̨����ֱ��ʻ��ƻ��棬�ٽ�Device��̨�����е����ݸ�
			�Ƶ�SwapChain��̨������
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include <RwgeObject.h>
#include <RwgeSingleton.h>
#include <RwgeCoreDef.h>
#include "RwgeD3d9RenderTarget.h"

class RAppWindow;

class RD3d9Device : 
	public RD3d9RenderTarget,
	public Singleton<RD3d9Device>
{
private:
	RD3d9Device(const RAppWindow& window);
	~RD3d9Device();

public:
	FORCE_INLINE IDirect3DDevice9* GetD3dDevice() const { return m_pD3dDevice; }
	
	virtual void Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode) override;

private:
	IDirect3DDevice9*		m_pD3dDevice;
	D3DCAPS9				m_D3dDeviceCapabilites;

	unsigned int			m_u32AdapterID;
	D3DDEVTYPE				m_DeviceType;
	unsigned int			m_u32VertexProcessType;
	D3DPRESENT_PARAMETERS	m_D3dPresentParam;
};

