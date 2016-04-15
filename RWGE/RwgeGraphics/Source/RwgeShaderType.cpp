#include "RwgeShaderType.h"

#include "RwgeShader.h"
#include "RwgeShaderManager.h"
#include "RwgeRenderTarget.h"


ShaderType::ShaderType(unsigned long long u64Key) :
	m_uShaderKey(u64Key),
	m_strFilePath(ShaderManager::GetShaderPath(u64Key))
{

}


ShaderType::~ShaderType()
{
}

Shader* ShaderType::GetShader(RenderTarget* pRenderTarget)
{
	return pRenderTarget->GetShader(this);
}

unsigned long long ShaderType::GetShaderKey() const
{
	return m_uShaderKey;
}

const std::string& ShaderType::GetShaderPath() const
{
	return m_strFilePath;
}