#pragma once

#include "RwgeShaderType.h"
#include "RwgeMaterial.h"
#include "RwgeLight.h"
#include "RwgeD3D9Device.h"
#include <string>
#include <hash_map>
#include <RwgeSingleton.h>

/*
������ɫ���Ĺ���
1.	Ϊ�˱���ÿ��������Ϸʱ��Ҫ���±�����ɫ������Ҫʹ��FXC����ɫ��ִ�����߱��룬��������һ��ͳһ����ɫ���������С�
2.	����һ�����򣬽����߱���õ�����ɫ���ļ���·����һ��64λ�޷������͵�Key���ж�Ӧ��
3.	�����п����õ���Key�洢����Դ�ļ��У�ÿ����Ϸ����ʱ���ظ���Դ�ļ�������Key����õ�·�����ٸ���·����ȡ��ɫ�������Ʊ��룬����һ��<Key��Shader>hash����
4.	��������Ҫ�õ���ɫ��ʱ�����������ݲ��ʵȲ��������64λ��Key������RenderTarget���ҵ���Ӧ��Shader������ָ�롣

64λKey�����ɹ���
λ��Χ		λ��	˵��
MaterialKey
[ 0,  3]	4		BaseColor����ʽ���
[ 4,  7]	4		EmissiveColor����ʽ���
[ 8, 11]	4		Normal����ʽ���
[12, 15]	4		Metallic����ʽ���
[16, 19]	4		Specular����ʽ���
[20, 23]	4		Roughness����ʽ���
[24, 27]	4		Opacity����ʽ���
[28, 31]	4		Opacity����ʽ���
[32, 34]	3		���ʻ��ģʽ���
[35, 36]	2		������ɫģʽ���
[37, 37]	1		˫����ɫ��־
[38, 38]	1		�����Ƿ�Ϊ�ǽ����ı�־
[39, 39]	1		�����Ƿ���ȫ�ֲڵı�־

EnvironmentKey
[40, 41]	2		��������ͱ��
//[40, 40]	1		��ɫ���������������־
����		23		�����ֶ�


1.	һ����ɫ�������������Ӧһ��D3D9Device����һ��ָ����Ͳ��ܱ����
	��Ϊ���е�Shader������Device�󶨵ģ�����Device��ζ����Ҫ���¼������е�Shader��
2.	����D3D9Device�Ĺ�ϵ��ShaderManager��RenderTarget�ǰ󶨵ģ����ܹ��������ڣ�������ֻ����RenderTarget�������ͷš�
*/

class ShaderManager : public Singleton<ShaderManager>
{
public:
	ShaderManager();
	~ShaderManager();

	static unsigned long long GetMaterialKey(const Material* pMaterial);
	static unsigned long long GetEnvironmentKey(const Light* pLight);
	static unsigned long long GetShaderKey(const Material* pMaterial, const Light* pLight);
	static unsigned long long GetShaderKey(const unsigned long long u64MaterialKey, const unsigned long long u64EnvironmentKey);

	static const std::string GetShaderPath(unsigned long long u64Key);

	static const std::string GetFXCCommandLine(const Material* pMaterial, const Light* pLight);
	static const std::string GetFXCCommandLine(const unsigned long long u64Key);

	static bool CompileShader(const Material* pMaterial, const Light* pLight);
	static bool CompileShader(const unsigned long long u64Key);

	ShaderType* GetShaderType(unsigned long long u64Key);

private:
	static std::string m_strFXCPath;
	static std::string m_strTargetVersion;
	static std::string m_strShaderBinaryPrefix;
	static std::string m_strShaderBinaryExtension;
	static std::string m_strHLSLPath;
	static std::string m_strCompilationLogPrefix;

	std::hash_map<unsigned long long, ShaderType> m_hashShaderTypes;
};
