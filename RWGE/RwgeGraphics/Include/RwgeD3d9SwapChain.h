/*--------------------------------------------------------------------------------------------------------------------*\
   【Create】
   AUTH :	大便一箩筐																			   DATE : 2016-04-21
   DESC :
   1.	封装D3D Swap Chain，用于支持多窗口显示，SwapChain 与子窗口绑定，Device与主窗口绑定
   2.	只能通过RenderSystem创建与释放
   3.	Viewport				对应	Device BackBuffer  		—— 直接映射
		Device BackBuffer		对应	主窗口 					—— 拉伸映射
   	Device BackBuffer		对应	SwapChain BackBuffer	—— 直接映射
   	SwapChain BackBuffer	对应	子窗口					—— 拉伸映射
   	窗口					对应	屏幕					—— 直接映射

		为了保证显示效果与逻辑的一致性，RWGE引擎中所有涉及拉伸映射的对象都保持了分辨率同步，即：DeviceBackBuffer与主
		窗口分辨率相同；SwapChainBackBuffer 与子窗口分辨率相同

   4.	关于SwapChain 与Device：
		A.	SwapChain 从属于Device
		B.	创建Device时会自动生成一个初始的SwapChain ，可以通过Device的GetSwapChain接口获取该SwapChain 的指针
		C.	初始的SwapChain 不可被替换（Release 操作对该SwapChain 无效，除非将Device也Release）
		D.	由于初始SwapChain 与附加SwapChain 性质不同，所以不提供获取Device初始SwapChain 的接口，这样可以保证RWGE中
			创建的SwapChain 都是附加SwapChain ，从而避免发生混淆。初始SwapChain 只能通过Device来操作。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeGraphics.h"
#include <RwgeObject.h>
#include <RwgeAppWindowListener.h>
#include <RwgeCoreDef.h>
#include "RwgeD3d9RenderTarget.h"

class RAppWindow;
class RD3d9Device;

class RD3d9SwapChain : public RD3d9RenderTarget
{
	friend class RD3d9RenderSystem;

private:
	RD3d9SwapChain(const RAppWindow& window);
	~RD3d9SwapChain();

public:
	FORCE_INLINE IDirect3DSwapChain9* GetD3dSwapChain() const { return m_pD3dSwapChain; }

	virtual void Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode) override;
	virtual void Present() override;
	virtual IDirect3DSurface9* GetD3dSurface() override;

private:
	IDirect3DSwapChain9*		m_pD3dSwapChain;
	D3DPRESENT_PARAMETERS		m_D3dPresentParam;
};

