#include "RenderTarget.h"

#include "DisplayWindow.h"
#include <AssertUtil.h>

using namespace std;

RenderTarget::RenderTarget(DisplayWindow& window) : 
	D3D9Device			(window),
	m_pWindow			(&window),
	m_pActiveViewport	(nullptr), 
	m_IndexStreamBuffer	(*this, m_uIndexStreamBufferSize), 
	m_ShaderManager		(this)
{
	m_listVertexStreamBuffers.push_back(VertexStreamBuffer(*this, m_uVertexStreamBufferSize));
}

RenderTarget::RenderTarget(RenderTarget&& target) : 
	D3D9Device					(target),
	m_pWindow					(target.m_pWindow), 
	m_pActiveViewport			(target.m_pActiveViewport),
	m_ViewportList				(move(target.m_ViewportList)),
	m_listVertexStreamBuffers	(move(target.m_listVertexStreamBuffers)),
	m_IndexStreamBuffer			(target.m_IndexStreamBuffer),
	m_ShaderManager				(target.m_ShaderManager)
{
	target.m_pActiveViewport = nullptr;
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
	m_pActiveViewport = &m_ViewportList.back();

	return m_pActiveViewport;
}

bool RenderTarget::RemoveViewport(Viewport* pViewport)
{
	ASSERT(pViewport);

	if (pViewport == m_pActiveViewport)
	{
		m_pActiveViewport = nullptr;
	}

	m_ViewportList.remove(*pViewport);

	return true;
}

void RenderTarget::Update()
{
	// 注意，此处auto需要加上引用，否则会创建副本
	for (auto& viewport : m_ViewportList)
	{
		m_pActiveViewport = &viewport;

		viewport.Update();
	}
}
