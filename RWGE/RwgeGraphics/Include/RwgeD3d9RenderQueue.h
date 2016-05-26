/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	渲染队列能由RenderSystem创建与释放

	2.	渲染队列负责对渲染对象执行分类与排序，同时它会收集所有渲染状态，在渲染时提交给GPU 。渲染状态收集的原则是优先
		储存状态数据的指针，如果数据没有在其他地方被储存，才分配内存储存数据。

	2.	根据MSDN的Profile 结果，切换纹理的开销与切换顶点着色器常量的开销几乎是一样的，因此对渲染状态排序时，区分纹理
		与常量的意义不大，将它们统一按材质进行区分，也就是说一旦纹理或常量改变，就视为一种新材质。
		MSDN的Profile 结果：https://msdn.microsoft.com/en-us/library/windows/desktop/bb172234(v=vs.85).aspx

	3.	为渲染对象排序前，首先根据材质的混合模式，将它们分为不透明组与半透明组，并分别应用不同的排序规则：
		A.	不透明组：着色器			> 材质（Texture = Constant）> 深度（先近后远）
		B.	半透明组：深度（先远后近）	> 着色器					> 材质（Texture = Constant）
		绘制时，先绘制不透明组的对象（Opaque、Masked），再绘制半透明组的对象（Translucent、 Additive、Modulate）。

	4.	关于渲染半透明物体的一些说明：
		A.	关于排序
			多数情况下，使用上述的排序方法，可以正常模拟半透明物体的视觉表现，但有存在例外情况：
			A.	由于模型的大小不同，根据模型深度区分的先后顺序并不能真实反映网格之间的先后顺序
			B.	两个半透明物体发生交叉
			对于上面两种情况，彻底解决的方法是使用 OIT（Order-Independent Transparency）技术，Depth Peeling 就是一种
			典型的OIT 技术；减少上面两种情况发生的概率或减轻它们的视觉影响的方法是将一个半透明物体分割成多个小的半透
			明物体（可能造成性能损失），同时避免一个网格中同时包含半透明与不透明的部分（即将半透明部分设置为一个独立
			的网格）

		B.	关于性能
			由于半透明物体需要优先按照深度顺序执行渲染，无法按照着色器进行渲染状态分组，所以应该尽量减少绘制半透明物
			体时的渲染状态切换，具体优化方式有如下几个：
			I.	尽量让所有的半透明物体使用同一个着色器（如果半透明物体很多，就算只使用了两种着色器，它们之间如果存在
				交替的位置关系，也会极大增加着色器的切换频率）
			II.	对于局部完全透明、局部完全不透明的物体，使用Masked渲染模式，而不是Translucent 渲染模式
			III.限制半透明物体的数量以及半透明网格的顶点数
			IV.	如果对半透明物体的显示效果要求不高，放弃对半透明物体的排序

	6.	关于Early Z Culling的说明
		A.	现代显卡在硬件上提供了对Early Z Culling的支持，对Forward Rendering来说，先绘制近处物体再绘制远处物体才可
			以将Early Z Culling 的优化效果最大化
		B.	部分情况下，Early Z Culling 会被关闭：开启了Alpha Test；Pixel Shader中对像素进行了剔除操作
		C.	渲染管线中，Early Z Culling 的执行的阶段：
			……
			Vertex Shader
			Rasterizer
			Early Z Culling
			PixelShader
			……
			Alhpa Test
			Stencil\Depth Test
			Alpha Blending
			……
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

// 用于不透明组的渲染图元排序
struct RenderUnitWithDepth
{
	float			f32DepthSquare;		// 图元深度值的平方
	RRenderUnit*	pRenderUnit;

	RenderUnitWithDepth() : f32DepthSquare(0.0f), pRenderUnit(nullptr) {};
	RenderUnitWithDepth(float f32DepthSquare, RRenderUnit* pPrimitive) : f32DepthSquare(f32DepthSquare), pRenderUnit(pPrimitive) {};

	bool operator < (const RenderUnitWithDepth& right) const
	{
		return f32DepthSquare < right.f32DepthSquare;
	}
};

// 用于半透明组的渲染图元排序
struct RenderStateWithDepth : public RenderState
{
	float f32DepthSquare;				// 图元深度值的平方

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
	// 如果环境设置发生改变（如场景切换、场景发生改变等），就需要更新材质中缓存的Shader
	FORCE_INLINE void NeedUpdateCachedMaterialShader()		{ m_bNeedUpdateCachedMaterialShader = true; };
	void SetCamera(const RCamera* pCamera);		// 设置与摄影机有关渲染状态
	void InsertModel(RModel* pModel);
	FORCE_INLINE void SetLight(const RLight* pLight)		{ m_pLight = pLight; };
	FORCE_INLINE void SetSceneKey(const SceneKey& key)		{ m_SceneKey = key; };
	FORCE_INLINE void SetGlobalKey(const GlobalKey& key)	{ m_GlobalKey = key; };

	void Clear();								// 清空渲染状态与图元

private:
	OpaqueGroupMap			m_mapOpaqueGroup;
	TranslucentGroupMap		m_mapTranslucentGroup;

	// 相机相关
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

