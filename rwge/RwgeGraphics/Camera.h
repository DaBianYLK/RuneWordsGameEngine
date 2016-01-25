#pragma once

#include <d3dx9.h>

#include "SceneNode.h"

/*
左手视图变换矩阵计算
D3DXMATRIX * D3DXMatrixLookAtLH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
zaxis = normal(At - Eye)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x           0
xaxis.y           yaxis.y           zaxis.y           0
xaxis.z           yaxis.z           zaxis.z           0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

右手视图变换矩阵计算
D3DXMATRIX * D3DXMatrixLookAtRH(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt, CONST D3DXVECTOR3 *pUp);
zaxis = normal(Eye - At)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x          0
xaxis.y           yaxis.y           zaxis.y          0
xaxis.z           yaxis.z           zaxis.z          0
dot(xaxis, eye)   dot(yaxis, eye)   dot(zaxis, eye)  1

详情查阅MSDN
https://msdn.microsoft.com/en-us/library/windows/desktop/bb205342(v=vs.85).aspx

关于相机
1.	继承自SceneNode，相机的正方向与SceneNode相同，默认为Z轴正方向
2.	视图矩阵即为相机世界变换矩阵的逆矩阵
3.	Camera由场景管理器创建、删除并管理
*/

class Camera : public SceneNode
{
public:
	Camera();
	~Camera();

	void SetPerspective(float fFovy, float fAspect, float fLookNear, float fLookFar);

	const D3DXMATRIX* GetViewMatrix() const;
	const D3DXMATRIX* GetProjectionMatrix() const;

	void UpdateCachedWorldTransform() const override;

	void UpdateViewMatrix() const;

private:
	mutable D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	float m_fFovy;				// 相机垂直视角的大小，单位为弧度
	float m_fAspect;			// 相机裁剪平面宽高比
	float m_fLookNear;			// 相机近裁剪平面距离
	float m_fLookFar;			// 相机远裁剪平面距离

	mutable bool m_bCacheViewMatrixOutOfDate;
};