#pragma once

#include <list>
#include "D3D9Device.h"
#include "Viewport.h"

class DisplayWindow;

/*
RenderTarget��DisplayWindow�Ĺ�ϵ˵����
1. RenderTarget���ڣ������󶨵�DisplayWindowһ�����ڣ���Ҫ���������б�֤��
2. RenderTarget�����٣�DisplayWindow��һ���ᱻ����
3. RenderTarget��DisplayWindowһ����һ��һ�Ĺ�ϵ�������RenderTarget����ͬһ��DisplayWindow���ǿ��ܵ�
4. RenderTarget��DisplayWindowһ����ָ�����Ͳ��ܸ��ģ�����Windowֻ�ܴ����µ�RenderTarget��

RenderTarget��Viewport�Ĺ�ϵ˵����
1. Viewport����RenderTarget���ɣ��ⲿֻ��ͨ��ָ������ö�����в���
2. Viewport����RenderTarget��һ���֣��ͷ�ʱ��RenderTargetһ��ɾ��

RenderTarget��RenderSystem�Ĺ�ϵ˵����
1. RenderTarget����RenderSystem���ɣ��ⲿֻ��ͨ��ָ������ö�����в���
2. RenderTarget����RenderSystem��һ���֣��ͷ�ʱ��RenderTargetһ��ɾ��
*/

class RenderTarget : public D3D9Device
{
	friend class RenderSystem;

private:
	RenderTarget(DisplayWindow& window);
	RenderTarget(RenderTarget&& target);	// ת�ƹ��캯��
	virtual ~RenderTarget();

	bool Release() override;

public:
	Viewport* CreateViewport();
	bool RemoveViewport(Viewport* pViewport);

	void Update();

private:
	const DisplayWindow* m_pWindow;

	Viewport* m_pActiveViewport;	// ���������Viewport���������DefaultViewport
	ViewportList m_ViewportList;
};

typedef std::list<RenderTarget> RenderTargetList;