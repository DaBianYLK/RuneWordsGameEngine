/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-12
	DESC :	
	1.	基于Windows API 封装得到的窗口类，它可以显示渲染到RenderTarget中的图像

	2.	窗口只能由Application 生成和释放，并禁止其他类继承，刚生成的窗口默认是隐藏的

	3.	关于窗口监听器与OnCreate：
		为了保证RAII原则，RWindow不使用二段构造，所以无法在RWindow构造前注册监听器，也就是说在RAII原则下监听窗口的构
		造事件是不可能的。而且窗口的创建是由开发者控制的，需要在窗口构造时做的事情完全可以实现在监听器的构造函数中，
		所以监听器中没有定义OnCreate函数。

	4.	两种全屏模式：
		A.	非独占全屏		- Fake Full Screen
			非独占全屏模式的窗口不会独占显卡资源，可以同时将多个窗口设置为此模式。实现方式：隐藏窗口边框，并将窗口显
			示到最顶层（覆盖任务栏和其他窗口）。
		B.	独占全屏		- True Full Screen
			独占显卡资源，渲染效率会更高，只有用于初始化D3D Device的主窗口可以设置为独占全屏。实现方式：利用 D3D API 
			实现。

	5.	使用多窗口的注意事项：
		A.	子窗口大于主窗口时，由于D3D 后台缓冲的限制，超出的部分会显示为黑边，因此应该尽量避免这种情况。
		B.	只有主窗口可以被设为独占全屏模式，试图将非主窗口设为独占全屏时，引擎会忽略该操作
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Windows.h>
#include <string>
#include <list>

#include "RwgeEnumAsByte.h"
#include "RwgeObject.h"
#include "RwgeAppWindowListener.h"
#include "RwgeCoreDef.h"

class RD3d9RenderTarget;
class RD3d9Viewport;
class RCamera;

class RAppWindow : public RObject
{
	friend class RApplication;

private:
	RAppWindow(HINSTANCE hAppInstance, const char* strName);
	~RAppWindow();

public:
	// ================ 其他模块可能会调用系统API修改窗口，所以窗口参数需要在响应函数中同步 ================
	void OnShow();
	void OnHide();
	void OnDestroy();
	void OnMove(int s32X, int s32Y);
	void OnResize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode);

	void OnKeyUp(unsigned int u32Key);
	void OnKeyDown(unsigned int u32Key);

	void Resize(int s32X, int s32Y, int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode = RwgeAppWindow::EDM_Windowed);
	void Show(bool bShow = true);

	void RegWindowListener(RwgeAppWindow::WindowListener* pListener);
	void DeRegWindowListener(RwgeAppWindow::WindowListener* pListener);

	void RegKeyBoardListener(RwgeAppWindow::KeyBoardListener* pListener);
	void DeRegKeyBoardListener(RwgeAppWindow::KeyBoardListener* pListener);
	
	FORCE_INLINE const char*	GetName()			const	{ return m_strName.c_str(); };
	FORCE_INLINE HWND			GetHandle()			const	{ return m_hWnd; };
	FORCE_INLINE int			GetX()				const	{ return m_s32X; };
	FORCE_INLINE int			GetY()				const	{ return m_s32Y; };
	FORCE_INLINE int			GetWidth()			const	{ return m_s32Width; };
	FORCE_INLINE int			GetHeight()			const	{ return m_s32Height; };
	FORCE_INLINE bool			IsShow()			const	{ return m_bShow; }
	FORCE_INLINE bool			IsFakeFullScreen()	const	{ return m_DisplayMode == RwgeAppWindow::EDM_FakeFullScreen; }
	FORCE_INLINE bool			IsTrueFullScreen()	const	{ return m_DisplayMode == RwgeAppWindow::EDM_TrueFullScreen; }

	FORCE_INLINE bool IsFullScreen()	const
	{
		return m_DisplayMode == RwgeAppWindow::EDM_FakeFullScreen || m_DisplayMode == RwgeAppWindow::EDM_TrueFullScreen;
	};

	void AddViewport(RD3d9Viewport* pViewport);
	void RemoveViewport(RD3d9Viewport* pViewport);
	void SetCamera(RCamera* pCamera);

private:
	HWND m_hWnd;
	std::string m_strName;

	int m_s32X;
	int m_s32Y;
	int m_s32Width;
	int m_s32Height;
	bool m_bShow;
	EnumAsByte<RwgeAppWindow::EDisplayMode> m_DisplayMode;

	RD3d9RenderTarget* m_pRenderTarget;

	std::list<RwgeAppWindow::WindowListener*>	m_listWindowListeners;
	std::list<RwgeAppWindow::KeyBoardListener*>	m_listKeyBoardListeners;
};

