#include "RwgeVertexShader.h"


RwgeVertexShader::RwgeVertexShader() {
	m_hViewMatrix = GetConstantHandle("ViewMatrix");
	m_hViewProjectionMatrix = GetConstantHandle("ViewProjectionMatrix");

	m_hMaterialAmbient = GetConstantHandle("MaterialAmbient");
	m_hMaterialDiffuse = GetConstantHandle("MaterialDiffuse");

	m_hLightDiffuse = GetConstantHandle("LightDiffuse");
	m_hLightAmbient = GetConstantHandle("LightAmbient");
	m_hLightDirection = GetConstantHandle("LightDirection");
}


RwgeVertexShader::~RwgeVertexShader() {

}

void RwgeVertexShader::SetViewTransform(const D3DXMATRIX* pMatrix) {
	m_pConstantTable->SetMatrix(m_pDevice, m_hViewMatrix, pMatrix);
}

void RwgeVertexShader::SetViewProjectionTransform(const D3DXMATRIX* pMatrix) {
	m_pConstantTable->SetMatrix(m_pDevice, m_hViewProjectionMatrix, pMatrix);
}

void RwgeVertexShader::SetMaterial(const D3DMATERIAL9* pMaterial) {
	m_pConstantTable->SetVector(m_pDevice, m_hMaterialAmbient, (D3DXVECTOR4*)&(pMaterial->Ambient));
	m_pConstantTable->SetVector(m_pDevice, m_hMaterialDiffuse, (D3DXVECTOR4*)&(pMaterial->Diffuse));
}

void RwgeVertexShader::SetLight(const D3DLIGHT9* pLight) {
	m_pConstantTable->SetVector(m_pDevice, m_hLightDiffuse, (D3DXVECTOR4*)&(pLight->Diffuse));
	m_pConstantTable->SetVector(m_pDevice, m_hLightAmbient, (D3DXVECTOR4*)&(pLight->Ambient));
	m_pConstantTable->SetVector(m_pDevice, m_hLightDirection, (D3DXVECTOR4*)&(pLight->Direction));
}
