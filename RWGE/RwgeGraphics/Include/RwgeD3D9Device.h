/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-09
	DESC :	
	1.	封装D3D Device，整个程序只需要维护一个Device实例即可，因此将它定义为单例
	2.	关于显示分辨率
		A.	Viewport和Device后台缓冲：Viewport是在Device后台缓冲上划定的一块区域，它的分辨率不可超过Device后台缓冲的
			分辨率，否则多出来的像素会被丢弃
		B.	屏幕和Device后台缓冲：使用D3D 全屏模式时，Device后台缓冲的分辨率一定要大于或等于屏幕分辨率，否则会报错
		C.	主窗口和Device后台缓冲：显卡会先以Device后台缓冲的分辨率绘制画面，再把画面拉伸占满整个主窗口
		D.	SwapChain 后台缓冲和Device后台缓冲：显卡会先以Device后台缓冲分辨率绘制画面，再将Device后台缓冲中的数据复
			制到SwapChain后台缓冲中
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

