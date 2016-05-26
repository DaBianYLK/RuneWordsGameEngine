/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	ͨ����Viewport��Camera�󶨣�����ʵ����Ⱦģ���볡��ģ�������
	2.	Viewportʵ������Device BackBuffer ��һ���Ӽ�����������һ����λ�����������դ�������Ⱦͼ���ŵ���黺������
	3.	��ΪRWGE���汣֤�˴�����DeviceBackBuffer�Ĵ�Сһ����һ���ģ����Կ�����ΪViewportҲ�Ǵ��ڵ�һ���Ӽ�
	4.	һ������Viewport���԰���ͬһ��Camera�������ǵ����������ǻ��������
	5.	Viewport�Ƕ����Ķ��������Ա����õ�RenderTarget�У�������Ⱦ����ʾ����
	6.	Viewportֻ��һ��D3D ������������COM ���󣬿������ⴴ����������ɾ����Ҳ����Ϊ��ˣ���ֻ�б����õ�RenderTarget
		�в�������
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include "d3d9.h"
#include <RwgeObject.h>

class RCamera;
class RD3d9RenderTarget;
class RD3d9RenderQueue;

class RD3d9Viewport : public RObject
{
public:
	RD3d9Viewport(unsigned int u32X, unsigned int u32Y, unsigned int u32Width, unsigned int u32Height);
	~RD3d9Viewport();

	void Move(unsigned int u32X, unsigned int u32Y);
	void Resize(unsigned int u32Width, unsigned int u32Height);
	void SetRect(unsigned int u32X, unsigned int u32Y, unsigned int u32Width, unsigned int u32Height);

	void UpdateCamera(RD3d9RenderQueue& renderQueue);

	FORCE_INLINE const D3DVIEWPORT9*	GetD3dViewport()		const					{ return &m_D3dViewport; };
	FORCE_INLINE const D3DRECT&			GetD3dRect()			const					{ return m_D3dRect; };
	FORCE_INLINE void					SetBackgroundColor(const D3DCOLOR& d3dColor)	{ m_D3dBackgroundColor = d3dColor; };
	FORCE_INLINE const D3DCOLOR& 		GetBackgroundColor()	const					{ return m_D3dBackgroundColor; }
	FORCE_INLINE void					SetCamera(RCamera* pCamera)						{ m_pCamera = pCamera; };
	FORCE_INLINE RCamera*				GetCamera()				const					{ return m_pCamera; };
	FORCE_INLINE void					SetRenderTarget(RD3d9RenderTarget* pTarget)		{ m_pRenderTarget = pTarget; };
	FORCE_INLINE RD3d9RenderTarget*		GetRenderTarget()		const					{ return m_pRenderTarget; };
	FORCE_INLINE float					GetMaxZ()				const					{ return m_D3dViewport.MaxZ; };

private:
	D3DVIEWPORT9		m_D3dViewport;
	D3DRECT				m_D3dRect;					// ΪClear����Ĳ���
	D3DCOLOR			m_D3dBackgroundColor;

	RCamera*			m_pCamera;					// Viewport�󶨵�Camera
	RD3d9RenderTarget*	m_pRenderTarget;			// Viewport������RenderTarget
};