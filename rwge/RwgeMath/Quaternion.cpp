#include "Quaternion.h"

Quaternion::Quaternion() :
	x(1.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{
}

Quaternion::Quaternion(float fX, float fY, float fZ, float fW) :
	x(fX),
	y(fY),
	z(fZ),
	w(fW)
{
}

Quaternion::Quaternion(const D3DXVECTOR3& rotateAxis, const AngleRadian& radianAngle)
{
	this->SetByAxisAngle(rotateAxis, radianAngle);
}

Quaternion::Quaternion(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis)
{
	this->SetByAxes(xAxis, yAxis, zAxis);
}

Quaternion::Quaternion(const D3DXMATRIX& matrix)
{
	this->SetByRotationMatrix(matrix);
}

Quaternion::~Quaternion()
{
}

Quaternion& Quaternion::SetByAxisAngle(const D3DXVECTOR3& rotateAxis, const AngleRadian& radianAngle)
{
	AxisAngleToQuaternion(this, &rotateAxis, radianAngle);

	return *this;
}

void Quaternion::ToAxisAngle(D3DXVECTOR3& outRotateAxis, AngleRadian& outRadianAngle) const
{
	QuaternionToAxisAngle(this, &outRotateAxis, outRadianAngle);
}

Quaternion& Quaternion::SetByAxes(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis)
{
	D3DXMATRIX matrix;

	matrix.m[0][0] = xAxis.x;
	matrix.m[0][1] = xAxis.y;
	matrix.m[0][2] = xAxis.z;

	matrix.m[1][0] = yAxis.x;
	matrix.m[1][1] = yAxis.y;
	matrix.m[1][2] = yAxis.z;

	matrix.m[2][0] = zAxis.x;
	matrix.m[2][1] = zAxis.y;
	matrix.m[2][2] = zAxis.z;

	RotationMatrixToQuaternion(this, &matrix);

	return *this;
}

void Quaternion::ToAxes(D3DXVECTOR3& outXAxis, D3DXVECTOR3& outYAxis, D3DXVECTOR3& outZAxis) const
{
	D3DXMATRIX matrix;

	QuaternionToRotationMatrix(this, &matrix);

	outXAxis.x = matrix.m[0][0];
	outXAxis.y = matrix.m[0][1];
	outXAxis.z = matrix.m[0][2];

	outYAxis.x = matrix.m[1][0];
	outYAxis.y = matrix.m[1][1];
	outYAxis.z = matrix.m[1][2];

	outZAxis.x = matrix.m[2][0];
	outZAxis.y = matrix.m[2][1];
	outZAxis.z = matrix.m[2][2];
}

Quaternion& Quaternion::SetByRotationMatrix(const D3DXMATRIX& matrix)
{
	RotationMatrixToQuaternion(this, &matrix);

	return *this;
}

void Quaternion::ToRotationMatrix(D3DXMATRIX& outMatrix) const
{
	QuaternionToRotationMatrix(this, &outMatrix);
}

Quaternion Quaternion::operator+(const Quaternion& quaternion) const
{
	return Quaternion(
		x + quaternion.x,
		y + quaternion.y,
		z + quaternion.z,
		w + quaternion.w);
}

Quaternion Quaternion::operator-(const Quaternion& quaternion) const
{
	return Quaternion(
		x - quaternion.x,
		y - quaternion.y,
		z - quaternion.z,
		w - quaternion.w);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::operator*(float fScalar) const
{
	return Quaternion(x * fScalar, y * fScalar, z * fScalar, w * fScalar);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const
{
	// 两个四元数相乘不同于两个向量相乘
	return Quaternion(
		w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y,
		w * quaternion.y + y * quaternion.w + z * quaternion.x - x * quaternion.z,
		w * quaternion.z + z * quaternion.w + x * quaternion.y - y * quaternion.x,
		w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z);
}

Quaternion Quaternion::Inverse() const
{
	float fNorm = x * x + y * y + z * z + w * w;
	if (fNorm > 0.0f)
	{
		float fInvNorm = 1.0f / fNorm;
		return Quaternion(-x * fInvNorm, -y * fInvNorm, -z * fInvNorm, w * fInvNorm);
	}

	return Quaternion();
}

float Quaternion::Norm() const
{
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Modulus() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Quaternion& Quaternion::Normalise()
{
	float fNorm = x * x + y * y + z * z + w * w;
	if (fNorm > 0.0f)
	{
		float fOneDivModulus = 1.0f / sqrt(fNorm);

		*this = (*this) * fOneDivModulus;
	}
	else
	{
		*this = Quaternion();
	}

	return *this;
}

D3DXVECTOR3&& Quaternion::RotateVector(const D3DXVECTOR3& vector) const
{
	D3DXVECTOR3 uv;
	D3DXVECTOR3 uuv;
	D3DXVECTOR3 qVector(x, y, z);
	D3DXVec3Cross(&uv, &qVector, &vector);
	D3DXVec3Cross(&uuv, &qVector, &uv);
	uv	*= (2.0f * w);
	uuv *= 2.0f;

	return vector + uv + uuv;
}
