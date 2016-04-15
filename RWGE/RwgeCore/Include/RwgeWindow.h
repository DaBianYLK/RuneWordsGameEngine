/*--------------------------------------------------------------------------------------------------------------------*\
【CREATE】	AUTH :	大便一箩筐																	   DATE : 2016-01-12
			DESC :	1.	封装Windows 提供的窗口，用于显示渲染结果
					2.	窗口只能由Application 生成和释放
					3.	刚生成的窗口默认显示
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Windows.h>
#include <string>

class RWindow
{
	friend class RApplication;

public:


private:
	RWindow(HINSTANCE hAppInstance, bool bFullScreen = false);
	RWindow(HINSTANCE hAppInstance, const char* strName, bool bFullScreen = false);
	RWindow(HINSTANCE hAppInstance, int x, int y, int width, int height);
	RWindow(HINSTANCE hAppInstance, const char* strName, int x, int y, int width, int height);

public:
	~RWindow();

private:
	void SetDefaultSize();		// 此函数仅在构造函数中使用，窗口构造完成后使用无效
	void Init(HINSTANCE hAppInstance);
	void Release();

public:
	void SetFullScreen();
	void Resize(int x, int y, int width, int height);
	void Show(bool bShow = true);
	void Hide();
	
	const std::string& GetName() const;
	HWND GetHandle() const;
	void GetSize(int& x, int&y, int& width, int& height) const;
	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsFullScreen() const;

private:
	HWND m_hWnd;
	std::string m_strName;

	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;
	bool m_bFullScreen;
	bool m_bShow;
};

