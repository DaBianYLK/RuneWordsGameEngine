/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	关于RenderSystem与RenderQueue：
		A.	RenderQueue是RenderSystem的一部分，所有RenderTarget共用同一个RenderQueue
		B.	绘制每个Viewport时（遍历场景前），RenderQueue都会被清空
		C.	RenderQueue不设计成单例的原因是如果要多线程渲染，可能会需要多个RenderQueue：
			I.	实现多线程渲染时需要使用双缓冲，此时需要2个RenderQueue
			II.	若需要支持同时渲染多个视口，需要多个RenderQueue

\*--------------------------------------------------------------------------------------------------------------------*/


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
*/
#pragma once

#include "RwgeRenderQueue.h"
#include "RwgeD3d9RenderTarget.h"
#include "RwgeVertexDeclarationManager.h"
#include "RwgeShaderManager.h"
#include "RwgeTextureManager.h"
#include <RwgeSingleton.h>
#include <RwgeObject.h>

struct IDirect3D9;
class RenderPrimitive;


#include <list>
#include <hash_map>
#include "RwgeD3d9Device.h"
#include "RwgeViewport.h"
#include "RwgeVertexBuffer.h"
#include "RwgeIndexBuffer.h"
#include "RwgeRenderState.h"
#include "RwgeShader.h"
#include "RwgeVertexDeclaration.h"

class RenderPrimitive;
class ShaderType;
class RAppWindow;
class SceneManager;




class RD3d9RenderSystem :
	public RObject,
	public Singleton<RD3d9RenderSystem>
{
public:
	RD3d9RenderSystem();
	virtual ~RD3d9RenderSystem();

private:
	bool CreateDevice(const RAppWindow& window);
	bool DestroyDevice();

public:
	void RenderOneFrame(float fDeltaTime);
	void RenderScene(SceneManager* pSceneManager);
	void DrawRenderQueue();
	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void DrawPrimitive(const RenderPrimitive& primitive);

	IDirect3D9* GetD3d9() const;

	RenderTarget* CreateRenderTarget(const RAppWindow* pWindow);
	bool RemoveRenderTarget(RenderTarget* pTarget);

	RenderTarget* GetActiveRenderTarget() const;

	RenderQueue* GetRenderQueuePtr();

private:
	IDirect3D9* m_pD3D9;

	RenderTarget* m_pActiveRenderTarget;			// 当前被激活的渲染目标
	std::list<RenderTarget*> m_listRenderTarget;	// 渲染目标列表

	RenderQueue m_RenderQueue;

	VertexDeclarationManager	m_VertexDeclarationManager;
	RShaderManager				m_ShaderManager;
	TextureManager				m_TextureManager;










	bool Release() override;

	Viewport* CreateViewport();
	bool RemoveViewport(Viewport* pViewport);

	void Update();

	void SetActiveSceneManager(SceneManager* pSceneManager);
	SceneManager* GetActiveSceneManager() const;

	// 当ShaderType指针为空时，返回hash表中的第一个shader，如果hash表为空返回空指针
	RShader* GetShader(ShaderType* pShaderType = nullptr);

	VertexDeclaration* GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration);

	Texture* GetTexture(TextureInfo* pInfo);

	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void ApplyVertexStreamAndIndexStream(VertexDeclaration* pVertexDeclarationInstance);		// 设置顶点声明，并绑定相应的顶点流
	void DrawPrimitive(const RenderPrimitive& primitive);

	bool operator==(const RenderTarget& renderTarget) const;

private:
	const RAppWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;		// 创建或更新Viewport，都会更改变更当前被激活的Viewport
	ViewportList					m_ViewportList;

	SceneManager*					m_pActiveSceneManager;	// 当前被激活（正在被渲染）的场景管理器，用于记录当前RenderTarget渲染的场景是否发生变更，从而判断是否需要更新shader

	static const unsigned int		m_uVertexBufferSize = 16 * 1024 * 1024;	// 16MB，若一个顶点64bytes，一次绘制可以支持约25000个顶点
	std::list<VertexBuffer*>		m_listVertexBuffers;

	static const unsigned int		m_uIndexBufferSize = 4 * 1024 * 1024;		// 4MB，一个索引2bytes，一次绘制可以支持约70万个三角形
	IndexBuffer*					m_pIndexBuffer;

	std::hash_map<ShaderType*, RShader*>		m_hashShaders;			// 通过着色器类型获取相应的着色器实例
	LPD3DXEFFECTPOOL						m_pEffectPool;			// 用于在Shader之间共享常量

	std::hash_map<VertexDeclarationType*, VertexDeclaration*>	m_hashVertexDeclarationInstances;	// 通过顶点声明获取相应的顶点声明实例

	std::hash_map<TextureInfo*, Texture*>	m_hashTextures;			// 通过纹理信息获取纹理对象

	RenderState						m_CurrentRenderState;
	VertexDeclaration*				m_pCurrentVertexDeclaration;
	IndexBuffer*					m_pCurrentIndexBuffer;
};

#define g_RenderSystem RRenderSystem::GetInstance()