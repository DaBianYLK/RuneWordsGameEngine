#pragma once

#include "MathDefinitions.h"

class AngleDegree;

/*
弧度，圆周取值范围[0.0f, 2 * PI)
*/
class AngleRadian
{
	friend class AngleDegree;

public:
	inline AngleRadian();
	inline AngleRadian(float angle);
	inline AngleRadian(const AngleDegree& angle);
	inline ~AngleRadian();

	inline float ToFloat() const;
	inline AngleDegree ToDegree() const;

	inline AngleRadian operator* (const AngleRadian& angle) const;
	inline AngleRadian operator/ (const AngleRadian& angle) const;
	inline AngleRadian operator+ (const AngleRadian& angle) const;
	inline AngleRadian operator- (const AngleRadian& angle) const;

	inline AngleRadian& operator= (float angle);
	inline AngleRadian& operator= (const AngleDegree& angle);

	inline bool operator== (float angle) const;
	inline bool operator== (const AngleRadian& angle) const;
	inline bool operator== (const AngleDegree& angle) const; 
	inline bool operator!= (float angle) const;
	inline bool operator!= (const AngleRadian& angle) const;
	inline bool operator!= (const AngleDegree& angle) const;

	inline AngleRadian& Clamp();
	inline static float Clamp(float radian);

private:
	float m_fValue;
};

/*
角度，圆周取值范围[0.0f, 360.0f)
*/
class AngleDegree
{
	friend class AngleRadian;

public:
	inline AngleDegree();
	inline AngleDegree(float fValue);
	inline AngleDegree(const AngleRadian& value);
	inline ~AngleDegree();

	inline float ToFloat() const;
	inline AngleRadian ToDegree() const;

	inline AngleDegree operator* (const AngleDegree& angle) const;
	inline AngleDegree operator/ (const AngleDegree& angle) const;
	inline AngleDegree operator+ (const AngleDegree& angle) const;
	inline AngleDegree operator- (const AngleDegree& angle) const;

	inline AngleDegree& operator= (float angle);
	inline AngleDegree& operator= (const AngleRadian& angle);

	inline bool operator== (float angle) const;
	inline bool operator== (const AngleDegree& angle) const;
	inline bool operator== (const AngleRadian& angle) const;
	inline bool operator!= (float angle) const;
	inline bool operator!= (const AngleDegree& angle) const;
	inline bool operator!= (const AngleRadian& angle) const;

	inline AngleDegree& Clamp();
	inline static float Clamp(float degree);

private:
	float m_fValue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AngleRadian::AngleRadian() : m_fValue(0.0f)
{

}

inline AngleRadian::AngleRadian(float angle) : m_fValue(angle)
{
	Clamp(m_fValue);
}

inline AngleRadian::AngleRadian(const AngleDegree& angle)
{
	*this = angle;
}

inline AngleRadian::~AngleRadian()
{

}

inline float AngleRadian::ToFloat() const
{
	return m_fValue;
}

inline AngleDegree AngleRadian::ToDegree() const
{
	return AngleDegree(*this);
}

inline AngleRadian AngleRadian::operator*(const AngleRadian& angle) const
{
	return AngleRadian(m_fValue * angle.m_fValue);
}

inline AngleRadian AngleRadian::operator/(const AngleRadian& angle) const
{
	return AngleRadian(m_fValue / angle.m_fValue);
}

inline AngleRadian AngleRadian::operator+(const AngleRadian& angle) const
{
	return AngleRadian(m_fValue + angle.m_fValue);
}

inline AngleRadian AngleRadian::operator-(const AngleRadian& angle) const
{
	return AngleRadian(m_fValue - angle.m_fValue);
}

inline AngleRadian& AngleRadian::operator=(float angle)
{
	m_fValue = angle;
	Clamp(m_fValue);

	return *this;
}

inline AngleRadian& AngleRadian::operator= (const AngleDegree& angle)
{
	m_fValue = angle.m_fValue * RwgeMath::lpDegreeToRadian;
	Clamp();

	return *this;
}

inline bool AngleRadian::operator==(float angle) const
{
	return m_fValue == Clamp(angle);
}

inline bool AngleRadian::operator==(const AngleRadian& angle) const
{
	return m_fValue == angle.m_fValue;
}

inline bool AngleRadian::operator==(const AngleDegree& angle) const
{
	return m_fValue == AngleRadian(angle).m_fValue;
}

inline bool AngleRadian::operator!=(float angle) const
{
	return m_fValue != Clamp(angle);
}

inline bool AngleRadian::operator!=(const AngleRadian& angle) const
{
	return m_fValue != angle.m_fValue;
}

inline bool AngleRadian::operator!=(const AngleDegree& angle) const
{
	return m_fValue != AngleRadian(angle).m_fValue;
}

inline AngleRadian& AngleRadian::Clamp()
{
	int iMultiple = static_cast<int>(m_fValue * RwgeMath::lpDivideTwoPI);
	m_fValue = m_fValue - static_cast<float>(iMultiple)* RwgeMath::lpTwoPI;

	return *this;
}

inline float AngleRadian::Clamp(float radian)
{
	int iMultiple = static_cast<int>(radian * RwgeMath::lpDivideTwoPI);
	radian = radian - static_cast<float>(iMultiple)* RwgeMath::lpTwoPI;

	return radian;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AngleDegree::AngleDegree() : m_fValue(0.0f)
{

}

inline AngleDegree::AngleDegree(float angle) : m_fValue(angle)
{
	Clamp();
}

inline AngleDegree::AngleDegree(const AngleRadian& angle)
{
	*this = angle;
}

inline AngleDegree::~AngleDegree()
{

}

inline float AngleDegree::ToFloat() const
{
	return m_fValue;
}

inline AngleRadian AngleDegree::ToDegree() const
{
	return AngleRadian(*this);
}

inline AngleDegree AngleDegree::operator*(const AngleDegree& angle) const
{
	return AngleDegree(m_fValue * angle.m_fValue);
}

inline AngleDegree AngleDegree::operator/(const AngleDegree& angle) const
{
	return AngleDegree(m_fValue / angle.m_fValue);
}

inline AngleDegree AngleDegree::operator+(const AngleDegree& angle) const
{
	return AngleRadian(m_fValue + angle.m_fValue);
}

inline AngleDegree AngleDegree::operator-(const AngleDegree& angle) const
{
	return AngleDegree(m_fValue - angle.m_fValue);
}

inline AngleDegree& AngleDegree::operator=(float angle)
{
	m_fValue = angle;
	Clamp();

	return *this;
}

inline AngleDegree& AngleDegree::operator= (const AngleRadian& angle)
{
	m_fValue = angle.m_fValue * RwgeMath::lpRadianToDegree;
	Clamp();

	return *this;
}

inline bool AngleDegree::operator==(float angle) const
{
	return m_fValue == Clamp(angle);
}

inline bool AngleDegree::operator==(const AngleDegree& angle) const
{
	return m_fValue == angle.m_fValue;
}

inline bool AngleDegree::operator==(const AngleRadian& angle) const
{
	return m_fValue == AngleDegree(angle).m_fValue;
}

inline bool AngleDegree::operator!=(float angle) const
{
	return m_fValue != Clamp(angle);
}

inline bool AngleDegree::operator!=(const AngleDegree& angle) const
{
	return m_fValue != angle.m_fValue;
}

inline bool AngleDegree::operator!=(const AngleRadian& angle) const
{
	return m_fValue != AngleDegree(angle).m_fValue;
}

inline AngleDegree& AngleDegree::Clamp()
{
	int iMultiple = static_cast<int>(m_fValue * RwgeMath::lpDivide360);
	m_fValue = m_fValue - static_cast<float>(iMultiple) * 360.0f;

	return *this;
}

inline float AngleDegree::Clamp(float degree)
{
	int iMultiple = static_cast<int>(degree * RwgeMath::lpDivide360);
	degree = degree - static_cast<float>(iMultiple)* 360.0f;

	return degree;
}
