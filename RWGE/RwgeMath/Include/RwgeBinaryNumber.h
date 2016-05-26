/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-25
	DESC :	用于使用二进制表示常量（注意：只可用于表示常量，不可传入宏定义或变量名作为参数）
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

namespace RwgeMath
{
	static const unsigned char b0000 = 0x0;
	static const unsigned char b0001 = 0x1;
	static const unsigned char b0010 = 0x2;
	static const unsigned char b0011 = 0x3;
	static const unsigned char b0100 = 0x4;
	static const unsigned char b0101 = 0x5;
	static const unsigned char b0110 = 0x6;
	static const unsigned char b0111 = 0x7;
	static const unsigned char b1000 = 0x8;
	static const unsigned char b1001 = 0x9;
	static const unsigned char b1010 = 0xA;
	static const unsigned char b1011 = 0xB;
	static const unsigned char b1100 = 0xC;
	static const unsigned char b1101 = 0xD;
	static const unsigned char b1110 = 0xE;
	static const unsigned char b1111 = 0xF;
}

#define b08(high4Bit, low4Bit)																(RwgeMath::b##high4Bit <<  4 | RwgeMath::b##low4Bit)

#define b16(high4Bit, mid4Bit1, mid4Bit0, low4Bit)											(b08(high4Bit, mid4Bit1) << 8 | b08(mid4Bit0, low4Bit))

#define b32(high4Bit, mid4Bit5, mid4Bit4, mid4Bit3, mid4Bit2, mid4Bit1, mid4Bit0, low4Bit)	\
	(b16(high4Bit, mid4Bit5, mid4Bit4, mid4Bit3) << 16 | b16(mid4Bit2, mid4Bit1, mid4Bit0, low4Bit))

#define b64(high4Bit, mid4Bit13, mid4Bit12, mid4Bit11, mid4Bit10, mid4Bit9, mid4Bit8, mid4Bit7, mid4Bit6, mid4Bit5, mid4Bit4, mid4Bit3, mid4Bit2, mid4Bit1, mid4Bit0, low4Bit)	\
	(b32(high4Bit, mid4Bit13, mid4Bit12, mid4Bit11, mid4Bit10, mid4Bit9, mid4Bit8, mid4Bit7) << 32 | b32(mid4Bit6, mid4Bit5, mid4Bit4, mid4Bit3, mid4Bit2, mid4Bit1, mid4Bit0, low4Bit))