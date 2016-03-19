#pragma once

/*
四元数
四元数可以用来准确唯一地表达三维向量的旋转，公式如下：
设有向量V，旋转轴R = （x, y, z）, 旋转角a，
则四元数A = (x*sin(a/2), y*sin(a/2), z*sin(a/2), cos(a/2))，旋转后的向量 V' = A V (A^-1)
*/

#include <d3dx9.h>
#include <math.h>
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

	__forceinline static Quaternion* GetRotationBetween(
		Quaternion* rotation, 
		const D3DXVECTOR3& v1, 
		const D3DXVECTOR3& v2, 
		const D3DXVECTOR3& rotationAxis = RwgeMath::Vector3Zero);

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
	float fTrace = pInMatrix->m[0][0] + pInMatrix->m[1][1] + pInMatrix->m[2][2];
	float fRoot;

	if (fTrace > 0.0)
	{
		fRoot = sqrt(fTrace + 1.0f);	// 2w
		pOutQuaternion->w = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;			// 1/(4w)
		pOutQuaternion->x = (pInMatrix->m[1][2] - pInMatrix->m[2][1]) * fRoot;
		pOutQuaternion->y = (pInMatrix->m[2][0] - pInMatrix->m[0][2]) * fRoot;
		pOutQuaternion->z = (pInMatrix->m[0][1] - pInMatrix->m[1][0]) * fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (pInMatrix->m[1][1] >  pInMatrix->m[0][0])
		{
			i = 1;
		}
		if (pInMatrix->m[2][2] >  pInMatrix->m[i][i])
		{
			i = 2;
		}
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(pInMatrix->m[i][i] - pInMatrix->m[j][j] - pInMatrix->m[k][k] + 1.0f);
		float* pQ[3] = { &pOutQuaternion->x, &pOutQuaternion->y, &pOutQuaternion->z };
		*pQ[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		pOutQuaternion->w = (pInMatrix->m[j][k] - pInMatrix->m[k][j]) * fRoot;
		*pQ[j] = (pInMatrix->m[i][j] + pInMatrix->m[j][i]) * fRoot;
		*pQ[k] = (pInMatrix->m[i][k] + pInMatrix->m[k][i]) * fRoot;
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

	pOutMatrix->m[0][0] = 1.0f - (f2yy + f2zz);	pOutMatrix->m[0][1] = f2xy + f2zw;				pOutMatrix->m[0][2] = f2xz - f2yw;				pOutMatrix->m[0][3] = 0.0f;
	pOutMatrix->m[1][0] = f2xy - f2zw;			pOutMatrix->m[1][1] = 1.0f - (f2xx + f2zz);		pOutMatrix->m[1][2] = f2yz + f2xw;				pOutMatrix->m[1][3] = 0.0f;
	pOutMatrix->m[2][0] = f2xz + f2yw;			pOutMatrix->m[2][1] = f2yz - f2xw;				pOutMatrix->m[2][2] = 1.0f - (f2xx + f2yy);		pOutMatrix->m[2][3] = 0.0f;
	pOutMatrix->m[3][0] = 0.0f;					pOutMatrix->m[3][1] = 0.0f;						pOutMatrix->m[3][2] = 0.0f;						pOutMatrix->m[3][3] = 1.0f;

	return pOutMatrix;
}

__forceinline Quaternion* Quaternion::GetRotationBetween(Quaternion* pRotation, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& rotationAxis /* = RwgeMath::Vector3Zero */)
{
	D3DXVECTOR3 normalV1 = v1;
	D3DXVECTOR3 normalV2 = v2;

	D3DXVec3Normalize(&normalV1, &normalV1);
	D3DXVec3Normalize(&normalV2, &normalV2);

	float dot = D3DXVec3Dot(&normalV1, &normalV2);
	// 如果点乘结果为1，说明两个向量相等
	if (dot >= 0.999999f)
	{
		pRotation->x = 0.0f;
		pRotation->y = 0.0f;
		pRotation->z = 0.0f;
		pRotation->w = 1.0f;
	}
	// 如果点乘结果为-1，说明两个向量恰好反向
	else if (dot <= -0.999999f)
	{
		if (rotationAxis == RwgeMath::Vector3Zero)
		{
			// 如果没有指定旋转轴，则生成一个
			D3DXVECTOR3 axis;
			D3DXVec3Cross(&axis, &RwgeMath::Vector3UnitX, &normalV1);
			if (axis == RwgeMath::Vector3Zero)
			{
				// 如果v1平行于X轴，则叉乘Y轴
				D3DXVec3Cross(&axis, &RwgeMath::Vector3UnitY, &normalV1);
			}

			D3DXVec3Normalize(&axis, &axis);

			pRotation->SetByAxisAngle(axis, -RwgeMath::lpPI);
		}
		else
		{
			pRotation->SetByAxisAngle(rotationAxis, -RwgeMath::lpPI);
		}
	}
	else
	{
		float fSqrt = sqrt((1.0f + dot) * 2.0f);
		float fInverse = 1.0f / fSqrt;

		D3DXVec3Cross(&normalV1, &normalV1, &normalV2);

		pRotation->x = normalV1.x * fInverse;
		pRotation->y = normalV1.y * fInverse;
		pRotation->z = normalV1.z * fInverse;
		pRotation->w = fSqrt * 0.5f;

		pRotation->Normalise();
	}

	return pRotation;
}