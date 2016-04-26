/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-04-26
	DESC :	��ö�����ʹ����һ���ֽ��У�ͬʱ�ﵽ���Ͱ�ȫ���ʡ�ڴ��Ŀ�ģ�ʹ��ǰӦȷ��ö�����͵�ȡֵ��ΧΪ[0, 255]
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

template<class EnumType>
class EnumAsByte
{
public:
	inline EnumAsByte()									: m_u8Value(0)										{ }
	inline EnumAsByte(const EnumAsByte &inValue)		: m_u8Value(inValue.m_u8Value)						{ }
	inline EnumAsByte(EnumType inValue)					: m_u8Value(static_cast<unsigned char>(inValue))	{ }
	explicit inline EnumAsByte(int inValue)				: m_u8Value(static_cast<unsigned char>(inValue))	{ }
	explicit inline EnumAsByte(unsigned char inValue)	: m_u8Value(inValue)								{ }

public:
	inline EnumAsByte& operator=(const EnumAsByte& inValue)
	{
		m_u8Value = inValue.m_u8Value;
		return *this;
	}

	inline EnumAsByte& operator=(const EnumType& inValue)
	{
		m_u8Value = static_cast<unsigned char>(inValue);
		return *this;
	}

	bool operator==(EnumType inValue) const
	{
		return static_cast<EnumType>(m_u8Value) == inValue;
	}

	bool operator==(EnumAsByte inValue) const
	{
		return m_u8Value == inValue.m_u8Value;
	}

	operator EnumType() const
	{
		return (EnumType)m_u8Value;
	}

public:
	EnumType GetValue() const
	{
		return (EnumType)m_u8Value;
	}

private:
	unsigned char m_u8Value;
};