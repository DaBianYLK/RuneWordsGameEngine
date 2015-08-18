#pragma once

#include <d3dx9.h>

class Camera {
public:
	enum Type {
		Perspective,
		Orthographic
	};

public:
	Camera();
	Camera(const D3DXVECTOR3& position, const D3DXVECTOR3& rightAxis, const D3DXVECTOR3& upAxis, const D3DXVECTOR3& lookAxis);
	~Camera();

	D3DXMATRIX* GetViewMatrix();

	void Move(float x, float y, float z);
	// ÈÆrightÖáÐý×ª
	void Pitch(float radian);
	// ÈÆupÖáÐý×ª
	void Yaw(float radian);
	// ÈÆlookÖáÐý×ª
	void Roll(float radian);

private:
	Type m_Type;

	D3DXMATRIX m_ViewMatrix;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_RightAxis;
	D3DXVECTOR3 m_UpAxis;
	D3DXVECTOR3 m_LookAxis;
};