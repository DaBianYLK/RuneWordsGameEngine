#pragma once

/*
ËÄÔªÊý
*/

#include <d3dx9.h>
#include "AngleDefinitions.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(float fX, float fY, float fZ, float fW);
	Quaternion(D3DXVECTOR3& rotateAxis, AngleRadian radian);
	~Quaternion();



	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fW;
};

