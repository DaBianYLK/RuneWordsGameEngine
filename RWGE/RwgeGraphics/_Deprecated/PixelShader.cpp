//#include "PixelShader.h"
//
//#include "AppConfig.h"
//#include "Graphics.h"
//
//PixelShader::PixelShader()
//{
//	Initialize(AppConfig::defaultPixelShaderPath, AppConfig::defaultPixelShaderEntryFuncName, (Target)AppConfig::defaultPixelShaderTarget);
//}
//
//PixelShader::PixelShader(const char* sourceFilePath)
//{
//	Initialize(sourceFilePath, AppConfig::defaultPixelShaderEntryFuncName, (Target)AppConfig::defaultPixelShaderTarget);
//}
//
//PixelShader::PixelShader(const char* sourceFilePath, const char* entryFuncName)
//{
//	Initialize(sourceFilePath, entryFuncName, (Target)AppConfig::defaultPixelShaderTarget);
//}
//
//PixelShader::PixelShader(const char* sourceFilePath, Target target)
//{
//	Initialize(sourceFilePath, AppConfig::defaultPixelShaderEntryFuncName, target);
//}
//
//PixelShader::PixelShader(const char* sourceFilePath, const char* entryFuncName, Target target)
//{
//	Initialize(sourceFilePath, entryFuncName, target);
//}
//
//PixelShader::~PixelShader()
//{
//
//}
//
//void PixelShader::Initialize(const char* sourceFilePath, const char* entryFuncName, Target target)
//{
//	m_pDevice = Graphics::GetInstance()->GetD3D9Device();
//
//	// 编译着色器
//	const char* targetString;
//	if (target == PS_1_1)
//{
//		targetString = "ps_1_1";
//	}
//	else if (target == PS_2_0)
//{
//		targetString = "ps_2_0";
//	}
//	else if (target == PS_3_0)
//{
//		targetString = "ps_3_0";
//	}
//	else
//{
//		targetString = "ps_2_0";
//	}
//
//	ID3DXBuffer* pShaderBuffer = nullptr;
//	ID3DXBuffer* pErrorBuffer = nullptr;
//
//	HRESULT result = D3DXCompileShaderFromFile(
//		sourceFilePath,
//		0,
//		0,
//		entryFuncName,
//		targetString,
//		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
//		&pShaderBuffer,
//		&pErrorBuffer,
//		&m_pConstantTable);
//
//	// output any error messages
//	if (pErrorBuffer)
//{
//		MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
//
//		pErrorBuffer->Release();
//	}
//
//	if (FAILED(result))
//{
//		MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
//
//		return;
//	}
//
//	m_SourceFilePath = sourceFilePath;
//	m_EntryFuncName = entryFuncName;
//	m_Target = target;
//
//	// 创建着色器
//	result = m_pDevice->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pShader);
//
//	if (FAILED(result))
//{
//		MessageBox(0, "CreatePixelShader - FAILED", 0, 0);
//
//		return;
//	}
//
//	pShaderBuffer->Release();
//
//	// 设置常量表
//	m_pConstantTable->SetDefaults(m_pDevice);
//}
//
//void PixelShader::SetBool(D3DXHANDLE handle, BOOL b)
//{
//	m_pConstantTable->SetBool(m_pDevice, handle, b);
//}
//
//void PixelShader::SetBoolArray(D3DXHANDLE handle, const BOOL* booleans, unsigned int n)
//{
//	m_pConstantTable->SetBoolArray(m_pDevice, handle, booleans, n);
//}
//
//void PixelShader::SetFloat(D3DXHANDLE handle, float f)
//{
//	m_pConstantTable->SetFloat(m_pDevice, handle, f);
//}
//
//void PixelShader::SetFloatArray(D3DXHANDLE handle, const float* floats, unsigned int n)
//{
//	m_pConstantTable->SetFloatArray(m_pDevice, handle, floats, n);
//}
//
//void PixelShader::SetInt(D3DXHANDLE handle, int x)
//{
//	m_pConstantTable->SetInt(m_pDevice, handle, x);
//}
//
//void PixelShader::SetIntArray(D3DXHANDLE handle, const int* integers, unsigned int n)
//{
//	m_pConstantTable->SetIntArray(m_pDevice, handle, integers, n);
//}
//
//void PixelShader::SetMatrix(D3DXHANDLE handle, const D3DXMATRIX* pMatrix)
//{
//	m_pConstantTable->SetMatrix(m_pDevice, handle, pMatrix);
//}
//
//void PixelShader::SetMatrixArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n)
//{
//	m_pConstantTable->SetMatrixArray(m_pDevice, handle, matrices, n);
//}
//
//void PixelShader::SetMatrixPointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n)
//{
//	m_pConstantTable->SetMatrixPointerArray(m_pDevice, handle, ppMatrices, n);
//}
//
//void PixelShader::SetMatrixTranspose(D3DXHANDLE handle, const D3DXMATRIX* pMatrix)
//{
//	m_pConstantTable->SetMatrixTranspose(m_pDevice, handle, pMatrix);
//}
//
//void PixelShader::SetMatrixTransposeArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n)
//{
//	m_pConstantTable->SetMatrixTransposeArray(m_pDevice, handle, matrices, n);
//}
//
//void PixelShader::SetMatrixTransposePointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n)
//{
//	m_pConstantTable->SetMatrixTransposePointerArray(m_pDevice, handle, ppMatrices, n);
//}
//
//void PixelShader::SetVector(D3DXHANDLE handle, const D3DXVECTOR4* pVector)
//{
//	m_pConstantTable->SetVector(m_pDevice, handle, pVector);
//}
//
//void PixelShader::SetVectorArray(D3DXHANDLE handle, const D3DXVECTOR4* vectors, unsigned int n)
//{
//	m_pConstantTable->SetVectorArray(m_pDevice, handle, vectors, n);
//}
//
//void PixelShader::SetValue(D3DXHANDLE handle, const void* pValue, unsigned int byteNum)
//{
//	m_pConstantTable->SetValue(m_pDevice, handle, pValue, byteNum);
//}
//
//D3DXHANDLE PixelShader::GetConstantHandle(const char* name)
//{
//	return m_pConstantTable->GetConstantByName(0, name);
//}
//
//void PixelShader::Enable()
//{
//	m_pDevice->SetPixelShader(m_pShader);
//}