/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-09
	DESC :	
	1.	封装D3D Device，只能通过RenderSystem创建与释放
	2.	整个程序只需要维护一个Device实例即可，将它定义为单例
	3.	关于显示分辨率
		A.	Viewport和Device后台缓冲：Viewport是在Device后台缓冲上划定的一块区域，它的分辨率不可超过Device后台缓冲的
			分辨率，否则多出来的像素会被丢弃
		B.	屏幕和Device后台缓冲：使用D3D 全屏模式时，Device后台缓冲的分辨率一定要大于或等于屏幕分辨率，否则会报错
		C.	主窗口和Device后台缓冲：显卡会先以Device后台缓冲的分辨率绘制画面，再把画面拉伸占满整个主窗口
		D.	SwapChain 后台缓冲和Device后台缓冲：显卡会先以Device后台缓冲分辨率绘制画面，再将Device后台缓冲中的数据复
			制到SwapChain后台缓冲中
	4.	关于Present与EndScene：
		CPU 提交渲染请求给GPU 后会立即返回，也就是说GPU 绘图运算相对CPU 来说是异步的，EndScene调用成功后，DX保证所有
		的渲染请求都已经提交给GPU ，但不保证这些请求都已经执行完毕；Present 调用成功时，保证GPU 将所有的渲染请求都执
		行完毕，并将渲染结果提交给屏幕缓冲。如果调用EndScene后立即调用Present ，CPU 需要等待GPU 绘制完毕，进而导致线
		程阻塞。因此，最好在EndScene后先执行游戏逻辑，再执行Present ，可以在一定程度上提高游戏运行帧数。

	ToDo: D3D Device Present函数的第三个参数似乎可以用来支持多窗口显示，可以尝试一下是否可行
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include <RwgeObject.h>
#include <RwgeSingleton.h>
#include <RwgeCoreDef.h>
#include "RwgeD3d9RenderTarget.h"

class RAppWindow;
class RD3d9Viewport;
class RD3d9Shader;
class RD3d9VertexDeclaration;

class RD3d9Device : 
	public RD3d9RenderTarget,
	public Singleton<RD3d9Device>
{
	friend class RD3d9RenderSystem;

private:
	RD3d9Device(const RAppWindow& window);
	~RD3d9Device();

public:
	FORCE_INLINE IDirect3DDevice9* GetD3dDevice() const { return m_pD3dDevice; }
	
	virtual void Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode) override;
	virtual void Present() override;
	virtual IDirect3DSurface9* GetD3dSurface() override;

	bool CheckBackBufferFormat(D3DFORMAT format);

private:
	IDirect3DDevice9*		m_pD3dDevice;
	D3DCAPS9				m_D3dDeviceCapabilites;

	unsigned int			m_u32AdapterID;
	D3DDEVTYPE				m_DeviceType;
	unsigned int			m_u32VertexProcessType;
	D3DPRESENT_PARAMETERS	m_D3dPresentParam;
};

