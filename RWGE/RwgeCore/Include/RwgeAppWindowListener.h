/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-04-25
	DESC :	RAppWindow�ļ�������������Ӧ�����¼�
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once



namespace RwgeAppWindow
{
	enum EDisplayMode
	{
		EDM_Windowed,			// ����ģʽ
		EDM_FakeFullScreen,		// �Ƕ�ռȫ��
		EDM_TrueFullScreen,		// ��ռȫ��

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