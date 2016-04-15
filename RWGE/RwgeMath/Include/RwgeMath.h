#pragma once

#include <d3dx9.h>

namespace RwgeMath
{
	static const float		lpPI				= 3.141592f;				// PI�ĵ;��ȸ�������low precision
	static const float		lpDividePI			= 1.0f / lpPI;
	static const float		lpTwoPI				= 2.0f * lpPI;
	static const float		lpDivideTwoPI		= 1.0f / lpTwoPI;
	static const float		lpDegreeToRadian	= lpPI / 180.0f;
	static const float		lpRadianToDegree	= 180.0f / lpPI;
	static const float		lpDivide360			= 1.0f / 360.0f;

	static const double		hpPI				= 3.141592653589793;		// PI�ĸ߾��ȸ�����, high precision

	static const D3DXVECTOR3 Vector3Zero(0.0f, 0.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitX(1.0f, 0.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitY(0.0f, 1.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitZ(0.0f, 0.0f, 1.0f);

	/*
	�ü���������value��ֵ������[lowerBound, upperBound]��Χ�ڣ���С������ΪlowerBound������������ΪupperBound
	@Param	
		value		����
		lowerBound	����
		upperBound	����
	@Return
		���زü����ֵ
	*/
	template<typename T>
	T& Clamp(const T& value, const T& lowerBound, const T& upperBound)
	{
		return value < lowerBound ? lowerBound : (value > upperBound ? upperBound : value);
	}

	__forceinline D3DXVECTOR3 operator * (const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		return D3DXVECTOR3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	__forceinline D3DXVECTOR3 operator *= (D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		v1.x *= v2.x;
		v1.y *= v2.y;
		v1.z *= v2.z;

		return v1;
	}

	__forceinline D3DXVECTOR3 operator / (const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		return D3DXVECTOR3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}

	__forceinline D3DXVECTOR3 operator /= (D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		v1.x /= v2.x;
		v1.y /= v2.y;
		v1.z /= v2.z;

		return v1;
	}

	__forceinline float Distance2(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z);
	}

	__forceinline float Distance(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
	{
		return sqrtf(Distance2(v1, v2));
	}
}