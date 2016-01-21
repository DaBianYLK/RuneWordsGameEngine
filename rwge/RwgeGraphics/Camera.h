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

�������
1. �̳���SceneNode���������������SceneNode��ͬ��Ĭ��ΪZ��������
2. ��ͼ����Ϊ�������任����������
3. ����̳���SceneNode����ת��ʽ�����⻹����ͨ��LookAt����������ת�������Ǹ��ǹ�ϵ
4. LookAtPosition�ǻ�����������ϵ�����
5. �����position��SceneNodeһ���ǻ��ڸ��ڵ�����ϵ�����
6. FrontAxis��UpAxis�Ƿ�������
*/

class Camera : public SceneNode
{
public:
	Camera();
	~Camera();

	void LookAtPosition(const D3DXVECTOR3& lookAtPos, const D3DXVECTOR3& upAxis);
	void LookAtDirection(const D3DXVECTOR3& lookAtDirection, const D3DXVECTOR3& upAxis);
	void SetLookAtPosition(const D3DXVECTOR3& lookAtPos);
	void SetFrontAxis(const D3DXVECTOR3& axis);
	void SetUpAxis(const D3DXVECTOR3& axis);
	void SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar);

	D3DXMATRIX* GetViewMatrix();
	D3DXMATRIX* GetProjectionMatrix();

	void Update() override;

private:
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	D3DXVECTOR3 m_LookAtPosition;
	D3DXVECTOR3 m_FrontAxis;
	D3DXVECTOR3 m_UpAxis;

	float m_fFovy;				// �����ֱ�ӽǵĴ�С����λΪ����
	float m_fAspect;			// ����ü�ƽ���߱�
	float m_fLookNear;			// ������ü�ƽ�����
	float m_fLookFar;			// ���Զ�ü�ƽ�����

	byte m_bLockLookAtPos;		// �������߽��㣬FrontAxis��LookAtPosition�仯������LookAtPosition��FrontAxis�仯
};