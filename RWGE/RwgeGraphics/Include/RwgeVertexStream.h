/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-20
	DESC :	
	1.	VertexStream用于标记一段顶点数据在顶点缓冲中的组织形式，如偏移量（Offset）、步长（Stride）、大小（Size）等
	2.	是否需要将顶点缓冲定义为双StreamSourcce （一个存储变化数据，一个存储不变数据）是一个性能的取舍问题：
		A.	SetStreamSource占用的CPU时钟周期约为3700 - 5800 ，约是DP开销的四倍
		B.	Lock/ Unlock一段顶点缓冲的开销取决于数据量的大小
		当B 的性能消耗大于A 时，设置双StreamSource能够提升性能
	3.	根据D3D 官方的性能优化文档，将所有静态对象的顶点流数据合并到一个顶点缓冲中存储效率更高——所有静态对象的顶点
		流每帧只需要提交一次
	4.	关于VertexStream和VertexBuffer的说明：
		VertexStream封装了需要提交给显卡的顶点数据，VertexBuffer封装了D3D 的顶点缓冲对象，二者是独立的概念：一个顶点
		缓冲可以同时存放多个顶点流的数据，顶点流可以根据需要绑定在不同的顶点缓冲中（同一时间只能绑定于一个缓冲）
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

struct IDirect3DVertexBuffer9;

struct VertexStream
{
	unsigned char			u8VertexSize;		// 一个顶点的字节数，即步长Stride
	unsigned int			u32VertexCount;		// 流中的顶点个数
	unsigned int			u32StreamSize;		// 整个顶点流的字节数
	const void*				aryVertices;		// 顶点流数据指针

	// ============== 以下数据在顶点流绑定到顶点缓冲后被设置 ==============
	IDirect3DVertexBuffer9*	pD3dVertexBuffer;	// 顶点流绑定的顶点缓冲区
	unsigned int			u32StreamOffset;	// 顶点流在顶点缓冲区中的偏移字节数

	VertexStream() :
		u8VertexSize(0),
		u32VertexCount(0),
		u32StreamSize(0),
		aryVertices(nullptr),
		pD3dVertexBuffer(nullptr),
		u32StreamOffset(0)
	{

	}

	VertexStream(unsigned char u8Size, unsigned int u32Count, void* aryVertices) :
		u8VertexSize(u8Size),
		u32VertexCount(u32Count),
		u32StreamSize(u8VertexSize * u32VertexCount),
		aryVertices(aryVertices),
		pD3dVertexBuffer(nullptr),
		u32StreamOffset(0)
	{

	}
};

