#include "Camera.h"

#include "AppConfig.h"
#include "Graphics.h"
#include "Window.h"

Camera::Camera() {
	m_Position = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_RightAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_ViewMatrix = D3DXMATRIX();
	m_ProjectionMatrix = D3DXMATRIX();

	D3DXMatrixPerspectiveFovLH(
		&m_ProjectionMatrix,
		D3DX_PI * 0.5f, // 90 - degree
		(float)Graphics::GetInstance()->GetWindow()->GetWidth() / (float)Graphics::GetInstance()->GetWindow()->GetHeight(),
		1.0f,
		1000.0f);
}

Camera::Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rightAxis, const D3DXVECTOR3& upAxis, const D3DXVECTOR3& lookAxis) {
	m_Position = position;
	m_RightAxis = rightAxis;
	m_UpAxis = upAxis;
	m_LookAxis = lookAxis;

	m_ViewMatrix = D3DXMATRIX();
	m_ProjectionMatrix = D3DXMATRIX();

	D3DXMatrixPerspectiveFovLH(
		&m_ProjectionMatrix,
		D3DX_PI * 0.25f, // 90 - degree
		(float)Graphics::GetInstance()->GetWindow()->GetWidth() / (float)Graphics::GetInstance()->GetWindow()->GetHeight(),
		30.0f,
		1000.0f);
}

Camera::~Camera() {

}

D3DXMATRIX* Camera::GetViewMatrix() {
	// 标准化相机的前，上，右三个轴，并确保它们保持正交
	D3DXVec3Normalize(&m_LookAxis, &m_LookAxis);

	D3DXVec3Cross(&m_UpAxis, &m_LookAxis, &m_RightAxis);
	D3DXVec3Normalize(&m_UpAxis, &m_UpAxis);

	D3DXVec3Cross(&m_RightAxis, &m_UpAxis, &m_LookAxis);
	D3DXVec3Normalize(&m_RightAxis, &m_RightAxis);

	// 建立视图矩阵
	float x = -D3DXVec3Dot(&m_RightAxis, &m_Position);
	float y = -D3DXVec3Dot(&m_UpAxis, &m_Position);
	float z = -D3DXVec3Dot(&m_LookAxis, &m_Position);

#define M m_ViewMatrix
	M(0, 0) = m_RightAxis.x;	M(0, 1) = m_UpAxis.x;		M(0, 2) = m_LookAxis.x;		M(0, 3) = 0.0f;
	M(1, 0) = m_RightAxis.y;	M(1, 1) = m_UpAxis.y;		M(1, 2) = m_LookAxis.y;		M(1, 3) = 0.0f;
	M(2, 0) = m_RightAxis.z;	M(2, 1) = m_UpAxis.z;		M(2, 2) = m_LookAxis.z;		M(2, 3) = 0.0f;
	M(3, 0) = x;				M(3, 1) = y;				M(3, 2) = z;				M(3, 3) = 1.0f;

	//static float angle = (3.0f * D3DX_PI) / 2.0f;
	//static float height = 2.0f;
	//D3DXVECTOR3 position(cosf(angle) * 3.0f, height, sinf(angle) * 3.0f);
	//D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(m_pViewMatrix, &position, &target, &up);

	//int x11 = m_pViewMatrix->_11;	int x12 = m_pViewMatrix->_12;	int x13 = m_pViewMatrix->_13;	int x14 = m_pViewMatrix->_14;
	//int x21 = m_pViewMatrix->_21;	int x22 = m_pViewMatrix->_22;	int x23 = m_pViewMatrix->_23;	int x24 = m_pViewMatrix->_24;
	//int x31 = m_pViewMatrix->_31;	int x32 = m_pViewMatrix->_32;	int x33 = m_pViewMatrix->_33;	int x34 = m_pViewMatrix->_34;
	//int x41 = m_pViewMatrix->_41;	int x42 = m_pViewMatrix->_42;	int x43 = m_pViewMatrix->_43;	int x44 = m_pViewMatrix->_44;

	return &m_ViewMatrix;
}

D3DXMATRIX* Camera::GetProjectionMatrix() {
	return &m_ProjectionMatrix;
}

void Camera::Move(float x, float y, float z) {
	m_Position += m_RightAxis * x + m_UpAxis * y + m_LookAxis * z;
}

void Camera::Pitch(float radian) {
	D3DXMATRIX transform;
	D3DXMatrixRotationAxis(&transform, &m_RightAxis, radian);

	// 修正其他两个轴的方向
	D3DXVec3TransformCoord(&m_UpAxis, &m_UpAxis, &transform);
	D3DXVec3TransformCoord(&m_LookAxis, &m_LookAxis, &transform);
}

void Camera::Yaw(float radian) {
	D3DXMATRIX transform;
	D3DXMatrixRotationAxis(&transform, &m_UpAxis, radian);

	// 修正其他两个轴的方向
	D3DXVec3TransformCoord(&m_RightAxis, &m_RightAxis, &transform);
	D3DXVec3TransformCoord(&m_LookAxis, &m_LookAxis, &transform);
}

void Camera::Roll(float radian) {
	D3DXMATRIX transform;
	D3DXMatrixRotationAxis(&transform, &m_LookAxis, radian);

	// 修正其他两个轴的方向
	D3DXVec3TransformCoord(&m_UpAxis, &m_UpAxis, &transform);
	D3DXVec3TransformCoord(&m_RightAxis, &m_RightAxis, &transform);
}
