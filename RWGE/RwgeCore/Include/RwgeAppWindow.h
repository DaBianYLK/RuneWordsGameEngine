/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-12
	DESC :	
	1.	����Windows API ��װ�õ��Ĵ����࣬��������ʾ��Ⱦ��RenderTarget�е�ͼ��

	2.	����ֻ����Application ���ɺ��ͷţ�����ֹ������̳У������ɵĴ���Ĭ�������ص�

	3.	���ڴ��ڼ�������OnCreate��
		Ϊ�˱�֤RAIIԭ��RWindow��ʹ�ö��ι��죬�����޷���RWindow����ǰע���������Ҳ����˵��RAIIԭ���¼������ڵĹ�
		���¼��ǲ����ܵġ����Ҵ��ڵĴ������ɿ����߿��Ƶģ���Ҫ�ڴ��ڹ���ʱ����������ȫ����ʵ���ڼ������Ĺ��캯���У�
		���Լ�������û�ж���OnCreate������

	4.	����ȫ��ģʽ��
		A.	�Ƕ�ռȫ��		- Fake Full Screen
			�Ƕ�ռȫ��ģʽ�Ĵ��ڲ����ռ�Կ���Դ������ͬʱ�������������Ϊ��ģʽ��ʵ�ַ�ʽ�����ش��ڱ߿򣬲���������
			ʾ����㣨�������������������ڣ���
		B.	��ռȫ��		- True Full Screen
			��ռ�Կ���Դ����ȾЧ�ʻ���ߣ�ֻ�����ڳ�ʼ��D3D Device�������ڿ�������Ϊ��ռȫ����ʵ�ַ�ʽ������ D3D API 
			ʵ�֡�

	5.	ʹ�öര�ڵ�ע�����
		A.	�Ӵ��ڴ���������ʱ������D3D ��̨��������ƣ������Ĳ��ֻ���ʾΪ�ڱߣ����Ӧ�þ����������������
		B.	ֻ�������ڿ��Ա���Ϊ��ռȫ��ģʽ����ͼ������������Ϊ��ռȫ��ʱ���������Ըò���
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
	// ================ ����ģ����ܻ����ϵͳAPI�޸Ĵ��ڣ����Դ��ڲ�����Ҫ����Ӧ������ͬ�� ================
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

