#pragma once

namespace RwgeMath
{
	static const float		lpPI = 3.141592f;				// PI�ĵ;��ȸ�������low precision
	static const float		lpDividePI = 1.0f / lpPI;
	static const float		lpTwoPI = 2.0f * lpPI;
	static const float		lpDivideTwoPI = 1.0f / lpTwoPI;
	static const float		lpDegreeToRadian = lpPI / 180.0f;
	static const float		lpRadianToDegree = 180.0f / lpPI;
	static const float		lpDivide360 = 1.0f / 360.0f;

	static const double		hpPI = 3.141592653589793;		// PI�ĸ߾��ȸ�����, high precision

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
}