#pragma once

#include <Windows.h>
#include <string>

/*
窗口只能由Application生成和释放，刚生成的窗口默认是显示的
*/

class DisplayWindow
{
	friend class Application;

private:
	DisplayWindow(HINSTANCE hAppInstance, bool bFullScreen = false);
	DisplayWindow(HINSTANCE hAppInstance, const char* strName, bool bFullScreen = false);
	DisplayWindow(HINSTANCE hAppInstance, int x, int y, int width, int height);
	DisplayWindow(HINSTANCE hAppInstance, const char* strName, int x, int y, int width, int height);

public:
	~DisplayWindow();

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

