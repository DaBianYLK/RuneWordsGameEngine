#include "Camera.h"

#include "Graphics.h"
#include "SceneManager.h"

Camera::Camera() :
	m_LookAtPosition(0.0f, 0.0f, -1.0f),
	m_FrontAxis		(0.0f, 0.0f, -1.0f),
	m_UpAxis		(0.0f, 1.0f, 0.0f),
	m_fFovy			(D3DX_PI * 0.5f),
	m_fAspect		(800.0f / 600.0f),
	m_fLookNear		(2.0f),
	m_fLookFar		(1000.0f),
	m_bLockLookAtPos(false)
{
	
}

Camera::~Camera()
{

}

void Camera::LookAtPosition(const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis)
{
	m_LookAtPosition = lookAtPos;
	m_UpAxis = upAxis;
}

void Camera::LookAtDirection(const D3DXVECTOR3& lookAtDirection, const D3DXVECTOR3& upAxis)
{
}

void Camera::SetLookAtPosition(const D3DXVECTOR3& lookAtPos)
{
	m_LookAtPosition = lookAtPos;
}

void Camera::SetFrontAxis(const D3DXVECTOR3& axis)
{
	m_FrontAxis = axis;
}

void Camera::SetUpAxis(const D3DXVECTOR3& axis)
{
	m_UpAxis = axis;
}

void Camera::SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar)
{
	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fLookNear = fLookNear;
	m_fLookFar = fLookFar;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fFovy, fAspect, fLookNear, fLookFar);
}

D3DXMATRIX* Camera::GetViewMatrix()
{
	Update();

	return &m_ViewMatrix;
}

D3DXMATRIX* Camera::GetProjectionMatrix()
{
	return &m_ProjectionMatrix;
}

void Camera::Update()
{
	if (m_bNeedUpdate)
	{
		// ��ȡ���ڵ������任����ǰ���ڵ����и���
		D3DXMATRIX* pParentTransform = m_pParent->GetTransformMatrix();
		D3DXVECTOR3 WorldPosition;

		// ���LookAtPosition����������ʹ�ø��ڵ�����任�����LookAtPosition���б任
		if (m_bLockLookAt)
		{
			
			D3DXMATRIX tempMatrix;

			ASSERT(D3DXMatrixInverse(&tempMatrix, nullptr, pParentTransform));	// ��D3DXMatrixInverse���ؿգ�˵��pParentTransform����ʽ��ֵΪ0�������������

			D3DXMatrixLookAtLH
		}
		else
		{

		}
	}

	m_bNeedUpdate = false;
}
