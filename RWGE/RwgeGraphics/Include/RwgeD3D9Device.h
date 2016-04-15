#pragma once

#include "RwgeGraphics.h"

/*
D3D9Device仅能由RenderTarget创建和释放
*/

/*
关于显示分辨率
1.	Viewport分辨率和Device后台缓冲分辨率：Viewport是在Device后台缓冲上划定的一块区域，它的分辨率不可超过Device后台缓冲的分辨率，否则多出来的像素会被丢弃
2.	屏幕分辨率和Device后台缓冲分辨率：使用D3D全屏模式时，Device后台缓冲的分辨率一定要大于或等于屏幕分辨率，否则会报错
3.	主窗口分辨率和Device后台缓冲分辨率：显卡会先以Device后台缓冲分辨率绘制画面，再把Device后台缓冲的画面拉伸占满整个主窗口
4.	SwapChain后台缓冲分辨率和Device后台缓冲分辨率：显卡会先以Device后台缓冲分辨率绘制画面，再将Device后台缓冲中的数据复制到SwapChain后台缓冲中
*/

class RWindow;

class D3D9Device
{
protected:
	D3D9Device(const RWindow& window);
	D3D9Device(D3D9Device&& device);		// 转移构造函数
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

