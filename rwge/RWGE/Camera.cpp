#include "Camera.h"

#include "AppConfig.h"
#include "Graphics.h"
#include "Window.h"

Camera::Camera() {
	m_NodeType = SceneNode::Type::CameraNode;

	m_Position = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_RightAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_ViewMatrix = D3DXMATRIX();
	m_ProjectionMatrix = D3DXMATRIX();

	D3DXMatrixPerspectiveFovLH(
		&m_ProjectionMatrix,
		AppConfig::cameraFovy,
		AppConfig::cameraAspect,
		AppConfig::cameraLookNear,
		AppConfig::cameraLookFar);
}

Camera::Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rightAxis, const D3DXVECTOR3& upAxis, const D3DXVECTOR3& lookAxis, 
			   float fovy, float aspect, float lookNear, float lookFar) {
	m_Position = position;
	m_RightAxis = rightAxis;
	m_UpAxis = upAxis;
	m_LookAxis = lookAxis;

	m_ViewMatrix = D3DXMATRIX();
	m_ProjectionMatrix = D3DXMATRIX();

	D3DXMatrixPerspectiveFovLH(
		&m_ProjectionMatrix,
		fovy,
		aspect,
		lookNear,
		lookFar);
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

	m_ViewMatrix(0, 0) = m_RightAxis.x;		m_ViewMatrix(0, 1) = m_UpAxis.x;		m_ViewMatrix(0, 2) = m_LookAxis.x;		m_ViewMatrix(0, 3) = 0.0f;
	m_ViewMatrix(1, 0) = m_RightAxis.y;		m_ViewMatrix(1, 1) = m_UpAxis.y;		m_ViewMatrix(1, 2) = m_LookAxis.y;		m_ViewMatrix(1, 3) = 0.0f;
	m_ViewMatrix(2, 0) = m_RightAxis.z;		m_ViewMatrix(2, 1) = m_UpAxis.z;		m_ViewMatrix(2, 2) = m_LookAxis.z;		m_ViewMatrix(2, 3) = 0.0f;
	m_ViewMatrix(3, 0) = x;					m_ViewMatrix(3, 1) = y;					m_ViewMatrix(3, 2) = z;					m_ViewMatrix(3, 3) = 1.0f;

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
