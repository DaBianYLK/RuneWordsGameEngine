#pragma once

#include "GraphicsDefinitions.h"

/*
D3D9Device仅能由RenderTarget创建和释放
*/

class DisplayWindow;

class D3D9Device
{
	friend class RenderTarget;

private:
	D3D9Device(const DisplayWindow& window);
	~D3D9Device();

	void SetDefaultParam();
	bool Init(const DisplayWindow& window);
	bool Release();

private:
	D3DDEVTYPE m_DeviceType;
	D3DPRESENT_PARAMETERS m_PresentParam;
	unsigned int m_uAdapterID;
	unsigned int m_uVertexProcessType;

	IDirect3DDevice9*	m_pDevice;
};

