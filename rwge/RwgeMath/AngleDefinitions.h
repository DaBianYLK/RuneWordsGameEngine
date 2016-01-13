#pragma once

#include "MathDefinitions.h"

struct AngleDegree;

/*
弧度，圆周取值范围[0, 2 * PI)
*/
class AngleRadian
{
	friend class AngleDegree;

public:
	inline AngleRadian();
	inline AngleRadian(float angle);
	inline AngleRadian(const AngleDegree& angle);
	inline ~AngleRadian();

	inline AngleRadian& operator= (const AngleDegree& angle);

	inline AngleRadian& Clamp();

private:
	float m_fValue;
};

/*
角度，圆周取值范围[0, 360)
*/
class AngleDegree
{
	friend class AngleRadian;

public:
	inline AngleDegree();
	inline AngleDegree(float fValue);
	inline AngleDegree(const AngleRadian& value);
	inline ~AngleDegree();

	inline AngleDegree& operator= (const AngleRadian& angle);

	inline AngleDegree& Clamp();

private:
	float m_fValue;
};


AngleRadian::AngleRadian() : m_fValue(0.0f)
{

}

AngleRadian::AngleRadian(float angle) : m_fValue(angle)
{
	Clamp();
}

AngleRadian::AngleRadian(const AngleDegree& angle)
{
	*this = angle;
}

AngleRadian::~AngleRadian()
{

}

AngleRadian& AngleRadian::operator= (const AngleDegree& angle)
{
	m_fValue = angle.m_fValue * RwgeMath::lpDegreeToRadian;
	Clamp();

	return *this;
}

AngleRadian& AngleRadian::Clamp()
{
	int iMultiple = m_fValue * RwgeMath::lpDivideTwoPI;
	m_fValue = m_fValue - static_cast<float>(iMultiple) * RwgeMath::lpTwoPI;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AngleDegree::AngleDegree() : m_fValue(0.0f)
{

}

AngleDegree::AngleDegree(float angle) : m_fValue(angle)
{
	Clamp();
}

AngleDegree::AngleDegree(const AngleRadian& angle)
{
	*this = angle;
}

AngleDegree::~AngleDegree()
{

}

AngleDegree& AngleDegree::operator= (const AngleRadian& angle)
{
	m_fValue = angle.m_fValue * RwgeMath::lpRadianToDegree;
	Clamp();

	return *this;
}

AngleDegree& AngleDegree::Clamp()
{
	int iMultiple = m_fValue * RwgeMath::lpDivide360;
	m_fValue = m_fValue - static_cast<float>(iMultiple) * 360.0f;

	return *this;
}