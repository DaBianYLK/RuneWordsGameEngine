#ifndef __TEXTURE__
#define __TEXTURE__

#define TEXTURE_DEFINE(Name) \
	texture2D Name; \
	sampler Name##Sampler = sampler_state \
	{ \
		Texture = <Name>; \
		MinFilter = LINEAR; \
		MagFilter = LINEAR; \
		MipFilter = LINEAR; \
	}


#define TEXTURE_SAMPLE(Name, texCoord)	tex2D(Name##Sampler, texCoord)

#endif // __TEXTURE__