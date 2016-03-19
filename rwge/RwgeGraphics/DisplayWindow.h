#pragma once

#include <Windows.h>
#include <string>

/*
����ֻ����Application���ɺ��ͷţ������ɵĴ���Ĭ������ʾ��
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
	void SetDefaultSize();		// �˺������ڹ��캯����ʹ�ã����ڹ�����ɺ�ʹ����Ч
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

