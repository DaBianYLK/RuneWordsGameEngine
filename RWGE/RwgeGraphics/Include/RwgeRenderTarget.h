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

2016-04-06 ToDo:
	1. D3D��Ҳ��RenderTarget�ĸ���������ڽ�����Ⱦ�����������Ⱦͼ��Ļ��������������BackBuffer��Texture�ȵȡ�
	2. �������Ҷ����RenderTarget���г�ͻ�ģ���������Ķ��壬RenderTarget���ܹ��̳�D3D9Device�������ص�������Ҫ�����ع���
	3. ֮ǰ����RenderTarget��Ҫ��Ϊ��֧�ֶര����Ⱦ��Ȼ��������ֶര����ȾҲû�б�Ҫ������D3D9Device������ͨ��SwapChainʵ�֣�
	   ����D3D9Device��ȫ���Զ���Ϊ������
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

	// ��ShaderTypeָ��Ϊ��ʱ������hash���еĵ�һ��shader�����hash��Ϊ�շ��ؿ�ָ��
	Shader* GetShader(ShaderType* pShaderType = nullptr);

	VertexDeclaration* GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration);

	Texture* GetTexture(TextureInfo* pInfo);

	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void ApplyVertexStreamAndIndexStream(VertexDeclaration* pVertexDeclarationInstance);		// ���ö���������������Ӧ�Ķ�����
	void DrawPrimitive(const RenderPrimitive& primitive);

	bool operator==(const RenderTarget& renderTarget) const;

private:
	const RWindow*			m_pWindow;

	Viewport*						m_pActiveViewport;		// ���������Viewport��������ı����ǰ�������Viewport
	ViewportList					m_ViewportList;

	SceneManager*					m_pActiveSceneManager;	// ��ǰ��������ڱ���Ⱦ���ĳ��������������ڼ�¼��ǰRenderTarget��Ⱦ�ĳ����Ƿ���������Ӷ��ж��Ƿ���Ҫ����shader

	static const unsigned int		m_uVertexBufferSize = 16 * 1024 * 1024;	// 16MB����һ������64bytes��һ�λ��ƿ���֧��Լ25000������
	std::list<VertexBuffer*>		m_listVertexBuffers;

	static const unsigned int		m_uIndexBufferSize = 4 * 1024 * 1024;		// 4MB��һ������2bytes��һ�λ��ƿ���֧��Լ70���������
	IndexBuffer*					m_pIndexBuffer;

	std::hash_map<ShaderType*, Shader*>		m_hashShaders;			// ͨ����ɫ�����ͻ�ȡ��Ӧ����ɫ��ʵ��
	LPD3DXEFFECTPOOL						m_pEffectPool;			// ������Shader֮�乲����

	std::hash_map<VertexDeclarationType*, VertexDeclaration*>	m_hashVertexDeclarationInstances;	// ͨ������������ȡ��Ӧ�Ķ�������ʵ��

	std::hash_map<TextureInfo*, Texture*>	m_hashTextures;			// ͨ��������Ϣ��ȡ�������
	
	RenderState						m_CurrentRenderState;
	VertexDeclaration*				m_pCurrentVertexDeclaration;
	IndexBuffer*					m_pCurrentIndexBuffer;
};