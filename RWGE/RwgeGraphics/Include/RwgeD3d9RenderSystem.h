/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	����RenderSystem��RenderQueue��
		A.	RenderQueue��RenderSystem��һ���֣�����RenderTarget����ͬһ��RenderQueue
		B.	����ÿ��Viewportʱ����������ǰ����RenderQueue���ᱻ���
		C.	RenderQueue����Ƴɵ�����ԭ�������Ҫ���߳���Ⱦ�����ܻ���Ҫ���RenderQueue��
			I.	ʵ�ֶ��߳���Ⱦʱ��Ҫʹ��˫���壬��ʱ��Ҫ2��RenderQueue
			II.	����Ҫ֧��ͬʱ��Ⱦ����ӿڣ���Ҫ���RenderQueue

\*--------------------------------------------------------------------------------------------------------------------*/


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

	RenderTarget* m_pActiveRenderTarget;			// ��ǰ���������ȾĿ��
	std::list<RenderTarget*> m_listRenderTarget;	// ��ȾĿ���б�

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

	// ��ShaderTypeָ��Ϊ��ʱ������hash���еĵ�һ��shader�����hash��Ϊ�շ��ؿ�ָ��
	RShader* GetShader(ShaderType* pShaderType = nullptr);

	VertexDeclaration* GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration);

	Texture* GetTexture(TextureInfo* pInfo);

	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void ApplyVertexStreamAndIndexStream(VertexDeclaration* pVertexDeclarationInstance);		// ���ö���������������Ӧ�Ķ�����
	void DrawPrimitive(const RenderPrimitive& primitive);

	bool operator==(const RenderTarget& renderTarget) const;

private:
	const RAppWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;		// ���������Viewport��������ı����ǰ�������Viewport
	ViewportList					m_ViewportList;

	SceneManager*					m_pActiveSceneManager;	// ��ǰ��������ڱ���Ⱦ���ĳ��������������ڼ�¼��ǰRenderTarget��Ⱦ�ĳ����Ƿ���������Ӷ��ж��Ƿ���Ҫ����shader

	static const unsigned int		m_uVertexBufferSize = 16 * 1024 * 1024;	// 16MB����һ������64bytes��һ�λ��ƿ���֧��Լ25000������
	std::list<VertexBuffer*>		m_listVertexBuffers;

	static const unsigned int		m_uIndexBufferSize = 4 * 1024 * 1024;		// 4MB��һ������2bytes��һ�λ��ƿ���֧��Լ70���������
	IndexBuffer*					m_pIndexBuffer;

	std::hash_map<ShaderType*, RShader*>		m_hashShaders;			// ͨ����ɫ�����ͻ�ȡ��Ӧ����ɫ��ʵ��
	LPD3DXEFFECTPOOL						m_pEffectPool;			// ������Shader֮�乲����

	std::hash_map<VertexDeclarationType*, VertexDeclaration*>	m_hashVertexDeclarationInstances;	// ͨ������������ȡ��Ӧ�Ķ�������ʵ��

	std::hash_map<TextureInfo*, Texture*>	m_hashTextures;			// ͨ��������Ϣ��ȡ�������

	RenderState						m_CurrentRenderState;
	VertexDeclaration*				m_pCurrentVertexDeclaration;
	IndexBuffer*					m_pCurrentIndexBuffer;
};

#define g_RenderSystem RRenderSystem::GetInstance()