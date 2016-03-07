#pragma once

#include "MaterialExpression.h"

/*
优先采用表达式输入，如果pExpression表达式指针为空，则使用常量输入
*/

template<typename InputType>
class MaterialInput
{
public:
	MaterialInput();
	virtual ~MaterialInput();

	unsigned char GetExpressionID() const;
	void GetConstantParamData(void*& pParam, unsigned char& uSize) const;
	unsigned char AddConstantParamToBuffer(void* pBuffer) const;

	Texture* GetTexture() const;

	MaterialExpression* pExpression;
	InputType constant;
};

template<typename InputType>
MaterialInput<InputType>::MaterialInput() : pExpression(nullptr)
{
	constant = static_cast<InputType>(0);
}

template<typename InputType>
MaterialInput<InputType>::~MaterialInput()
{
	
}

template <typename InputType>
unsigned char MaterialInput<InputType>::GetExpressionID() const
{
	if (pExpression)
	{
		return pExpression->GetExpressionID();
	}

	return 0;
}

template <typename InputType>
void MaterialInput<InputType>::GetConstantParamData(void*& pParam, unsigned char& uSize) const
{
	if (pExpression)
	{
		pExpression->GetConstantParamData(pParam, uSize);
	}
	else
	{
		pParam = reinterpret_cast<void*>(&constant);
		uSize = sizeof(InputType);
	}
}

template <typename InputType>
unsigned char MaterialInput<InputType>::AddConstantParamToBuffer(void* pBuffer) const
{
	if (pExpression)
	{
		return pExpression->AddConstantParamToBuffer(pBuffer);
	}

	memcpy(pBuffer, &constant, sizeof(InputType));
	return sizeof(InputType);
}

template <typename InputType>
Texture* MaterialInput<InputType>::GetTexture() const
{
	if (pExpression)
	{
		return pExpression->GetTexture();
	}

	return nullptr;
}
