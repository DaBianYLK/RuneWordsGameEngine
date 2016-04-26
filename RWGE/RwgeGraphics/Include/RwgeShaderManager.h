/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-03-01
	DESC :	
	1.	����ฺ������ɫ���ı����������
	2.	������ɫ���Ĺ���
		A.	Ϊ�˱���ÿ��������Ϸʱ�����±�����ɫ����ʹ��FXC ִ�����߱��룬������ɫ�����������ݱ�������Դ�ļ��С�
		B.	��Ҫ��ȡShaderʱ�����ȸ��ݲ��ʺͻ����õ�ShaderKey ��ʹ��ShaderKey ��һ��ȫ�ֵ�<ShaderKey, Shader> hash��
			�в��ң����ҳɹ�ʱ����Shader���������ShaderKey ����Դ�ļ��м�����ɫ��������ʧ��ʱ�����±�����ɫ����
	3.	��ɫ������������ָ��D3D9Device����һ��ָ����Ͳ��ܱ������Ϊ���е�Shader������Device�󶨵ģ�����Device��ζ��
		��Ҫ���¼������е�Shader��
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string>
#include <hash_map>
#include <RwgeSingleton.h>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include "RwgeShader.h"
#include "RwgeMaterial.h"
#include "RwgeLight.h"
#include "RwgeD3d9Device.h"

class RShaderManager : 
	public RObject,
	public Singleton<RShaderManager>
{
public:
	RShaderManager();
	~RShaderManager();

	__forceinline static const MaterialKey& GetMaterialKey(const RMaterial* pMaterial)
	{
		return pMaterial->GetMaterialKey();
	};

	static const EnvironmentKey& GetEnvironmentKey(const Light* pLight);
	static RShaderKey GetShaderKey(const MaterialKey& matKey, const EnvironmentKey& envKey);

	static bool CompileShader(const RShaderKey& key);

private:
	std::hash_map<RShaderKey, RShader> m_hashShaders;
};

