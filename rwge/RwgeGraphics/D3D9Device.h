#pragma once

#include "GraphicsDefinitions.h"

/*
D3D9Device仅能由RenderTarget创建和释放
*/

class DisplayWindow;

class D3D9Device
{
protected:
	D3D9Device(const DisplayWindow& window);
	D3D9Device(D3D9Device&& device);		// 转移构造函数
	~D3D9Device();

	void SetDefaultParam();
	bool Init(const DisplayWindow& window);
	virtual bool Release();

public:
	IDirect3DDevice9* GetDevicePtr() const { return m_pDevice; }

protected:
	IDirect3DDevice9*	m_pDevice;

private:
	D3DDEVTYPE m_DeviceType;
	D3DPRESENT_PARAMETERS m_PresentParam;
	unsigned int m_uAdapterID;
	unsigned int m_uVertexProcessType;
};

