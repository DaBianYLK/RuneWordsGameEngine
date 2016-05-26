/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-03
	DESC :	
	1.	����ฺ������ɫ���ı����������
	2.	������ɫ���Ĺ���
		A.	Ϊ�˱���ÿ��������Ϸʱ�����±�����ɫ����ʹ��FXC ִ�����߱��룬������ɫ�����������ݱ�������Դ�ļ��С�
		B.	��Ҫ��ȡShaderʱ�����ȸ��ݲ��ʺͻ����õ�ShaderKey ��ʹ��ShaderKey ��һ��ȫ�ֵ�<ShaderKey, Shader> hash��
			�в��ң����ҳɹ�ʱ����Shader���������ShaderKey ����Դ�ļ��м�����ɫ��������ʧ��ʱ�����±�����ɫ����
	3.	��ɫ������������ָ��D3D9Device����һ��ָ����Ͳ��ܱ������Ϊ���е�Shader������Device�󶨵ģ�����Device��ζ��
		��Ҫ���¼������е�Shader
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeSingleton.h>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include "RwgeD3d9Shader.h"

#define USING_HASH_MAP		1		// �Ƿ�ʹ��hahs����Shader	

#if USING_HASH_MAP
#	include <unordered_map>
typedef std::unordered_map<RShaderKey, RD3d9Shader*, RShaderKey::Hash, RShaderKey::Equal> ShaderMap;
#else
#	include <map>
typedef std::map<RShaderKey, RShader*> ShaderMap;
#endif

class RD3d9ShaderManager : 
	public RObject,
	public Singleton<RD3d9ShaderManager>
{
public:
	RD3d9ShaderManager();
	~RD3d9ShaderManager();

	static bool CompileShader(const RShaderKey& key);
	RD3d9Shader* GetShader(const RShaderKey& key);

	RD3d9Shader* GetSharedShader();				// ������ɫ��ӳ����еĵ�һ����ɫ�������ӳ���Ϊ���򷵻�nullptr

private:
	RD3d9Shader*		m_pSharedShader;		// �������ù�������Shader
	ShaderMap			m_mapShaders;

	LPD3DXEFFECTPOOL	m_pEffectPool;			// ������Shader֮�乲����

	static bool			m_bRecompileShader;		// �Ƿ�����Ϸ����ʱ���±������е�Shader
};

