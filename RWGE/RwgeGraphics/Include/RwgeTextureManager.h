/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-05
	DESC :	��������Ĵ��������
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeSingleton.h>
#include <map>
#include <RwgeTString.h>
#include <RwgeObject.h>

class RD3d9Texture;

class RTextureManager : 
	public RObject,
	public Singleton<RTextureManager>
{
public:
	RTextureManager();
	~RTextureManager();

	RD3d9Texture* GetTexture(const Rwge::tstring& strPath);

private:
	std::map<Rwge::tstring, RD3d9Texture> m_mapTextures;
};

