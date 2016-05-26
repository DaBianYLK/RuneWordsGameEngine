/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	材质表达式--常量
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include "RwgeCoreDef.h"
#include "RwgeMaterialExpression.h"
#include <string.h>

template<typename ReturnType>
class MExpConstant : public MaterialExpressionWithID<ReturnType, EME_Constant>
{
public:
	MExpConstant() { RwgeZeroMemory(&m_Value, sizeof(ReturnType)); };
	MExpConstant(const ReturnType& value) : m_Value(value)	{};
	virtual ~MExpConstant() {};

	FORCE_INLINE void SetValue(const ReturnType& value)		{ m_Value = value; };
	FORCE_INLINE const ReturnType& GetValue()				{ return m_Value; };

	virtual unsigned short	GetConstantCount()	override	{ return sizeof(ReturnType) / sizeof(float); }
	virtual float*			GetConstants()		override	{ return reinterpret_cast<float*>(&m_Value); }

private:
	ReturnType m_Value;
};

typedef MExpConstant<float>			MExpConstantScalar;
typedef MExpConstant<D3DXVECTOR3>	MExpConstantVector; 
typedef MExpConstant<D3DXVECTOR3>	MExpConstantColor;