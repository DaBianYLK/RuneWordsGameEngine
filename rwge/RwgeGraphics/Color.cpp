#include "Color.h"


FColorRGB::FColorRGB() :
	fR(0.0f),
	fG(0.0f),
	fB(0.0f)
{
}

FColorRGB::FColorRGB(float fGray) :
	fR(fGray),
	fG(fGray),
	fB(fGray)
{
}

FColorRGB::FColorRGB(float r, float g, float b) :
	fR(r),
	fG(g),
	fB(b)
{
}

FColorRGB::FColorRGB(const FColorRGB& color) :
	fR(color.fR),
	fG(color.fG),
	fB(color.fB)
{
}

FColorRGB::FColorRGB(const FColorRGBA& color) :
	fR(color.fR),
	fG(color.fG),
	fB(color.fB)
	
{
}

FColorRGB::~FColorRGB()
{
}

void FColorRGB::Set(float fGray)
{
	fR = fGray;
	fG = fGray;
	fB = fGray;
}

void FColorRGB::Set(float r, float g, float b)
{
	fR = r;
	fG = g;
	fB = b;
}

void FColorRGB::Set(const FColorRGB& color)
{
	fR = color.fR;
	fG = color.fG;
	fB = color.fB;
}

void FColorRGB::Set(const FColorRGBA& color)
{
	fR = color.fR;
	fG = color.fG;
	fB = color.fB;
}

FColorRGBA::FColorRGBA() :
	fR(0.0f),
	fG(0.0f),
	fB(0.0f),
	fA(1.0f)
{
}

FColorRGBA::FColorRGBA(float fGray) :
	fR(fGray),
	fG(fGray),
	fB(fGray),
	fA(1.0f)
{
}

FColorRGBA::FColorRGBA(float r, float g, float b) :
	fR(r),
	fG(g),
	fB(b),
	fA(1.0f)
{
}

FColorRGBA::FColorRGBA(float r, float g, float b, float a) :
	fR(r),
	fG(g),
	fB(b),
	fA(a)
{
}

FColorRGBA::FColorRGBA(const FColorRGB& color) :
	fR(color.fR),
	fG(color.fG),
	fB(color.fB),
	fA(1.0f)
{
}

FColorRGBA::FColorRGBA(const FColorRGBA& color) :
	fR(color.fR),
	fG(color.fG),
	fB(color.fB),
	fA(color.fA)
{
}

FColorRGBA::~FColorRGBA()
{
}

void FColorRGBA::Set(float fGray)
{
	fR = fGray;
	fG = fGray;
	fB = fGray;
	fA = 1.0f;
}

void FColorRGBA::Set(float r, float g, float b)
{
	fR = r;
	fG = g;
	fB = b;
	fA = 1.0f;
}

void FColorRGBA::Set(float r, float g, float b, float a)
{
	fR = r;
	fG = g;
	fB = b;
	fA = a;
}

void FColorRGBA::Set(const FColorRGB& color)
{
	fR = color.fR;
	fG = color.fG;
	fB = color.fB;
	fA = 1.0f;
}

void FColorRGBA::Set(const FColorRGBA& color)
{
	fR = color.fR;
	fG = color.fG;
	fB = color.fB;
	fA = color.fA;
}
