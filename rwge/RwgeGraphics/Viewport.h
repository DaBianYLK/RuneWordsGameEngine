#pragma once


#include <list>
#include "GraphicsDefinitions.h"

class Camera;

/*
Viewport与Camera的关系：
1.	Viewport与Camera的关系是一对一，或多个Viewport对一个Camera
2.	Viewport与Camera的生命周期是独立的，互不影响
3.	Viewport可以随时更换绑定的Camera
*/

class Viewport
{
	friend class RenderTarget;

private:
	Viewport(IDirect3DDevice9* pDevice, unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight);
	~Viewport();

public:
	void Update() const;

	void Enable() const;
	void Clear() const;

	void SetRect(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight);
	void SetBackgroundColor(const D3DCOLOR& color);

	void SetCamera(Camera* pCamera);
	Camera* GetCamera() const;

private:
	IDirect3DDevice9* m_pDevice;

	/*	
	D3DVIEWPORT9的属性：
	视口位置（相对于窗口的左上角坐标）：X, Y
	视口宽高：Width, Height
	深度值范围：MinZ, MaxZ
	*/
	D3DVIEWPORT9 m_D3D9ViewportParam;
	D3DRECT m_Rect;

	Camera* m_pCamera;
	D3DCOLOR m_BackgroundColor;
};

typedef std::list<Viewport> ViewportList;