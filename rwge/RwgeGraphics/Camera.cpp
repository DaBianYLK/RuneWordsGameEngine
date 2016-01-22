#include "Camera.h"

#include "Graphics.h"
#include "SceneManager.h"

Camera::Camera() :
	m_fFovy						(D3DX_PI * 0.5f),
	m_fAspect					(800.0f / 600.0f),
	m_fLookNear					(2.0f),
	m_fLookFar					(1000.0f),
	m_bCacheViewMatrixOutOfDate	(false)
{
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_fFovy, m_fAspect, m_fLookNear, m_fLookFar);
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

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fFovy, fAspect, fLookNear, fLookFar);
}

const D3DXMATRIX* Camera::GetViewMatrix() const
{
	if (m_bCachedWorldTransformOutOfDate || m_bCacheViewMatrixOutOfDate)
	{
		UpdateViewMatrix();
	}

	return &m_ViewMatrix;
}

const D3DXMATRIX* Camera::GetProjectionMatrix() const
{
	return &m_ProjectionMatrix;
}

void Camera::UpdateCachedWorldTransform() const
{
	__super::UpdateCachedWorldTransform();

	// 如果相机的世界变换矩阵发生了更新，说明相机的视图变换矩阵过期
	m_bCacheViewMatrixOutOfDate = true;
}

void Camera::UpdateViewMatrix() const
{
	if (m_bCachedWorldTransformOutOfDate)
	{
		UpdateCachedWorldTransform();
	}
	
	float result = 0;
	ASSERT(D3DXMatrixInverse(&m_ViewMatrix, &result, &m_WorldTransform));

	m_bCacheViewMatrixOutOfDate = false;
}