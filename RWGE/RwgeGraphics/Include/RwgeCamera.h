/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	Camera�ɳ���������������ɾ�����������������ǽ��������еĶ���ӳ�䵽Viewport��
	2.	Camera�̳���SceneNode ��������������SceneNode��ͬ��Ĭ��ΪZ��������
	3.	��ͼ����Ϊ�������任����������
	4.	������ͼ�任�������
		D3DXMATRIX * D3DXMatrixLookAtLH(	D3DXMATRIX *pOut,		CONST D3DXVECTOR3 *pEye, 
											CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
		zaxis = normal(At - Eye)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x           0
		xaxis.y           yaxis.y           zaxis.y           0
		xaxis.z           yaxis.z           zaxis.z           0
		-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

		������ͼ�任�������
		D3DXMATRIX * D3DXMatrixLookAtRH(	D3DXMATRIX *pOut,		CONST D3DXVECTOR3 *pEye, 
											CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
		zaxis = normal(Eye - At)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x          0
		xaxis.y           yaxis.y           zaxis.y          0
		xaxis.z           yaxis.z           zaxis.z          0
		dot(xaxis, eye)   dot(yaxis, eye)   dot(zaxis, eye)  1

		�������MSDN
		https://msdn.microsoft.com/en-us/library/windows/desktop/bb205342(v=vs.85).aspx
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <d3dx9.h>

#include "RwgeSceneNode.h"

class RSceneManager;
class RD3d9Viewport;
class RD3d9RenderQueue;

class RCamera : public RSceneNode
{
public:
	RCamera();
	~RCamera();

	void SetPerspective(float f32Fovy, float f32Aspect, float f32LookNear, float f32LookFar);
	void SetOrthogonal(float f32Width, float f32Height, float f32LookNear, float f32LookFar);

	const D3DXMATRIX* GetViewTransform() const;
	const D3DXMATRIX* GetProjectionTransform() const;

	void UpdateCachedViewTransform() const;
	void UpdateCachedWorldTransform() const override;

	void GetSceneShot(RD3d9Viewport* pViewport, RD3d9RenderQueue& renderQueue);

private:
	mutable D3DXMATRIX m_ViewTransform;	// ��ͼ�����ڻ�ȡʱ�Ż����
	D3DXMATRIX m_ProjectionTransform;	// ͶӰ���������ò���ʱ�ͻᱻ��������

	float m_f32Fovy;					// �����ֱ�ӽǵĴ�С����λΪ����
	float m_f32Aspect;					// ����ü�ƽ���߱�
	float m_f32LookNear;				// ������ü�ƽ�����
	float m_f32LookFar;					// ���Զ�ü�ƽ�����

	mutable bool m_bCacheViewTransformOutOfDate;
};