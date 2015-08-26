#include "RwgeVertexShader.h"


RwgeVertexShader::RwgeVertexShader() {
	Initialize("DefaultVertexShader.fx", "Main", Target::VS_2_0);
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

}

void RwgeVertexShader::SetLight(const D3DLIGHT9* pLight) {

}
