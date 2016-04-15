#include "RwgeShader.h"

#include "RwgeD3D9Device.h"
#include "RwgeShaderType.h"
#include "RwgeMaterial.h"
#include "RwgeTextureInfo.h"
#include "RwgeTexture.h"
#include "RwgeRenderTarget.h"
#include <RwgeAssert.h>
#include <list>

using namespace std;

Shader::Shader(ShaderType* pShaderType) : 
	m_pShaderType(pShaderType),
	m_pEffect(nullptr), 
	m_hWVPTransform(nullptr), 
	m_hViewOppositeDirection(nullptr), 
	m_hMaterial(nullptr), 
	m_hBaseColorTexture(nullptr), 
	m_hEmissiveColorTexture(nullptr), 
	m_hNormalTexture(nullptr), 
	m_hMetallicTexture(nullptr), 
	m_hSpecularTexture(nullptr), 
	m_hRoughnessTexture(nullptr), 
	m_hOpacityTexture(nullptr), 
	m_hOpacityMaskTexture(nullptr), 
	m_hDirectionalLight(nullptr), 
	m_hPointLight(nullptr)
{
}

Shader::Shader(Shader&& shader) :
	m_pShaderType(shader.m_pShaderType),
	m_pEffect(shader.m_pEffect),
	m_hWVPTransform(shader.m_hWVPTransform),
	m_hViewOppositeDirection(shader.m_hViewOppositeDirection),
	m_hMaterial(shader.m_hMaterial),
	m_hBaseColorTexture(shader.m_hBaseColorTexture),
	m_hEmissiveColorTexture(shader.m_hEmissiveColorTexture),
	m_hNormalTexture(shader.m_hNormalTexture),
	m_hMetallicTexture(shader.m_hMetallicTexture),
	m_hSpecularTexture(shader.m_hSpecularTexture),
	m_hRoughnessTexture(shader.m_hRoughnessTexture),
	m_hOpacityTexture(shader.m_hOpacityTexture),
	m_hOpacityMaskTexture(shader.m_hOpacityMaskTexture),
	m_hDirectionalLight(shader.m_hDirectionalLight),
	m_hPointLight(shader.m_hPointLight)
{
	shader.m_pShaderType = nullptr;
	shader.m_pEffect = nullptr;
	shader.m_hWVPTransform = nullptr;
	shader.m_hViewOppositeDirection = nullptr;
	shader.m_hMaterial = nullptr;
	shader.m_hBaseColorTexture = nullptr;
	shader.m_hEmissiveColorTexture = nullptr;
	shader.m_hNormalTexture = nullptr;
	shader.m_hMetallicTexture = nullptr;
	shader.m_hSpecularTexture = nullptr;
	shader.m_hRoughnessTexture = nullptr;
	shader.m_hOpacityTexture = nullptr;
	shader.m_hOpacityMaskTexture = nullptr;
	shader.m_hDirectionalLight = nullptr;
	shader.m_hPointLight = nullptr;
}

Shader::~Shader()
{
}

bool Shader::Load(const D3D9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool)
{
	LPD3DXBUFFER pErrorBuffer = nullptr;

	HRESULT hResult = D3DXCreateEffectFromFile(pDevice->GetDevicePtr(),					// D3D9Device接口指针
											  m_pShaderType->GetShaderPath().c_str(),	// Shader资源文件路径
											  nullptr,									// 宏定义声明（二进制文件不需要）
											  nullptr,									// Include声明（二进制文件不需要）
											  0,										// Flag
											  pEffectPool,								// EffectPool指针
											  &m_pEffect,								// Effect指针的指针
											  &pErrorBuffer);							// 编译Error信息指针

	if (FAILED(hResult))
	{
		ErrorBox("Create effect failed : %X\n%s", hResult, pErrorBuffer);
		return false;
	}

	m_hWorldTransform		= m_pEffect->GetParameterByName(nullptr, "g_WorldTransform");
	m_hWVPTransform			= m_pEffect->GetParameterByName(nullptr, "g_WVPTransform");
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
	m_hSHCoefficients		= m_pEffect->GetParameterByName(nullptr, "g_OneBandSHCoefficients");
	return true;
}

void Shader::Begin() const
{
	unsigned int uPassCount;
	m_pEffect->Begin(&uPassCount, 0);
	m_pEffect->BeginPass(0);		// 暂时不考虑多Pass，假定所有Technique都是单Pass
}

void Shader::End() const
{
	m_pEffect->EndPass();
	m_pEffect->End();
}

void Shader::CommitChanges() const
{
	m_pEffect->CommitChanges();
}

void Shader::SetMaterial(Material* pMaterial, RenderTarget* pRenderTarget)
{
	// =========================== 绑定纹理 ===========================
	TextureInfo* pTextureInfo = pMaterial->m_BaseColor.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetBaseColorTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_EmissiveColor.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetEmissiveColorTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_Normal.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetNormalTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_Metallic.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetMetallicTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_Specular.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetSpecularTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_Roughness.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetRoughnessTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_Opacity.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetOpacityTexture(pRenderTarget->GetTexture(pTextureInfo));

	pTextureInfo = pMaterial->m_OpacityMask.GetTextureInfo();
	if (pTextureInfo != nullptr)	SetOpacityMaskTexture(pRenderTarget->GetTexture(pTextureInfo));

	// =========================== 绑定常量 ===========================
	unsigned char* pBuffer;
	unsigned char uSize;

	pMaterial->GetConstantBuffer(pBuffer, uSize);
	SetMaterialConstant(pBuffer, uSize);
}

void Shader::SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pWVP)
{
	/*
	Shader中的矩阵默认是column-major的，若使用SetRawValue或SetValue传递矩阵，需要先将矩阵转置
	*/
	m_pEffect->SetMatrix(m_hWorldTransform, pWorld);
	m_pEffect->SetMatrix(m_hWVPTransform, pWVP);
}

void Shader::SetViewOppositeDirection(const void* pDirection)
{
	/*
	从CPU向Shader传递数据时，需要注意Shader中所有的数据都是以float4为单位来排列的，即如果在hlsl中定义了一个结构体：
	struct Light   // in HLSL
	{
		float3 diffuse;
		float3 position;
	}
	实际上这个结构体在GPU中会占用2个float4的寄存器，若使用SetRawValue函数向Light传递数据，C++中定义的结构体应该为：
	struct Light    // in C++
	{  
		float diffuse[4];    // 此处应该多补一个float，以对齐GPU中的寄存器
		float position[3];
	}
	若定义为float diffuse[3]，会导致GPU读取的数据错位。

	若使用SetValue函数传递数据，则DX会自动对齐数据，此时不需要在diffuse中多补一个float，
	但是相对于SetRawValue，SetValue会有额外的性能消耗
	*/
	m_pEffect->SetRawValue(m_hViewOppositeDirection, pDirection, 0, sizeof(float)* 3);
}

void Shader::SetMaterialConstant(const void* pMaterialConstant, unsigned uSize)
{
	m_pEffect->SetValue(m_hMaterial, pMaterialConstant, uSize);
}

void Shader::SetBaseColorTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hBaseColorTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetEmissiveColorTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hEmissiveColorTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetNormalTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hNormalTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetMetallicTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hMetallicTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetSpecularTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hSpecularTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetRoughnessTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hRoughnessTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetOpacityTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hOpacityTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetOpacityMaskTexture(const Texture* pTexture)
{
	m_pEffect->SetTexture(m_hOpacityMaskTexture, pTexture->GetD3DTexturePtr());
}

void Shader::SetLight(const void* pLight, unsigned int uSize)
{
	m_pEffect->SetValue(m_hPointLight, pLight, uSize);
}

void Shader::SetSHCoefficients(const SHCoefficients* pCoefficients)
{
	m_pEffect->SetRawValue(m_hSHCoefficients, pCoefficients, 0, sizeof(float)* 12);
}
