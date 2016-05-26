#include "RwgeCamera.h"

#include "RwgeAssert.h"
#include "RwgeSceneManager.h"
#include <RwgeLog.h>

RCamera::RCamera() :
	m_f32Fovy							(D3DX_PI * 0.5f),
	m_f32Aspect						(800.0f / 600.0f),
	m_f32LookNear						(2.0f),
	m_f32LookFar						(1000.0f),
	m_bCacheViewTransformOutOfDate	(false)
{
	m_NodeType = ENT_Camera;

	D3DXMatrixIdentity(&m_ViewTransform);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionTransform, m_f32Fovy, m_f32Aspect, m_f32LookNear, m_f32LookFar);
}

RCamera::~RCamera()
{

}

void RCamera::SetPerspective(float f32Fovy, float f32Aspect, float f32LookNear, float f32LookFar)
{
	m_f32Fovy = f32Fovy;
	m_f32Aspect = f32Aspect;
	m_f32LookNear = f32LookNear;
	m_f32LookFar = f32LookFar;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionTransform, f32Fovy, f32Aspect, f32LookNear, f32LookFar);
}

void RCamera::SetOrthogonal(float fW, float fH, float fLookNear, float fLookFar)
{
	D3DXMatrixOrthoLH(&m_ProjectionTransform, fW, fH, fLookNear, fLookFar);
}

const D3DXMATRIX* RCamera::GetViewTransform() const
{
	if (m_bCachedWorldTransformOutOfDate || m_bCacheViewTransformOutOfDate)
	{
		UpdateCachedViewTransform();
	}

	return &m_ViewTransform;
}

const D3DXMATRIX* RCamera::GetProjectionTransform() const
{
	return &m_ProjectionTransform;
}

void RCamera::UpdateCachedWorldTransform() const
{
	__super::UpdateCachedWorldTransform();

	// 如果相机的世界变换矩阵发生了更新，说明相机的视图变换矩阵过期
	m_bCacheViewTransformOutOfDate = true;
}

void RCamera::UpdateCachedViewTransform() const
{
	if (m_bCachedWorldTransformOutOfDate)
	{
		UpdateCachedWorldTransform();
	}

	RwgeAssert(D3DXMatrixInverse(&m_ViewTransform, nullptr, &m_WorldTransform));

	m_bCacheViewTransformOutOfDate = false;
}

void RCamera::GetSceneShot(RD3d9Viewport* pViewport, RD3d9RenderQueue& renderQueue)
{
	if (m_pSceneManager)
	{
		m_pSceneManager->RenderScene(this, pViewport, renderQueue);
	}
}