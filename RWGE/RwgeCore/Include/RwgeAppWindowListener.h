/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-04-25
	DESC :	RAppWindow的监听器，用于响应窗口事件
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once



namespace RwgeAppWindow
{
	enum EDisplayMode
	{
		EDM_Windowed,			// 窗口模式
		EDM_FakeFullScreen,		// 非独占全屏
		EDM_TrueFullScreen,		// 独占全屏

		EDisplayMode_MAX
	};

	class WindowListener
	{
	public:
		WindowListener()			{};
		virtual ~WindowListener()	{};

		virtual void OnShow()		{};
		virtual void OnHide()		{};
		virtual void OnDestroy()	{};
		virtual void OnMove(int s32X, int s32Y) {};
		virtual void OnResize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode) {};
	};

	class KeyBoardListener
	{
	public:
		KeyBoardListener()			{};
		virtual ~KeyBoardListener()	{};

		virtual void OnKeyUp(unsigned int u32Key)	{};
		virtual void OnKeyDown(unsigned int u32Key) {};
	};
}