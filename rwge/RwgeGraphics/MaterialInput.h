#pragma once

#include <d3d9.h>

template<typename InputType>
struct MaterialInput
{
	MaterialInput();
	~MaterialInput();

	bool bUseConstant;
	InputType Constant;
};

template<typename InputType>
MaterialInput<InputType>::MaterialInput()
{
	bUseConstant = true;
	Constant = static_cast<InputType>(0);
}

template<typename InputType>
MaterialInput<InputType>::~MaterialInput()
{
	
}