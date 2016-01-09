#include "VertexShader.h"

#include "AppConfig.h"
#include "Graphics.h"

VertexShader::VertexShader()
{
	Initialize(AppConfig::defaultVertexShaderPath, AppConfig::defaultVertexShaderEntryFuncName, (Target)AppConfig::defaultVertexShaderTarget);
}

VertexShader::VertexShader(const char* sourceFilePath)
{
	Initialize(sourceFilePath, AppConfig::defaultVertexShaderEntryFuncName, (Target)AppConfig::defaultVertexShaderTarget);
}

VertexShader::VertexShader(const char* sourceFilePath, const char* entryFuncName)
{
	Initialize(sourceFilePath, entryFuncName, (Target)AppConfig::defaultVertexShaderTarget);
}

VertexShader::VertexShader(const char* sourceFilePath, Target target)
{
	Initialize(sourceFilePath, AppConfig::defaultVertexShaderEntryFuncName, target);
}

VertexShader::VertexShader(const char* sourceFilePath, const char* entryFuncName, Target target)
{
	Initialize(sourceFilePath, entryFuncName, target);
}

VertexShader::~VertexShader()
{

}

void VertexShader::Initialize(const char* sourceFilePath, const char* entryFuncName, Target target)
{
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();

	// 编译着色器
	const char* targetString;
	if (target == VS_1_1)
{
		targetString = "vs_1_1";
	}
	else if (target == VS_2_0)
{
		targetString = "vs_2_0";
	}
	else if (target == VS_3_0)
{
		targetString = "vs_3_0";
	}
	else
{
		targetString = "vs_2_0";
	}

	ID3DXBuffer* pShaderBuffer = nullptr;
	ID3DXBuffer* pErrorBuffer = nullptr;

	HRESULT result = D3DXCompileShaderFromFile(
		sourceFilePath,
		0,
		0,
		entryFuncName,
		targetString,
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
		&pShaderBuffer,
		&pErrorBuffer,
		&m_pConstantTable);

	// output any error messages
	if (pErrorBuffer)
{
		MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
		
		pErrorBuffer->Release();
	}

	if (FAILED(result))
{
		MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);

		return;
	}

	m_SourceFilePath = sourceFilePath;
	m_EntryFuncName = entryFuncName;
	m_Target = target;

	// 创建着色器
	result = m_pDevice->CreateVertexShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pShader);

	if (FAILED(result))
{
		MessageBox(0, "CreateVertexShader - FAILED", 0, 0);

		return;
	}

	pShaderBuffer->Release();

	// 设置常量表
	m_pConstantTable->SetDefaults(m_pDevice);
}

void VertexShader::SetBool(D3DXHANDLE handle, BOOL b)
{
	m_pConstantTable->SetBool(m_pDevice, handle, b);
}

void VertexShader::SetBoolArray(D3DXHANDLE handle, const BOOL* booleans, unsigned int n)
{
	m_pConstantTable->SetBoolArray(m_pDevice, handle, booleans, n);
}

void VertexShader::SetFloat(D3DXHANDLE handle, float f)
{
	m_pConstantTable->SetFloat(m_pDevice, handle, f);
}

void VertexShader::SetFloatArray(D3DXHANDLE handle, const float* floats, unsigned int n)
{
	m_pConstantTable->SetFloatArray(m_pDevice, handle, floats, n);
}

void VertexShader::SetInt(D3DXHANDLE handle, int x)
{
	m_pConstantTable->SetInt(m_pDevice, handle, x);
}

void VertexShader::SetIntArray(D3DXHANDLE handle, const int* integers, unsigned int n)
{
	m_pConstantTable->SetIntArray(m_pDevice, handle, integers, n);
}

void VertexShader::SetMatrix(D3DXHANDLE handle, const D3DXMATRIX* pMatrix)
{
	m_pConstantTable->SetMatrix(m_pDevice, handle, pMatrix);
}

void VertexShader::SetMatrixArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n)
{
	m_pConstantTable->SetMatrixArray(m_pDevice, handle, matrices, n);
}

void VertexShader::SetMatrixPointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n)
{
	m_pConstantTable->SetMatrixPointerArray(m_pDevice, handle, ppMatrices, n);
}

void VertexShader::SetMatrixTranspose(D3DXHANDLE handle, const D3DXMATRIX* pMatrix)
{
	m_pConstantTable->SetMatrixTranspose(m_pDevice, handle, pMatrix);
}

void VertexShader::SetMatrixTransposeArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n)
{
	m_pConstantTable->SetMatrixTransposeArray(m_pDevice, handle, matrices, n);
}

void VertexShader::SetMatrixTransposePointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n)
{
	m_pConstantTable->SetMatrixTransposePointerArray(m_pDevice, handle, ppMatrices, n);
}

void VertexShader::SetVector(D3DXHANDLE handle, const D3DXVECTOR4* pVector)
{
	m_pConstantTable->SetVector(m_pDevice, handle, pVector);
}

void VertexShader::SetVectorArray(D3DXHANDLE handle, const D3DXVECTOR4* vectors, unsigned int n)
{
	m_pConstantTable->SetVectorArray(m_pDevice, handle, vectors, n);
}

void VertexShader::SetValue(D3DXHANDLE handle, const void* pValue, unsigned int byteNum)
{
	m_pConstantTable->SetValue(m_pDevice, handle, pValue, byteNum);
}

D3DXHANDLE VertexShader::GetConstantHandle(const char* name)
{
	return m_pConstantTable->GetConstantByName(0, name);
}

void VertexShader::Enable()
{
	m_pDevice->SetVertexShader(m_pShader);
}