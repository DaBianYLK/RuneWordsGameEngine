#pragma once

#include "RwgeGraphics.h"

/*
D3D9Device������RenderTarget�������ͷ�
*/

/*
������ʾ�ֱ���
1.	Viewport�ֱ��ʺ�Device��̨����ֱ��ʣ�Viewport����Device��̨�����ϻ�����һ���������ķֱ��ʲ��ɳ���Device��̨����ķֱ��ʣ��������������ػᱻ����
2.	��Ļ�ֱ��ʺ�Device��̨����ֱ��ʣ�ʹ��D3Dȫ��ģʽʱ��Device��̨����ķֱ���һ��Ҫ���ڻ������Ļ�ֱ��ʣ�����ᱨ��
3.	�����ڷֱ��ʺ�Device��̨����ֱ��ʣ��Կ�������Device��̨����ֱ��ʻ��ƻ��棬�ٰ�Device��̨����Ļ�������ռ������������
4.	SwapChain��̨����ֱ��ʺ�Device��̨����ֱ��ʣ��Կ�������Device��̨����ֱ��ʻ��ƻ��棬�ٽ�Device��̨�����е����ݸ��Ƶ�SwapChain��̨������
*/

class RWindow;

class D3D9Device
{
protected:
	D3D9Device(const RWindow& window);
	D3D9Device(D3D9Device&& device);		// ת�ƹ��캯��
	~D3D9Device();

	void SetDefaultParam();
	bool Init(const RWindow& window);
	virtual bool Release();

public:
	IDirect3DDevice9* GetDevicePtr() const { return m_pDevice; }

protected:
	IDirect3DDevice9*	m_pDevice;
	IDirect3DSwapChain9* m_pSwapChain;

private:
	D3DDEVTYPE m_DeviceType;
	D3DPRESENT_PARAMETERS m_PresentParam;
	unsigned int m_uAdapterID;
	unsigned int m_uVertexProcessType;
};

