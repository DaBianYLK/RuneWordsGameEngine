#pragma once

#include <d3dx9.h>

#include "SceneNode.h"

/*
������ͼ�任�������
D3DXMATRIX * D3DXMatrixLookAtLH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
zaxis = normal(At - Eye)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x           0
xaxis.y           yaxis.y           zaxis.y           0
xaxis.z           yaxis.z           zaxis.z           0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

������ͼ�任�������
D3DXMATRIX * D3DXMatrixLookAtRH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
zaxis = normal(Eye - At)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x          0
xaxis.y           yaxis.y           zaxis.y          0
xaxis.z           yaxis.z           zaxis.z          0
dot(xaxis, eye)   dot(yaxis, eye)   dot(zaxis, eye)  1

�������MSDN
https://msdn.microsoft.com/en-us/library/windows/desktop/bb205342(v=vs.85).aspx
*/

class Camera : public SceneNode
{
public:
	Camera();
	Camera(const D3DXVECTOR3& pos);
	Camera(const D3DXVECTOR3& pos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis, bool bLockLookAt);
	Camera(const D3DXVECTOR3& pos, const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis, bool bLockLookAt,
		float fFovy, float fAspect, float fLookNear, float fLookFar);
	~Camera();

	void SetUpAxis(const D3DXVECTOR3& upAxis);
	void LookAtPosition(const D3DXVECTOR3& lookAtPos);
	void SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar);

	D3DXMATRIX* GetViewMatrix();
	D3DXMATRIX* GetProjectionMatrix();

	void Update() override;

private:
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	D3DXVECTOR3 m_LookAtPosition;
	D3DXVECTOR3 m_UpAxis;

	bool m_bLockLookAt;		// �����ֵΪTRUE���������Ŀ��㲻��������������ת���ı䣻���ΪFALSE��LookAtPosition�����������ת�ı�

	float m_fFovy;			// �����ֱ�ӽǵĴ�С����λΪ����
	float m_fAspect;		// ����ü�ƽ���߱�
	float m_fLookNear;		// ������ü�ƽ�����
	float m_fLookFar;		// ���Զ�ü�ƽ�����
};