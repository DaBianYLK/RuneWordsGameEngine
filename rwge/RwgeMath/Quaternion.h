#pragma once

/*
四元数
四元数可以用来准确唯一地表达三维向量的旋转，公式如下：
设有向量V，旋转轴R = （x, y, z）, 旋转角a，
则四元数A = (x*sin(a/2), y*sin(a/2), z*sin(a/2), cos(a/2))，旋转后的向量 V' = A V (A^-1)
*/

#include <d3dx9.h>
#include "AngleDefinitions.h"

struct Quaternion
{
	Quaternion();
	Quaternion(float fX, float fY, float fZ, float fW);
	Quaternion(const D3DXVECTOR3& rotateAxis, const AngleRadian& radianAngle);
	Quaternion(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis);
	Quaternion(const D3DXMATRIX& matrix);
	~Quaternion();

	Quaternion& SetByAxisAngle(const D3DXVECTOR3& rotateAxis, const AngleRadian& radianAngle);
	void ToAxisAngle(D3DXVECTOR3& outRotateAxis, AngleRadian& outRadianAngle) const;

	Quaternion& SetByAxes(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis);
	void ToAxes(D3DXVECTOR3& outXAxis, D3DXVECTOR3& outYAxis, D3DXVECTOR3& outZAxis) const;

	Quaternion& SetByRotationMatrix(const D3DXMATRIX& matrix);
	void ToRotationMatrix(D3DXMATRIX& outMatrix) const;

	Quaternion operator+ (const Quaternion& quaternion) const;   
	Quaternion operator- (const Quaternion& quaternion) const;
	Quaternion operator- () const;
	Quaternion operator* (float fScalar) const;
	Quaternion operator* (const Quaternion& quaternion) const;
	Quaternion Inverse() const;		// 返回四元数的逆
	float Norm() const;				// 求四元数的范数，即四元数模的平方
	float Modulus() const;			// 求四元数的模
	Quaternion& Normalise();		// 单位化四元数，并返回自身

	D3DXVECTOR3&& RotateVector(const D3DXVECTOR3& vector) const;

	__forceinline static Quaternion* AxisAngleToQuaternion(Quaternion* pOutQuaternion, const D3DXVECTOR3* pInAxis, const AngleRadian& inAngle);
	__forceinline static void QuaternionToAxisAngle(const Quaternion* pInQuaternion, D3DXVECTOR3* pOutAxis, AngleRadian& outRadianAngle);

	__forceinline static Quaternion* RotationMatrixToQuaternion(Quaternion* pOutQuaternion, const D3DXMATRIX* pInMatrix);
	__forceinline static D3DXMATRIX* QuaternionToRotationMatrix(const Quaternion* pInQuaternion, D3DXMATRIX* pOutMatrix);

	float x;
	float y;
	float z;
	float w;
};

__forceinline Quaternion* Quaternion::AxisAngleToQuaternion(Quaternion* pOutQuaternion, const D3DXVECTOR3* pInAxis, const AngleRadian& inAngle)
{
	float halfRadian = inAngle.ToFloat() *0.5f;
	float fSin = sin(halfRadian);

	pOutQuaternion->x = fSin * pInAxis->x;		// X = sin(radian / 2) * rotateAxis.x
	pOutQuaternion->y = fSin * pInAxis->y;		// Y = sin(radian / 2) * rotateAxis.y
	pOutQuaternion->z = fSin * pInAxis->z;		// Z = sin(radian / 2) * rotateAxis.z
	pOutQuaternion->w = cos(halfRadian);		// W = cos(radian / 2)

	return pOutQuaternion;
}

__forceinline void Quaternion::QuaternionToAxisAngle(const Quaternion* pInQuaternion, D3DXVECTOR3* pOutAxis, AngleRadian& outRadianAngle)
{
	float x = pInQuaternion->x;
	float y = pInQuaternion->y;
	float z = pInQuaternion->z;
	float fSqrLength = x*x + y*y + z*z;

	if (fSqrLength > 0.0f)
	{
		outRadianAngle = 2.0f * acos(pInQuaternion->w);
		float fInvLength = 1.0f / sqrt(fSqrLength);
		pOutAxis->x = x * fInvLength;
		pOutAxis->y = y * fInvLength;
		pOutAxis->z = z * fInvLength;
	}
	else
	{
		// angle is 0 (mod 2*pi), so any axis will do
		outRadianAngle = 0.0f;
		pOutAxis->x = 1.0f;
		pOutAxis->y = 0.0f;
		pOutAxis->z = 0.0f;
	}
}

__forceinline Quaternion* Quaternion::RotationMatrixToQuaternion(Quaternion* pOutQuaternion, const D3DXMATRIX* pInMatrix)
{
	float const* const* pM = reinterpret_cast<float const* const*>(pInMatrix->m);
	float fTrace = pM[0][0] + pM[1][1] + pM[2][2];
	float fRoot;

	if (fTrace > 0.0)
	{
		fRoot = sqrt(fTrace + 1.0f);	// 2w
		pOutQuaternion->w = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;			// 1/(4w)
		pOutQuaternion->x = (pM[1][2] - pM[2][1]) * fRoot;
		pOutQuaternion->y = (pM[2][0] - pM[0][2]) * fRoot;
		pOutQuaternion->z = (pM[0][1] - pM[1][0]) * fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (pM[1][1] >  pM[0][0])
		{
			i = 1;
		}
		if (pM[2][2] >  pM[i][i])
		{
			i = 2;
		}
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(pM[i][i] - pM[j][j] - pM[k][k] + 1.0f);
		float* pQ[3] = { &pOutQuaternion->x, &pOutQuaternion->y, &pOutQuaternion->z };
		*pQ[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		pOutQuaternion->w = (pM[j][k] - pM[k][j]) * fRoot;
		*pQ[j] = (pM[i][j] + pM[j][i]) * fRoot;
		*pQ[k] = (pM[i][k] + pM[k][i]) * fRoot;
	}

	return pOutQuaternion;
}

__forceinline D3DXMATRIX* Quaternion::QuaternionToRotationMatrix(const Quaternion* pInQuaternion, D3DXMATRIX* pOutMatrix)
{
	float f2x = pInQuaternion->x + pInQuaternion->x;
	float f2y = pInQuaternion->y + pInQuaternion->y;
	float f2z = pInQuaternion->z + pInQuaternion->z;
	float f2xw = f2x * pInQuaternion->w;
	float f2yw = f2y * pInQuaternion->w;
	float f2zw = f2z * pInQuaternion->w;
	float f2xx = f2x * pInQuaternion->x;
	float f2xy = f2y * pInQuaternion->x;
	float f2xz = f2z * pInQuaternion->x;
	float f2yy = f2y * pInQuaternion->y;
	float f2yz = f2z * pInQuaternion->y;
	float f2zz = f2z * pInQuaternion->z;

	float** pM = reinterpret_cast<float**>(pOutMatrix->m);

	pM[0][0] = 1.0f - (f2yy + f2zz);	pM[0][1] = f2xy + f2zw;				pM[0][2] = f2xz - f2yw;				pM[0][3] = 0.0f;
	pM[1][0] = f2xy - f2zw;				pM[1][1] = 1.0f - (f2xx + f2zz);	pM[1][2] = f2yz + f2xw;				pM[1][3] = 0.0f;
	pM[2][0] = f2xz + f2yw;				pM[2][1] = f2yz - f2xw;				pM[2][2] = 1.0f - (f2xx + f2yy);	pM[2][3] = 0.0f;
	pM[3][0] = 0.0f;					pM[3][1] = 0.0f;					pM[3][2] = 0.0f;					pM[3][3] = 1.0f;

	return pOutMatrix;
}