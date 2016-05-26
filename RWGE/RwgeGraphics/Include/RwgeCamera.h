/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	Camera由场景管理器创建、删除并管理，它的作用是将将场景中的对象映射到Viewport中
	2.	Camera继承自SceneNode ，它的正方向与SceneNode相同，默认为Z轴正方向
	3.	视图矩阵即为相机世界变换矩阵的逆矩阵
	4.	左手视图变换矩阵计算
		D3DXMATRIX * D3DXMatrixLookAtLH(	D3DXMATRIX *pOut,		CONST D3DXVECTOR3 *pEye, 
											CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
		zaxis = normal(At - Eye)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x           0
		xaxis.y           yaxis.y           zaxis.y           0
		xaxis.z           yaxis.z           zaxis.z           0
		-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

		右手视图变换矩阵计算
		D3DXMATRIX * D3DXMatrixLookAtRH(	D3DXMATRIX *pOut,		CONST D3DXVECTOR3 *pEye, 
											CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
		zaxis = normal(Eye - At)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x          0
		xaxis.y           yaxis.y           zaxis.y          0
		xaxis.z           yaxis.z           zaxis.z          0
		dot(xaxis, eye)   dot(yaxis, eye)   dot(zaxis, eye)  1

		详情查阅MSDN
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
	mutable D3DXMATRIX m_ViewTransform;	// 视图矩阵在获取时才会更新
	D3DXMATRIX m_ProjectionTransform;	// 投影矩阵在设置参数时就会被立即更新

	float m_f32Fovy;					// 相机垂直视角的大小，单位为弧度
	float m_f32Aspect;					// 相机裁剪平面宽高比
	float m_f32LookNear;				// 相机近裁剪平面距离
	float m_f32LookFar;					// 相机远裁剪平面距离

	mutable bool m_bCacheViewTransformOutOfDate;
};