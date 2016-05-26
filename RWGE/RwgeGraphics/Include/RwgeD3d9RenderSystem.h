/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	RenderSytem ��Ҫ����Ĺ�������֯�������Ⱦ���̣�ͬʱ����Ⱦ״̬���й���

	2.	����RenderSystem��RenderQueue��
		A.	RenderQueue��RenderSystem��һ����
		B.	����ÿ��Viewportʱ����������ǰ����RenderQueue���ᱻ���
		C.	RenderQueue����Ƴɵ�����ԭ�������Ҫ���߳���Ⱦ����Ҫʹ��˫���壬���ܻ���Ҫ���RenderQueue��

	3.	�����е�D3D ͼԪ�ύ��DP����ϣ�������EndScene����ִ����Ϸ�߼�����ִ��Present �����Գ�ַ���CPU ��GPU �첽
		ִ�е�����

	4.	��Ⱦ����ʱ����Ⱦ״̬���ո���Ƶ�ʴ��¿��Է�Ϊ������Σ�
		A.	������Ⱦ״̬	- ���ݳ�����ͷ�л�����Ⱦ״̬��ÿ����Ⱦֻ��Ҫ����һ�Σ������߷��򣬹�Դ������
		B.	��ɫ����Ⱦ״̬	- ������ɫ���л����и��µ���Ⱦ״̬
		C.	������Ⱦ״̬	- ���ݲ����л����и��µ���Ⱦ״̬
		D.	ͼԪ��Ⱦ״̬	- ����ͼԪ�л����и��µ���Ⱦ״̬��ÿ��DP����Ҫ����

	5.	��Ⱦ����Ĳο�ָ�꣺https://msdn.microsoft.com/en-us/library/windows/desktop/bb172234(v=vs.85).aspx

	ToDo :
	2016-05-20
		��Ϊһ������£��������������仯������Ƚ��٣�����ĿǰRWGEû�н������������ǵ���Ⱦ������
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

	RD3d9RenderTarget* RegWinodwForRenderTarget(RAppWindow& window);	// Ϊ���ڴ���Device��SwapChain
	bool DeRegWindowForRenderTarget(RAppWindow& window);

	void BeginScene();
	void EndScene();

	void SubmitRenderTarget(RD3d9RenderTarget* pRenderTarget);
	void SubmitFormerRenderTarget();					// �ָ�֮ǰʹ�õ�RenderTarget�����֮ǰ��RenderTargetΪ�գ���ָ�ΪĬ��
	void SubmitDefaultRenderTarget();					// ��RenderTarget����ΪD3D Device��BackBuffer
	void ClearActivedRenderTarget();					// ֻ�е�ǰ�������RenderTarget���ܱ�Clear

	void SubmitViewport(const RD3d9Viewport* pViewport);
	void SubmitFormerViewport();
	void SubmitDefaultViewport();
	void ClearViewport(const RD3d9Viewport* pViewport);	// ͨ���²���Ҫ��������ӿڣ�Clear RenderTarget�Ϳ���Clear���е�Viewport

	void SubmitRenderState(const RenderState& newRenderState);
	void SubmitMaterial(RMaterial* pNewMaterial);
	void SubmitShader(RD3d9Shader* pNewShader);			// �����ύ�յ�Shaderָ�룬��ʱ��ʾ�����ǰ�󶨵�Shader
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

	// ӳ���еĵ�һ��RenderTargetһ����D3D Device
	std::map<RAppWindow*, RD3d9RenderTarget*> m_mapWindowsToRenderTargets;	
	RD3d9RenderTarget*			m_pActivedRenderTarget;			// ��ǰ���������ȾĿ��
	RD3d9RenderTarget*			m_pFormerRenderTarget;			// ֮ǰʹ�õ���ȾĿ��

	const RD3d9Viewport*		m_pAcitvedViewport;
	const RD3d9Viewport*		m_pFormerViewport;

	RD3d9RenderQueue			m_RenderQueue;
	RenderState					m_ActivedRenderState;			// ��ǰ��Ч����Ⱦ״̬��Shader�����ʣ�
	
	const RD3d9VertexDeclaration*		m_pActivedVertexDeclaration;
	unsigned char						m_u8ActivedStreamCount;
	std::vector<const VertexStream*>	m_vecActivedVertexStreams;
	const IndexStream*					m_pActivedIndexStream;

	RVertexDeclarationManager*	m_pVertexDeclarationManager;
	RD3d9ShaderManager*			m_pShaderManager;
	RTextureManager*			m_pTextureManager;
};