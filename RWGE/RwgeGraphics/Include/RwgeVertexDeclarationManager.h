/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-18
	DESC :	
	1.	�������ɶ�������

	ToDo��
	2016-05-18
		��ʱֻ֧��ͨ��VertexDeclarationTemplate���ɶ�������������Ҫ�ṩ���ļ����ض��������Ĺ���
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string>
#include <map>
#include <RwgeObject.h>
#include <RwgeSingleton.h>
#include "RwgeD3d9VertexDeclaration.h"

class RVertexDeclarationManager : 
	public RObject,
	public Singleton<RVertexDeclarationManager>
{
public:
	RVertexDeclarationManager();
	~RVertexDeclarationManager();

	RD3d9VertexDeclaration* GetDefaultVertexDeclaration();

private:
	void GenerateDefaultVertexDeclaration();

private:
	std::map<std::string, RD3d9VertexDeclaration*> m_mapVertexDeclarations;
};
