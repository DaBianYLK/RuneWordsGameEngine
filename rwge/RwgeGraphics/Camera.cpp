#include "Camera.h"

#include "Graphics.h"
#include "SceneManager.h"

const D3DXVECTOR3	DefaultCameraPosition(0.0f, 0.0f, 10.0f);
const D3DXVECTOR3	DefaultCameraLookAtOffset(0.0f, 0.0f, -10.0f);
const D3DXVECTOR3	DefaultCameraUpAxis(0.0f, 1.0f, 0.0f);
const bool			DefaultLockLookAt = false;
const float			DefaultFovy = D3DX_PI * 0.5f;
const float			DefaultAspect = 800.0f / 600.0f;
const float			DefaultLookNear = 2.0f;
const float			DefaultLookFar = 1000.0f;

Camera::Camera() :
	m_LookAtPosition(DefaultCameraPosition + DefaultCameraLookAtOffset),
	m_UpAxis(DefaultCameraUpAxis),
	m_bLockLookAt(DefaultLockLookAt),
	m_fFovy(DefaultFovy),
	m_fAspect(DefaultAspect),
	m_fLookNear(DefaultLookNear),
	m_fLookFar(DefaultLookFar)
{
	m_Position = DefaultCameraPosition;
}

Camera::Camera(const D3DXVECTOR3& pos) :
	m_LookAtPosition(DefaultCameraPosition + DefaultCameraLookAtOffset),
	m_UpAxis(DefaultCameraUpAxis),
	m_bLockLookAt(DefaultLockLookAt),
	m_fFovy(DefaultFovy),
	m_fAspect(DefaultAspect),
	m_fLookNear(DefaultLookNear),
	m_fLookFar(DefaultLookFar)
{
	m_Position = pos;
}

Camera::Camera(const D3DXVECTOR3& pos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis, bool bLockLookAt) :
	m_LookAtPosition(lookAtPos),
	m_UpAxis(upAxis),
	m_bLockLookAt(bLockLookAt),
	m_fFovy(DefaultFovy),
	m_fAspect(DefaultAspect),
	m_fLookNear(DefaultLookNear),
	m_fLookFar(DefaultLookFar)
{
	m_Position = pos;
}

Camera::Camera(const D3DXVECTOR3& pos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis, bool bLockLookAt, 
	float fFovy, float fAspect, float fLookNear, float fLookFar) :
	m_LookAtPosition(lookAtPos),
	m_UpAxis(upAxis),
	m_bLockLookAt(bLockLookAt),
	m_fFovy(fFovy),
	m_fAspect(fAspect),
	m_fLookNear(fLookNear),
	m_fLookFar(fLookFar)
{
	m_Position = pos;
}

Camera::~Camera()
{

}

void Camera::SetUpAxis(const D3DXVECTOR3& upAxis)
{
	m_UpAxis = upAxis;
}

void Camera::LookAtPosition(const D3DXVECTOR3& lookAtPos)
{
	m_LookAtPosition = lookAtPos;
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
