#pragma once


#include <list>
#include "GraphicsDef.h"

class Camera;

/*
Viewport��Camera�Ĺ�ϵ��
1.	Viewport��Camera�Ĺ�ϵ��N��1����һ������Viewport��ͬһ��Camera
2.	Viewport��Camera�����������Ƕ����ģ�����Ӱ��
3.	Viewport������ʱ�����󶨵�Camera
*/

class Viewport
{
	friend class RenderTarget;

private:
	Viewport(IDirect3DDevice9* pDevice, unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight);

public:
	~Viewport();

	void Update();

	void Enable() const;
	void Clear() const;

	void SetRect(unsigned int uX, unsigned int uY, unsigned int uWidth, unsigned int uHeight);
	void SetBackgroundColor(const D3DCOLOR& color);

	void SetCamera(Camera* pCamera);
	Camera* GetCamera() const;

	RenderTarget* GetRenderTarget() const;

	bool operator==(const Viewport& viewport) const;

private:
	IDirect3DDevice9* m_pDevice;

	/*	
	D3DVIEWPORT9�����ԣ�
	�ӿ�λ�ã�����ڴ��ڵ����Ͻ����꣩��X, Y
	�ӿڿ�ߣ�Width, Height
	���ֵ��Χ��MinZ, MaxZ
	*/
	D3DVIEWPORT9 m_D3D9ViewportParam;
	D3DRECT m_Rect;

	Camera* m_pCamera;
	D3DCOLOR m_BackgroundColor;

	RenderTarget* m_pRenderTarget;			// ��ǰViewport�󶨵�RenderTarget
};

typedef std::list<Viewport> ViewportList;