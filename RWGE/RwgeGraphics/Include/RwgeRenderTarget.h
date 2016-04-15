#pragma once

#include <list>
#include <hash_map>
#include "RwgeD3D9Device.h"
#include "RwgeViewport.h"
#include "RwgeVertexBuffer.h"
#include "RwgeIndexBuffer.h"
#include "RwgeRenderState.h"
#include "RwgeShader.h"
#include "RwgeVertexDeclaration.h"

class RenderPrimitive;
class ShaderType;
class RWindow;
class SceneManager;

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
2.	VertexBuffer
3.	IndexBuffer
4.	ShaderManager
5.	Shader
6.	Texture

2016-04-06 ToDo:
	1. D3D中也有RenderTarget的概念：可以用于接收渲染结果并保存渲染图像的缓冲对象，它可以是BackBuffer、Texture等等。
	2. 而这与我定义的RenderTarget是有冲突的，根据上面的定义，RenderTarget不能够继承D3D9Device。因此相关的内容需要进行重构。
	3. 之前定义RenderTarget主要是为了支持多窗口渲染，然而最近发现多窗口渲染也没有必要定义多个D3D9Device，可以通过SwapChain实现，
	   所以D3D9Device完全可以定义为单例。
*/

class RenderTarget : public D3D9Device
{
	friend class RenderSystem;

private:
	RenderTarget(const RWindow* pWindow);

public:
	virtual ~RenderTarget();

	bool Release() override;

	Viewport* CreateViewport();
	bool RemoveViewport(Viewport* pViewport);

	void Update();

	void SetActiveSceneManager(SceneManager* pSceneManager);
	SceneManager* GetActiveSceneManager() const;

	// 当ShaderType指针为空时，返回hash表中的第一个shader，如果hash表为空返回空指针
	Shader* GetShader(ShaderType* pShaderType = nullptr);

	VertexDeclaration* GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration);

	Texture* GetTexture(TextureInfo* pInfo);

	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void ApplyVertexStreamAndIndexStream(VertexDeclaration* pVertexDeclarationInstance);		// 设置顶点声明，并绑定相应的顶点流
	void DrawPrimitive(const RenderPrimitive& primitive);

	bool operator==(const RenderTarget& renderTarget) const;

private:
	const RWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;		// 创建或更新Viewport，都会更改变更当前被激活的Viewport
	ViewportList					m_ViewportList;

	SceneManager*					m_pActiveSceneManager;	// 当前被激活（正在被渲染）的场景管理器，用于记录当前RenderTarget渲染的场景是否发生变更，从而判断是否需要更新shader

	static const unsigned int		m_uVertexBufferSize = 16 * 1024 * 1024;	// 16MB，若一个顶点64bytes，一次绘制可以支持约25000个顶点
	std::list<VertexBuffer*>		m_listVertexBuffers;

	static const unsigned int		m_uIndexBufferSize = 4 * 1024 * 1024;		// 4MB，一个索引2bytes，一次绘制可以支持约70万个三角形
	IndexBuffer*					m_pIndexBuffer;

	std::hash_map<ShaderType*, Shader*>		m_hashShaders;			// 通过着色器类型获取相应的着色器实例
	LPD3DXEFFECTPOOL						m_pEffectPool;			// 用于在Shader之间共享常量

	std::hash_map<VertexDeclarationType*, VertexDeclaration*>	m_hashVertexDeclarationInstances;	// 通过顶点声明获取相应的顶点声明实例

	std::hash_map<TextureInfo*, Texture*>	m_hashTextures;			// 通过纹理信息获取纹理对象
	
	RenderState						m_CurrentRenderState;
	VertexDeclaration*				m_pCurrentVertexDeclaration;
	IndexBuffer*					m_pCurrentIndexBuffer;
};