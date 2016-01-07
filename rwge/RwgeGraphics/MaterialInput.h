#pragma once

#include <d3d9.h>

template<typename InputType>
struct MaterialInput
{
	MaterialInput();
	MaterialInput(IDirect3DTexture9* pTexture);
	~MaterialInput();

	bool bUseConstant;

	InputType Constant;
	IDirect3DTexture9* pTexture;
};

template<typename InputType>
MaterialInput<InputType>::MaterialInput()
{
	bUseConstant = true;

	Constant = static_cast<InputType>(0);
	pTexture = nullptr;
}

template<typename InputType>
MaterialInput<InputType>::MaterialInput(IDirect3DTexture9* pTexture)
{
	if (pTexture)
	{
		bUseConstant = false;

		Constant = static_cast<InputType>(0);
		this->pTexture = pTexture;
	}
	else
	{
		bUseConstant = true;

		Constant = static_cast<InputType>(0);
		this->pTexture = nullptr;
	}
}

template<typename InputType>
MaterialInput<InputType>::~MaterialInput()
{
	if (pTexture)
	{
		pTexture->Release();
		delete pTexture;
	}
}