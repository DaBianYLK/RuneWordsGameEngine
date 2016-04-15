//#pragma once
//
//#include <d3dx9.h>
//
//class VertexShader
//{
//public:
//	enum Target
//{
//		VS_1_1 = 0,
//		VS_2_0 = 1,
//		VS_3_0 = 2
//	};
//
//public:
//	VertexShader();
//	VertexShader(const char* sourceFilePath);
//	VertexShader(const char* sourceFilePath, const char* entryFuncName);
//	VertexShader(const char* sourceFilePath, Target target);
//	VertexShader(const char* sourceFilePath, const char* entryFuncName, Target target);
//	~VertexShader();
//
//protected:
//	void Initialize(const char* sourceFilePath, const char* entryFuncName, Target target);
//
//public:
//	void SetBool(D3DXHANDLE handle, BOOL b);
//	void SetBoolArray(D3DXHANDLE handle, const BOOL* booleans, unsigned int n);
//	void SetFloat(D3DXHANDLE handle, float f);
//	void SetFloatArray(D3DXHANDLE handle, const float* floats, unsigned int n);
//	void SetInt(D3DXHANDLE handle, int x);
//	void SetIntArray(D3DXHANDLE handle, const int* integers, unsigned int n);
//	void SetMatrix(D3DXHANDLE handle, const D3DXMATRIX* pMatrix);
//	void SetMatrixArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n);
//	void SetMatrixPointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n);
//	void SetMatrixTranspose(D3DXHANDLE handle, const D3DXMATRIX* pMatrix);
//	void SetMatrixTransposeArray(D3DXHANDLE handle, const D3DXMATRIX* matrices, unsigned int n);
//	void SetMatrixTransposePointerArray(D3DXHANDLE handle, const D3DXMATRIX** ppMatrices, unsigned int n);
//	void SetVector(D3DXHANDLE handle, const D3DXVECTOR4* pVector);
//	void SetVectorArray(D3DXHANDLE handle, const D3DXVECTOR4* vectors, unsigned int n);
//	void SetValue(D3DXHANDLE handle, const void* pValue, unsigned int byteNum);
//
//	D3DXHANDLE GetConstantHandle(const char* name);
//
//	void Enable();
//
//protected:
//	IDirect3DDevice9* m_pDevice;
//
//	IDirect3DVertexShader9* m_pShader;
//	ID3DXConstantTable* m_pConstantTable;
//	
//	const char* m_SourceFilePath;
//	Target m_Target;
//	const char* m_EntryFuncName;
//};
//
