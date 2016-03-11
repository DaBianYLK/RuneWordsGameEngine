#include "ShaderProgram.h"

#include "D3D9Device.h"

ShaderProgram::ShaderProgram(const std::string& strPath) : 
	m_strFilePath(strPath), 
	m_pEffect(nullptr)
{
}

ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::Load(const D3D9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool)
{
	LPD3DXBUFFER pErrorBuffer = nullptr;

	HRESULT result = D3DXCreateEffectFromFile(pDevice->GetDevicePtr(),			// D3D9Device接口指针
											  m_strFilePath.c_str(),			// Shader资源文件路径
											  nullptr,							// 宏定义声明（二进制文件不需要）
											  nullptr,							// Include声明（二进制文件不需要）
											  0,								// Flag
											  pEffectPool,						// EffectPool指针
											  &m_pEffect,						// Effect指针的指针
											  &pErrorBuffer);					// 编译Error信息指针

	m_hTransform			= m_pEffect->GetParameterByName(nullptr, "g_Transform");
	m_hViewOppositeDirection= m_pEffect->GetParameterByName(nullptr, "g_ViewOppositeDirection");
	m_hMaterial				= m_pEffect->GetParameterByName(nullptr, "g_Material");
	m_hBaseColorTexture		= m_pEffect->GetParameterByName(nullptr, "g_BaseColorTexture");
	m_hEmissiveColorTexture = m_pEffect->GetParameterByName(nullptr, "g_EmissiveColorTexture");
	m_hNormalTexture		= m_pEffect->GetParameterByName(nullptr, "g_NormalTexture");
	m_hMetallicTexture		= m_pEffect->GetParameterByName(nullptr, "g_MetallicTexture");
	m_hSpecularTexture		= m_pEffect->GetParameterByName(nullptr, "g_SpecularTexture");
	m_hRoughnessTexture		= m_pEffect->GetParameterByName(nullptr, "g_RoughnessTexture");
	m_hOpacityTexture		= m_pEffect->GetParameterByName(nullptr, "g_OpacityTexture");
	m_hOpacityMaskTexture	= m_pEffect->GetParameterByName(nullptr, "g_OpacityMaskTexture");
	m_hDirectionalLight		= m_pEffect->GetParameterByName(nullptr, "g_DirectionalLight");
	m_hPointLight			= m_pEffect->GetParameterByName(nullptr, "g_PointLight");

	return result == D3D_OK;
}

void ShaderProgram::Begin() const
{
	unsigned int uPassCount;
	m_pEffect->Begin(&uPassCount, 0);
	m_pEffect->BeginPass(0);		// 暂时不考虑多Pass，假定所有Technique都是单Pass
}

void ShaderProgram::End() const
{
	m_pEffect->EndPass();
	m_pEffect->End();
}

void ShaderProgram::SetTransform(const void* pTransform)
{
	m_pEffect->SetValue(m_hTransform, pTransform, sizeof(float) * 16);
}

void ShaderProgram::SetViewOppositeDirection(const void* pDirection)
{
	m_pEffect->SetValue(m_hViewOppositeDirection, pDirection, sizeof(float) * 3);
}

void ShaderProgram::SetMaterial(const void* pMaterialConstant, unsigned uSize)
{
	m_pEffect->SetValue(m_hMaterial, pMaterialConstant, uSize);
}

void ShaderProgram::SetBaseColorTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hBaseColorTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetEmissiveColorTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hEmissiveColorTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetNormalTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hNormalTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetMetallicTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hMetallicTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetSpecularTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hSpecularTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetRoughnessTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hRoughnessTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetOpacityTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hOpacityTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetOpacityMaskTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hOpacityMaskTexture, pTexture->GetTexturePtr());
}

void ShaderProgram::SetLight(const void* pLight, unsigned int uSize)
{
	m_pEffect->SetValue(m_hPointLight, pLight, uSize);
}
