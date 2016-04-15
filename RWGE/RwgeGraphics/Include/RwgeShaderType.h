#pragma once

#include <string>

class RenderTarget;
class Shader;

class ShaderType
{
	friend class ShaderManager;

private:
	ShaderType(unsigned long long u64Key);

public:
	~ShaderType();

	Shader* GetShader(RenderTarget* pRenderTarget);
	unsigned long long GetShaderKey() const;
	const std::string& GetShaderPath() const;

private:
	unsigned long long	m_uShaderKey;
	std::string			m_strFilePath;
};

