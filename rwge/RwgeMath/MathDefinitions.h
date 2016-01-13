#pragma once

namespace RwgeMath
{
	static const float		lpPI = 3.141592f;				// PI的低精度浮点数，low precision
	static const float		lpDividePI = 1.0f / lpPI;
	static const float		lpTwoPI = 2.0f * lpPI;
	static const float		lpDivideTwoPI = 1.0f / lpTwoPI;
	static const float		lpDegreeToRadian = lpPI / 180.0f;
	static const float		lpRadianToDegree = 180.0f / lpPI;
	static const float		lpDivide360 = 1.0f / 360.0f;

	static const double		hpPI = 3.141592653589793;		// PI的高精度浮点数, high precision

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
}