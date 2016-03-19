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

	TextureInfo* GetTextureInfo() const;

	void SetConstant(const InputType& constant);
	const InputType& GetConstant() const;

	void SetExpression(MaterialExpression* pExpression);
	MaterialExpression* GetExpression() const;

private:
	MaterialExpression* m_pExpression;
	InputType m_Constant;
};

template<typename InputType>
MaterialInput<InputType>::MaterialInput() : m_pExpression(nullptr)
{
	
}

template<typename InputType>
MaterialInput<InputType>::~MaterialInput()
{
	
}

template <typename InputType>
unsigned char MaterialInput<InputType>::GetExpressionID() const
{
	if (m_pExpression)
	{
		return m_pExpression->GetExpressionID();
	}

	return 0;
}

template <typename InputType>
void MaterialInput<InputType>::GetConstantParamData(void*& pParam, unsigned char& uSize) const
{
	if (m_pExpression)
	{
		m_pExpression->GetConstantParamData(pParam, uSize);
	}
	else
	{
		pParam = reinterpret_cast<void*>(&m_Constant);
		uSize = sizeof(InputType);
	}
}

template <typename InputType>
unsigned char MaterialInput<InputType>::AddConstantParamToBuffer(void* pBuffer) const
{
	if (m_pExpression)
	{
		return m_pExpression->AddConstantParamToBuffer(pBuffer);
	}

	memcpy(pBuffer, &m_Constant, sizeof(InputType));
	return sizeof(InputType);
}

template <typename InputType>
TextureInfo* MaterialInput<InputType>::GetTextureInfo() const
{
	if (m_pExpression)
	{
		return m_pExpression->GetTextureInfo();
	}

	return nullptr;
}

template <typename InputType>
void MaterialInput<InputType>::SetConstant(const InputType& constant)
{
	m_Constant = constant;
}

template <typename InputType>
const InputType& MaterialInput<InputType>::GetConstant() const
{
	return m_Constant;
}

template <typename InputType>
void MaterialInput<InputType>::SetExpression(MaterialExpression* pExpression)
{
	m_pExpression = pExpression;
}

template <typename InputType>
MaterialExpression* MaterialInput<InputType>::GetExpression() const
{
	return m_pExpression;
}
