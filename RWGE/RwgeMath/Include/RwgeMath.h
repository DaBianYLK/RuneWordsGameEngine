#pragma once

#include <d3dx9.h>

namespace RwgeMath
{
	static const float		lpPI				= 3.141592f;				// PI的低精度浮点数，low precision
	static const float		lpDividePI			= 1.0f / lpPI;
	static const float		lpTwoPI				= 2.0f * lpPI;
	static const float		lpDivideTwoPI		= 1.0f / lpTwoPI;
	static const float		lpDegreeToRadian	= lpPI / 180.0f;
	static const float		lpRadianToDegree	= 180.0f / lpPI;
	static const float		lpDivide360			= 1.0f / 360.0f;

	static const double		hpPI				= 3.141592653589793;		// PI的高精度浮点数, high precision

	static const D3DXVECTOR3 Vector3Zero(0.0f, 0.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitX(1.0f, 0.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitY(0.0f, 1.0f, 0.0f);
	static const D3DXVECTOR3 Vector3UnitZ(0.0f, 0.0f, 1.0f);

	/*
	裁剪函数：将value的值控制在[lowerBound, upperBound]范围内，若小于则置为lowerBound，若大于则置为upperBound
	@Param	
		value		输入
		lowerBound	下限
		upperBound	上限
	@Return
		返回裁剪后的值
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