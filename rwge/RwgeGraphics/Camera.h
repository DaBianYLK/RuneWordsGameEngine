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
1. 继承自SceneNode，相机的正方向与SceneNode相同，默认为Z轴正方向
2. 视图矩阵即为相机自身变换矩阵的逆矩阵
3. 相机继承了SceneNode的旋转方式，此外还可以通过LookAt函数进行旋转，两者是覆盖关系
4. LookAtPosition是基于世界坐标系定义的
5. 相机的position与SceneNode一样是基于父节点坐标系定义的
6. FrontAxis与UpAxis是方向向量
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

	float m_fFovy;				// 相机垂直视角的大小，单位为弧度
	float m_fAspect;			// 相机裁剪平面宽高比
	float m_fLookNear;			// 相机近裁剪平面距离
	float m_fLookFar;			// 相机远裁剪平面距离

	byte m_bLockLookAtPos;		// 锁定视线焦点，FrontAxis随LookAtPosition变化，否则LookAtPosition随FrontAxis变化
};