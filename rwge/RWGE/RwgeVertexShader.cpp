#include "RwgeVertexShader.h"


RwgeVertexShader::RwgeVertexShader() {
	m_hViewMatrix = GetConstantHandle("ViewMatrix");
	m_hWorldMatrix = GetConstantHandle("WorldMatrix");
	m_hWorldViewProjectionMatrix = GetConstantHandle("WorldViewProjectionMatrix");

	m_hMaterial = GetConstantHandle("Material");
	m_hLight = GetConstantHandle("Light");
}


RwgeVertexShader::~RwgeVertexShader() {

}

void RwgeVertexShader::SetViewTransform(const D3DXMATRIX* pMatrix) {
	m_pConstantTable->SetMatrix(m_pDevice, m_hViewMatrix, pMatrix);
}

void RwgeVertexShader::SetWorldTransform(const D3DXMATRIX* pMatrix) {
	m_pConstantTable->SetMatrix(m_pDevice, m_hWorldMatrix, pMatrix);
}

void RwgeVertexShader::SetWorldViewProjectionTransform(const D3DXMATRIX* pMatrix) {
	m_pConstantTable->SetMatrix(m_pDevice, m_hWorldViewProjectionMatrix, pMatrix);
}

void RwgeVertexShader::SetMaterial(const D3DMATERIAL9* pMaterial) {
	m_pConstantTable->SetValue(m_pDevice, m_hMaterial, (void*)pMaterial, sizeof(D3DMATERIAL9));
}

void RwgeVertexShader::SetLight(const D3DLIGHT9* pLight) {
	m_pConstantTable->SetValue(m_pDevice, m_hLight, (void*)pLight, sizeof(D3DLIGHT9));
}