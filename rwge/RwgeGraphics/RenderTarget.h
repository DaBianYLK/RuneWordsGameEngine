#pragma once

#include <list>
#include "D3D9Device.h"
#include "Viewport.h"
#include "VertexStreamBuffer.h"
#include "IndexStreamBuffer.h"
#include "ShaderManager.h"

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

��RenderTarget��D3D9Device���󶨵����У�
1.	VertexDeclaration
2.	VertexStreamBuffer
3.	IndexStreamBuffer
4.	ShaderManager
5.	ShaderProgram
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
	const DisplayWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;	// ���������Viewport���������DefaultViewport
	ViewportList					m_ViewportList;

	static const unsigned int		m_uVertexStreamBufferSize = 16 * 1024 * 1024;	// 16MB����һ������64bytes��һ�λ��ƿ���֧��Լ25000������
	std::list<VertexStreamBuffer>	m_listVertexStreamBuffers;

	static const unsigned int		m_uIndexStreamBufferSize = 4 * 1024 * 1024;		// 4MB��һ������2bytes��һ�λ��ƿ���֧��Լ70���������
	IndexStreamBuffer				m_IndexStreamBuffer;

	ShaderManager					m_ShaderManager;
};

typedef std::list<RenderTarget> RenderTargetList;