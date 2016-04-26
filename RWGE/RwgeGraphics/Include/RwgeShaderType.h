#pragma once

#include <string>

class RenderTarget;
class RShader;

class ShaderType
{
	friend class RShaderManager;

private:
	ShaderType(unsigned long long u64Key);

public:
	~ShaderType();

	RShader* GetShader(RenderTarget* pRenderTarget);
	unsigned long long GetShaderKey() const;
	const std::string& GetShaderPath() const;

private:
	unsigned long long	m_uShaderKey;
	std::string			m_strFilePath;
};

