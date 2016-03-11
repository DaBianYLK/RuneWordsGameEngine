#include "Camera.h"

#include "AssertUtil.h"
#include "SceneManager.h"

Camera::Camera() :
	m_fFovy							(D3DX_PI * 0.5f),
	m_fAspect						(800.0f / 600.0f),
	m_fLookNear						(2.0f),
	m_fLookFar						(1000.0f),
	m_bCacheViewTransformOutOfDate	(false)
{
	m_NodeType = NT_Camera;

	D3DXMatrixIdentity(&m_ViewTransform);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionTransform, m_fFovy, m_fAspect, m_fLookNear, m_fLookFar);
}

Camera::~Camera()
{

}

void Camera::SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar)
{
	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fLookNear = fLookNear;
	m_fLookFar = fLookFar;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionTransform, fFovy, fAspect, fLookNear, fLookFar);
}

const D3DXMATRIX* Camera::GetViewTransform() const
{
	if (m_bCachedWorldTransformOutOfDate || m_bCacheViewTransformOutOfDate)
	{
		UpdateCachedViewTransform();
	}

	return &m_ViewTransform;
}

const D3DXMATRIX* Camera::GetProjectionTransform() const
{
	return &m_ProjectionTransform;
}

void Camera::UpdateCachedWorldTransform() const
{
	__super::UpdateCachedWorldTransform();

	// 如果相机的世界变换矩阵发生了更新，说明相机的视图变换矩阵过期
	m_bCacheViewTransformOutOfDate = true;
}

void Camera::UpdateCachedViewTransform() const
{
	if (m_bCachedWorldTransformOutOfDate)
	{
		UpdateCachedWorldTransform();
	}
	
	float result = 0;
	ASSERT(D3DXMatrixInverse(&m_ViewTransform, &result, &m_WorldTransform));

	m_bCacheViewTransformOutOfDate = false;
}

void Camera::RenderScene(Viewport* pViewport)
{
	if (m_pSceneManager)
	{
		m_pSceneManager->RenderScene(pViewport);
	}
}