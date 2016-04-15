#pragma once

class FColorRGBA;

// float类型的RGB
class FColorRGB
{
public:
	FColorRGB();
	FColorRGB(float fGray);
	FColorRGB(float r, float g, float b);
	FColorRGB(const FColorRGB& color);
	FColorRGB(const FColorRGBA& color);
	~FColorRGB();

	void Set(float fGray);
	void Set(float r, float g, float b);
	void Set(const FColorRGB& color);
	void Set(const FColorRGBA& color);

	float fR;
	float fG;
	float fB;

	static const FColorRGB Black;
	static const FColorRGB White;
	static const FColorRGB Gray;
	static const FColorRGB Red;
	static const FColorRGB Green;
	static const FColorRGB Blue;
};

// float类型的RGBA
class FColorRGBA
{
public:
	FColorRGBA();
	FColorRGBA(float fGray);
	FColorRGBA(float r, float g, float b);
	FColorRGBA(float r, float g, float b, float a);
	FColorRGBA(const FColorRGB& color);
	FColorRGBA(const FColorRGBA& color);
	~FColorRGBA();

	void Set(float fGray);
	void Set(float r, float g, float b);
	void Set(float r, float g, float b, float a);
	void Set(const FColorRGB& color);
	void Set(const FColorRGBA& color);

	float fR;
	float fG;
	float fB;
	float fA;
};


