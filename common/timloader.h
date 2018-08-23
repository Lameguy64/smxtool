#ifndef _TIMLOADER_H
#define _TIMLOADER_H

#include <stdio.h>
#include <malloc.h>

typedef struct {
	unsigned int id;
	struct {
		unsigned int pmode:3;
		unsigned int cf:1;
		unsigned int reserved:28;
	} flag;
} TIM_HEADER;

typedef struct {
	unsigned int	bnum;
	unsigned short	x,y;
	unsigned short	w,h;
} TIM_BLOCK;

typedef struct {
	unsigned short	r:5;
	unsigned short	g:5;
	unsigned short	b:5;
	unsigned short	i:1;
} TIM_PIXEL;

enum TIM_ERROR {
	TIM_ERROR_NONE = 0,
	TIM_ERROR_NOTFOUND,
	TIM_ERROR_INVALID
};

class TimLoaderClass {
public:

	TimLoaderClass();
	virtual ~TimLoaderClass();

	TIM_ERROR LoadFile(const char* filename);
	void ConvertToRGBA(void *output);

	int GetWidth();
	int GetHeight();

	TIM_HEADER	header;
	TIM_BLOCK	clutBlock;
	TIM_BLOCK	pixelBlock;

	TIM_PIXEL	*clut;
    void		*pixels;

};

#endif // _TIMLOADER_H
