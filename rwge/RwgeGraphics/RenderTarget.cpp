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
	// clear����ֻ�����ñ�־����������ջ���
	// ����һ���յ���ʱ���������swap���������ڴ棬��������ʱ��������������ͷ��ڴ�(�����Ҫ�����ͷſ���ͨ���������޶���ʱ������������)
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
