/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	��Ⱦ��������RenderSystem�������ͷ�

	2.	��Ⱦ���и������Ⱦ����ִ�з���������ͬʱ�����ռ�������Ⱦ״̬������Ⱦʱ�ύ��GPU ����Ⱦ״̬�ռ���ԭ��������
		����״̬���ݵ�ָ�룬�������û���������ط������棬�ŷ����ڴ洢�����ݡ�

	2.	����MSDN��Profile ������л�����Ŀ������л�������ɫ�������Ŀ���������һ���ģ���˶���Ⱦ״̬����ʱ����������
		�볣�������岻�󣬽�����ͳһ�����ʽ������֣�Ҳ����˵һ����������ı䣬����Ϊһ���²��ʡ�
		MSDN��Profile �����https://msdn.microsoft.com/en-us/library/windows/desktop/bb172234(v=vs.85).aspx

	3.	Ϊ��Ⱦ��������ǰ�����ȸ��ݲ��ʵĻ��ģʽ�������Ƿ�Ϊ��͸�������͸���飬���ֱ�Ӧ�ò�ͬ���������
		A.	��͸���飺��ɫ��			> ���ʣ�Texture = Constant��> ��ȣ��Ƚ���Զ��
		B.	��͸���飺��ȣ���Զ�����	> ��ɫ��					> ���ʣ�Texture = Constant��
		����ʱ���Ȼ��Ʋ�͸����Ķ���Opaque��Masked�����ٻ��ư�͸����Ķ���Translucent�� Additive��Modulate����

	4.	������Ⱦ��͸�������һЩ˵����
		A.	��������
			��������£�ʹ�����������򷽷�����������ģ���͸��������Ӿ����֣����д������������
			A.	����ģ�͵Ĵ�С��ͬ������ģ��������ֵ��Ⱥ�˳�򲢲�����ʵ��ӳ����֮����Ⱥ�˳��
			B.	������͸�����巢������
			��������������������׽���ķ�����ʹ�� OIT��Order-Independent Transparency��������Depth Peeling ����һ��
			���͵�OIT ��������������������������ĸ��ʻ�������ǵ��Ӿ�Ӱ��ķ����ǽ�һ����͸������ָ�ɶ��С�İ�͸
			�����壨�������������ʧ����ͬʱ����һ��������ͬʱ������͸���벻͸���Ĳ��֣�������͸����������Ϊһ������
			������

		B.	��������
			���ڰ�͸��������Ҫ���Ȱ������˳��ִ����Ⱦ���޷�������ɫ��������Ⱦ״̬���飬����Ӧ�þ������ٻ��ư�͸����
			��ʱ����Ⱦ״̬�л��������Ż���ʽ�����¼�����
			I.	���������еİ�͸������ʹ��ͬһ����ɫ���������͸������ܶ࣬����ֻʹ����������ɫ��������֮���������
				�����λ�ù�ϵ��Ҳ�Ἣ��������ɫ�����л�Ƶ�ʣ�
			II.	���ھֲ���ȫ͸�����ֲ���ȫ��͸�������壬ʹ��Masked��Ⱦģʽ��������Translucent ��Ⱦģʽ
			III.���ư�͸������������Լ���͸������Ķ�����
			IV.	����԰�͸���������ʾЧ��Ҫ�󲻸ߣ������԰�͸�����������

	6.	����Early Z Culling��˵��
		A.	�ִ��Կ���Ӳ�����ṩ�˶�Early Z Culling��֧�֣���Forward Rendering��˵���Ȼ��ƽ��������ٻ���Զ������ſ�
			�Խ�Early Z Culling ���Ż�Ч�����
		B.	��������£�Early Z Culling �ᱻ�رգ�������Alpha Test��Pixel Shader�ж����ؽ������޳�����
		C.	��Ⱦ�����У�Early Z Culling ��ִ�еĽ׶Σ�
			����
			Vertex Shader
			Rasterizer
			Early Z Culling
			PixelShader
			����
			Alhpa Test
			Stencil\Depth Test
			Alpha Blending
			����
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeRenderUnit.h"
#include <map>
#include <list>
#include <set>
#include "RwgeShaderKey.h"

class RCamera;
class RModel;
class RD3d9Shader;
class RMaterial;
class RLight;

struct RenderState
{
	RD3d9Shader*		pShader;
	RMaterial*			pMaterial;

	RenderState() : pShader(nullptr), pMaterial(nullptr) {};

	bool operator < (const RenderState& right) const
	{
		if (pShader != right.pShader)
		{
			return pShader < right.pShader;
		}
		return pMaterial < right.pMaterial;
	}
};

// ���ڲ�͸�������ȾͼԪ����
struct RenderUnitWithDepth
{
	float			f32DepthSquare;		// ͼԪ���ֵ��ƽ��
	RRenderUnit*	pRenderUnit;

	RenderUnitWithDepth() : f32DepthSquare(0.0f), pRenderUnit(nullptr) {};
	RenderUnitWithDepth(float f32DepthSquare, RRenderUnit* pPrimitive) : f32DepthSquare(f32DepthSquare), pRenderUnit(pPrimitive) {};

	bool operator < (const RenderUnitWithDepth& right) const
	{
		return f32DepthSquare < right.f32DepthSquare;
	}
};

// ���ڰ�͸�������ȾͼԪ����
struct RenderStateWithDepth : public RenderState
{
	float f32DepthSquare;				// ͼԪ���ֵ��ƽ��

	RenderStateWithDepth() : RenderState(), f32DepthSquare(0.0f) {};
	RenderStateWithDepth(const RenderState& renderState) : RenderState(renderState), f32DepthSquare(0.0f) {};

	bool operator < (const RenderStateWithDepth& right) const
	{
		if (f32DepthSquare != right.f32DepthSquare)
		{
			return f32DepthSquare < right.f32DepthSquare;
		}

		if (pShader != right.pShader)
		{
			return pShader < right.pShader;
		}

		return pMaterial < right.pMaterial;
	}
};

typedef std::map<RenderState,			std::set<RenderUnitWithDepth>>		OpaqueGroupMap;
typedef std::map<RenderStateWithDepth,	std::list<RRenderUnit*>>			TranslucentGroupMap;

class RD3d9RenderQueue
{
	friend class RD3d9RenderSystem;

private:
	RD3d9RenderQueue();
	~RD3d9RenderQueue();

public:
	// ����������÷����ı䣨�糡���л������������ı�ȣ�������Ҫ���²����л����Shader
	FORCE_INLINE void NeedUpdateCachedMaterialShader()		{ m_bNeedUpdateCachedMaterialShader = true; };
	void SetCamera(const RCamera* pCamera);		// ��������Ӱ���й���Ⱦ״̬
	void InsertModel(RModel* pModel);
	FORCE_INLINE void SetLight(const RLight* pLight)		{ m_pLight = pLight; };
	FORCE_INLINE void SetSceneKey(const SceneKey& key)		{ m_SceneKey = key; };
	FORCE_INLINE void SetGlobalKey(const GlobalKey& key)	{ m_GlobalKey = key; };

	void Clear();								// �����Ⱦ״̬��ͼԪ

private:
	OpaqueGroupMap			m_mapOpaqueGroup;
	TranslucentGroupMap		m_mapTranslucentGroup;

	// ������
	D3DXVECTOR3				m_ViewOppositeDirection;
	const D3DXVECTOR3*		m_pCameraPosition;
	const D3DXMATRIX*		m_pViewTransform;
	const D3DXMATRIX*		m_pProjectionTransform;
	D3DXMATRIX				m_ViewProjTransform;

	const RLight*			m_pLight;

	bool					m_bNeedUpdateCachedMaterialShader;

	SceneKey				m_SceneKey;
	GlobalKey				m_GlobalKey;
};

