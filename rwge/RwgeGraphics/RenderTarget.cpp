#include "RenderTarget.h"

#include "GraphicsDefinitions.h"

using namespace std;

RenderTarget::RenderTarget(DisplayWindow& window) : D3D9Device(window)
{
	m_pWindow = &window;

	Init();
}

RenderTarget::RenderTarget(RenderTarget&& target) : 
	D3D9Device(target),
	m_pWindow(target.m_pWindow),
	m_ViewportList(move(target.m_ViewportList))
{
	target.m_pWindow = nullptr;
}

RenderTarget::~RenderTarget()
{
	RenderTarget::Release();
}

bool RenderTarget::Init()
{
	m_ViewportList.clear();

	return true;
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
	m_ViewportList.emplace_back(Viewport());

	return false;
}

bool RenderTarget::RemoveViewport(Viewport* pViewport)
{

}
