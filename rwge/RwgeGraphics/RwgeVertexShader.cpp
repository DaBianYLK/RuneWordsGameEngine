//#include "RwgeVertexShader.h"
//
//RwgeVertexShader::RwgeVertexShader()
//{
//	m_hViewMatrix = GetConstantHandle("g_ViewMatrix");
//	m_hWorldMatrix = GetConstantHandle("g_WorldMatrix");
//	m_hWorldViewProjectionMatrix = GetConstantHandle("g_WvpMatrix");
//
//#ifdef SHADER_ANIMATION
//	m_hModelMatrices = GetConstantHandle("g_ModelMatrices");
//#endif
//
//	m_hMaterial = GetConstantHandle("g_Material");
//	m_hLight = GetConstantHandle("g_Light");
//}
//
//
//RwgeVertexShader::~RwgeVertexShader()
//{
//
//}
//
//void RwgeVertexShader::SetViewTransform(const D3DXMATRIX* pMatrix)
//{
//	m_pConstantTable->SetMatrix(m_pDevice, m_hViewMatrix, pMatrix);
//}
//
//void RwgeVertexShader::SetWorldTransform(const D3DXMATRIX* pMatrix)
//{
//	m_pConstantTable->SetMatrix(m_pDevice, m_hWorldMatrix, pMatrix);
//}
//
//void RwgeVertexShader::SetWorldViewProjectionTransform(const D3DXMATRIX* pMatrix)
//{
//	m_pConstantTable->SetMatrix(m_pDevice, m_hWorldViewProjectionMatrix, pMatrix);
//}
//
//#ifdef SHADER_ANIMATION
//void RwgeVertexShader::SetModelTransform(const D3DXMATRIX* matrices, unsigned int num)
//{
//	m_pConstantTable->SetMatrixArray(m_pDevice, m_hModelMatrices, matrices, num);
//}
//#endif
//
//void RwgeVertexShader::SetMaterial(const D3DMATERIAL9* pMaterial)
//{
//	m_pConstantTable->SetValue(m_pDevice, m_hMaterial, (void*)pMaterial, sizeof(D3DMATERIAL9));
//}
//
//void RwgeVertexShader::SetLight(const D3DLIGHT9* pLight)
//{
//	m_pConstantTable->SetValue(m_pDevice, m_hLight, (void*)pLight, sizeof(D3DLIGHT9));
//}