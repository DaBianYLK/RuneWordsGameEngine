#pragma once

#include <d3dx9.h>

#include "SceneNode.h"

class Camera : public SceneNode {
public:
	enum Type {
		Perspective,
		Orthographic
	};

public:
	Camera();
	Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rightAxis, const D3DXVECTOR3& upAxis, const D3DXVECTOR3& lookAxis, 
		   float fovy, float aspect, float lookNear, float lookFar);
	~Camera();

	void SetLookAxes(const D3DXVECTOR3& rightAxis, const D3DXVECTOR3& upAxis, const D3DXVECTOR3& lookAxis);
	void SetPerspective(float fovy, float aspect, float lookNear, float lookFar);

	D3DXMATRIX* GetViewMatrix();
	D3DXMATRIX* GetProjectionMatrix();

private:
	Type m_Type;

	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	D3DXVECTOR3 m_RightAxis;
	D3DXVECTOR3 m_UpAxis;
	D3DXVECTOR3 m_LookAxis;
};