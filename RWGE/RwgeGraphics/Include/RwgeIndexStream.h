/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-20
	DESC :	�������壬������D3D �ύ��������
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

struct IDirect3DIndexBuffer9;

struct IndexStream
{
	static const unsigned char	u8IndexSize = 2;	// һ���������ֽ���

	unsigned int			u32IndexCount;					// ���е���������
	unsigned int			u32StreamSize;					// �������������ֽ���
	unsigned short*			aryIndices;						// ����������ָ��

	IDirect3DIndexBuffer9*	pD3dIndexBuffer;

	IndexStream() :
		u32IndexCount(0),
		u32StreamSize(0),
		aryIndices(nullptr),
		pD3dIndexBuffer(nullptr)
	{
		
	}

	IndexStream(unsigned int u32Count, unsigned short* aryIndices) : 
		u32IndexCount(u32Count),
		u32StreamSize(u8IndexSize * u32IndexCount),
		aryIndices(aryIndices),
		pD3dIndexBuffer(nullptr)
	{

	}
};

