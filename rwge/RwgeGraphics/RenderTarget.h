#pragma once

#include <list>
#include "D3D9Device.h"
#include "Viewport.h"
#include "VertexStreamBuffer.h"
#include "IndexStreamBuffer.h"
#include "ShaderManager.h"

class DisplayWindow;

/*
RenderTarget与DisplayWindow的关系说明：
1. RenderTarget存在，则它绑定的DisplayWindow一定存在（需要开发者自行保证）
2. RenderTarget被销毁，DisplayWindow不一定会被销毁
3. RenderTarget与DisplayWindow一般是一对一的关系，但多个RenderTarget绑定在同一个DisplayWindow上是可能的
4. RenderTarget的DisplayWindow一旦被指定，就不能更改（更换Window只能创建新的RenderTarget）

RenderTarget与Viewport的关系说明：
1. Viewport仅由RenderTarget生成，外部只能通过指针或引用对其进行操作
2. Viewport属于RenderTarget的一部分，释放时与RenderTarget一起被删除

RenderTarget与RenderSystem的关系说明：
1. RenderTarget仅由RenderSystem生成，外部只能通过指针或引用对其进行操作
2. RenderTarget属于RenderSystem的一部分，释放时与RenderTarget一起被删除

与RenderTarget（D3D9Device）绑定的类有：
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
	RenderTarget(RenderTarget&& target);	// 转移构造函数
	virtual ~RenderTarget();

	bool Release() override;

public:
	Viewport* CreateViewport();
	bool RemoveViewport(Viewport* pViewport);

	void Update();

private:
	const DisplayWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;	// 创建或更新Viewport，都会更改DefaultViewport
	ViewportList					m_ViewportList;

	static const unsigned int		m_uVertexStreamBufferSize = 16 * 1024 * 1024;	// 16MB，若一个顶点64bytes，一次绘制可以支持约25000个顶点
	std::list<VertexStreamBuffer>	m_listVertexStreamBuffers;

	static const unsigned int		m_uIndexStreamBufferSize = 4 * 1024 * 1024;		// 4MB，一个索引2bytes，一次绘制可以支持约70万个三角形
	IndexStreamBuffer				m_IndexStreamBuffer;

	ShaderManager					m_ShaderManager;
};

typedef std::list<RenderTarget> RenderTargetList;