#pragma once

#include <d3dx9.h>

#include "SceneNode.h"

class SceneManager;
class Viewport;

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
1.	�̳���SceneNode���������������SceneNode��ͬ��Ĭ��ΪZ��������
2.	��ͼ����Ϊ�������任����������
3.	Camera�ɳ���������������ɾ��������
*/

class Camera : public SceneNode
{
public:
	Camera();
	~Camera();

	void SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar);

	const D3DXMATRIX* GetViewTransform() const;
	const D3DXMATRIX* GetProjectionTransform() const;

	void UpdateCachedViewTransform() const;
	void UpdateCachedWorldTransform() const override;

	void RenderScene(Viewport* pViewport);

private:
	mutable D3DXMATRIX m_ViewTransform;	// ��ͼ�����ڻ�ȡ��ͼ����ʱ�������ͼ������ڲŻ����
	D3DXMATRIX m_ProjectionTransform;	// ͶӰ���������ò���ʱ�ͻᱻ��������

	float m_fFovy;				// �����ֱ�ӽǵĴ�С����λΪ����
	float m_fAspect;			// ����ü�ƽ���߱�
	float m_fLookNear;			// ������ü�ƽ�����
	float m_fLookFar;			// ���Զ�ü�ƽ�����

	mutable bool m_bCacheViewTransformOutOfDate;
};