#pragma once

struct FColorRGBA;

struct FColorRGB
{
	FColorRGB();
	FColorRGB(float fGray);
	FColorRGB(float fR, float fG, float fB);
	FColorRGB(const FColorRGB& color);
	FColorRGB(const FColorRGBA& color);
	~FColorRGB();

	void Set(float fGray);
	void Set(float fR, float fG, float fB);
	void Set(const FColorRGB& color);
	void Set(const FColorRGBA& color);

	float R;
	float G;
	float B;

	static const FColorRGB Black;
	static const FColorRGB White;
	static const FColorRGB Gray;
	static const FColorRGB Red;
	static const FColorRGB Green;
	static const FColorRGB Blue;
};

struct FColorRGBA
{
	FColorRGBA();
	FColorRGBA(float fGray);
	FColorRGBA(float fR, float fG, float fB);
	FColorRGBA(float fR, float fG, float fB, float fA);
	FColorRGBA(const FColorRGB& color);
	FColorRGBA(const FColorRGBA& color);
	~FColorRGBA();

	void Set(float fGray);
	void Set(float fR, float fG, float fB);
	void Set(float fR, float fG, float fB, float fA);
	void Set(const FColorRGB& color);
	void Set(const FColorRGBA& color);

	float R;
	float G;
	float B;
	float A;
};


