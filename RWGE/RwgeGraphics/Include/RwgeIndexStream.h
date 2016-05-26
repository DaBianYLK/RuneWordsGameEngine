/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-20
	DESC :	索引缓冲，用于向D3D 提交索引数据
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

struct IDirect3DIndexBuffer9;

struct IndexStream
{
	static const unsigned char	u8IndexSize = 2;	// 一个索引的字节数

	unsigned int			u32IndexCount;					// 流中的索引个数
	unsigned int			u32StreamSize;					// 整个索引流的字节数
	unsigned short*			aryIndices;						// 索引流数据指针

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

