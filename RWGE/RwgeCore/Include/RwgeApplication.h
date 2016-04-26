/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	�ͻ��˵���ѭ������࣬�����˳�����������ڣ������߿��Լ̳�AppDelegateʵ�ֶԳ����������ڵļ���
	2.	�������������ģ��ĳ�ʼ�������ٹ���
	3.	���򴴽��ĵ�һ�����ڳ�ΪPrimaryWindow��һ�������رգ���������ͻ��˳�
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <Windows.h>
#include <RwgeSingleton.h>
#include "RwgeFpsController.h"
#include <map>
#include <string>

#include "RwgeObject.h"

class RAppDelegate;
class RInputManager;
class RD3d9RenderSystem;
class RAppWindow;

class RApplication : 
	public RObject,
	public Singleton<RApplication>
{
public:
	class AppDelegate
	{
	public:
		AppDelegate()									{};
		virtual ~AppDelegate()							{};

		virtual void OnCreate()							{};
		virtual void OnUpdateFrame(float f32DeltaTime)	{};
		virtual void OnDestroy()						{};
	};

public:
	RApplication();
	~RApplication();

	static void SetDelegate(AppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle() const;
	float GetTimeSinceLastFrame() const;

	RAppWindow* CreateAppWindow(const char* pName);
	RAppWindow* GetAppWindow(const char* pName);
	RAppWindow* GetPrimaryWindow();
	bool DestroyAppWindow(const char* pName);

	float GetCurrentFPS() const;

private:
	static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT u32Message, WPARAM wParam, LPARAM lParam);
	void UpdateFrame();

private:
	HINSTANCE m_hInstance;
	RFpsController m_FPSController;

	static AppDelegate* m_pDelegate;
	RD3d9RenderSystem* m_pRenderSystem;
	RInputManager* m_pInputManager;

	std::map<std::string, RAppWindow*> m_mapAppWindows;
};

