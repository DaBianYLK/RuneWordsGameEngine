#include "RenderTarget.h"

#include "DisplayWindow.h"
#include <AssertUtil.h>

using namespace std;

RenderTarget::RenderTarget(DisplayWindow& window) : 
	D3D9Device			(window),
	m_pWindow			(&window),
	m_pDefaultViewport	(nullptr)
{

}

RenderTarget::RenderTarget(RenderTarget&& target) : 
	D3D9Device			(target),
	m_pWindow			(target.m_pWindow), 
	m_pDefaultViewport	(target.m_pDefaultViewport),
	m_ViewportList		(move(target.m_ViewportList))
{
	target.m_pDefaultViewport = nullptr;
}

RenderTarget::~RenderTarget()
{
	
}

bool RenderTarget::Release()
{
	// clear函数只会重置标志，而不会清空缓存
	// 创建一个空的临时链表和利用swap函数交换内存，再利用临时链表的析构函数释放内存(如果需要立即释放可以通过大括号限定临时变量的作用域)
	ViewportList emptyList;
	m_ViewportList.swap(emptyList);
	
	return __super::Release();
}

Viewport* RenderTarget::CreateViewport()
{
	m_ViewportList.emplace_back(Viewport(m_pDevice, 0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight()));
	m_pDefaultViewport = &m_ViewportList.back();

	return m_pDefaultViewport;
}

bool RenderTarget::RemoveViewport(Viewport* pViewport)
{
	ASSERT(pViewport);

	if (pViewport == m_pDefaultViewport)
	{
		m_pDefaultViewport = nullptr;
	}

	m_ViewportList.remove(*pViewport);

	return true;
}

void RenderTarget::Update()
{
	for (auto viewport : m_ViewportList)
	{
		m_pDefaultViewport = &viewport;

		viewport.Update();
	}
}
