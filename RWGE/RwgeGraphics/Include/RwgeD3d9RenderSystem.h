/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	RenderSytem 主要负责的工作是组织引擎的渲染流程，同时对渲染状态进行管理

	2.	关于RenderSystem与RenderQueue：
		A.	RenderQueue是RenderSystem的一部分
		B.	绘制每个Viewport时（遍历场景前），RenderQueue都会被清空
		C.	RenderQueue不设计成单例的原因是如果要多线程渲染，需要使用双缓冲，可能会需要多个RenderQueue：

	3.	在所有的D3D 图元提交（DP）完毕，并调用EndScene后，先执行游戏逻辑，再执行Present ，可以充分发挥CPU 与GPU 异步
		执行的优势

	4.	渲染场景时，渲染状态按照更新频率大致可以分为三个层次：
		A.	场景渲染状态	- 根据场景或镜头切换的渲染状态，每次渲染只需要更新一次，如视线方向，光源参数等
		B.	着色器渲染状态	- 根据着色器切换进行更新的渲染状态
		C.	材质渲染状态	- 根据材质切换进行更新的渲染状态
		D.	图元渲染状态	- 根据图元切换进行更新的渲染状态，每次DP都需要更新

	5.	渲染排序的参考指标：https://msdn.microsoft.com/en-us/library/windows/desktop/bb172234(v=vs.85).aspx

	ToDo :
	2016-05-20
		因为一般情况下，顶点声明发生变化的情况比较少，所以目前RWGE没有将顶点声明考虑到渲染排序中
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <list>
#include <map>
#include <hash_map>
#include <RwgeCoreDef.h>
#include <RwgeSingleton.h>
#include <RwgeObject.h>
#include "RwgeD3d9RenderQueue.h"

class RD3d9Viewport;
class RenderTarget;
class RAppWindow;
class RD3d9Device;
class RD3d9SwapChain;
class RD3d9RenderTarget;
struct IDirect3D9;
class RRenderUnit; 
class RD3d9Shader;
class RSceneManager;
class RD3d9VertexDeclaration;
class RD3d9VertexBuffer;
class RD3d9IndexBuffer;
class RVertexDeclarationManager;
class RD3d9ShaderManager;
class RTextureManager;

class RD3d9RenderSystem :
	public RObject,
	public Singleton<RD3d9RenderSystem>
{
public:
	RD3d9RenderSystem();
	virtual ~RD3d9RenderSystem();

	RD3d9RenderTarget* RegWinodwForRenderTarget(RAppWindow& window);	// 为窗口创建Device或SwapChain
	bool DeRegWindowForRenderTarget(RAppWindow& window);

	void BeginScene();
	void EndScene();

	void SubmitRenderTarget(RD3d9RenderTarget* pRenderTarget);
	void SubmitFormerRenderTarget();					// 恢复之前使用的RenderTarget，如果之前的RenderTarget为空，则恢复为默认
	void SubmitDefaultRenderTarget();					// 将RenderTarget设置为D3D Device的BackBuffer
	void ClearActivedRenderTarget();					// 只有当前被激活的RenderTarget才能被Clear

	void SubmitViewport(const RD3d9Viewport* pViewport);
	void SubmitFormerViewport();
	void SubmitDefaultViewport();
	void ClearViewport(const RD3d9Viewport* pViewport);	// 通常下不需要调用这个接口，Clear RenderTarget就可以Clear所有的Viewport

	void SubmitRenderState(const RenderState& newRenderState);
	void SubmitMaterial(RMaterial* pNewMaterial);
	void SubmitShader(RD3d9Shader* pNewShader);			// 允许提交空的Shader指针，此时表示解除当前绑定的Shader
	void SubmitVertexDeclaration(const RD3d9VertexDeclaration* pVertexDeclaration);
	void SubmitVertexStream(const std::vector<VertexStream*>& vecVertexStreams);
	void SubmitIndexStream(const IndexStream* pIndexStream);
	void SubmitRenderUnit(const RRenderUnit& primitive);
	void SubmitRenderQueue(const RD3d9RenderQueue& renderQueue);

	void RenderOneFrame(float fDeltaTime);
	void PresentFrame();

	FORCE_INLINE IDirect3D9* GetD3d9() const { return m_pD3d9; };
	FORCE_INLINE const RD3d9RenderTarget* GetActivedRenderTarget()	const { return m_pActivedRenderTarget; };
	FORCE_INLINE const RD3d9RenderQueue&  GetRenderQueue()			const { return m_RenderQueue; };

private:
	IDirect3D9*					m_pD3d9;
	RD3d9Device*				m_pDevice;

	// 映射中的第一个RenderTarget一定是D3D Device
	std::map<RAppWindow*, RD3d9RenderTarget*> m_mapWindowsToRenderTargets;	
	RD3d9RenderTarget*			m_pActivedRenderTarget;			// 当前被激活的渲染目标
	RD3d9RenderTarget*			m_pFormerRenderTarget;			// 之前使用的渲染目标

	const RD3d9Viewport*		m_pAcitvedViewport;
	const RD3d9Viewport*		m_pFormerViewport;

	RD3d9RenderQueue			m_RenderQueue;
	RenderState					m_ActivedRenderState;			// 当前生效的渲染状态（Shader、材质）
	
	const RD3d9VertexDeclaration*		m_pActivedVertexDeclaration;
	unsigned char						m_u8ActivedStreamCount;
	std::vector<const VertexStream*>	m_vecActivedVertexStreams;
	const IndexStream*					m_pActivedIndexStream;

	RVertexDeclarationManager*	m_pVertexDeclarationManager;
	RD3d9ShaderManager*			m_pShaderManager;
	RTextureManager*			m_pTextureManager;
};