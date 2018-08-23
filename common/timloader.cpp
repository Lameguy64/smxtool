#include "timloader.h"


TimLoaderClass::TimLoaderClass() {

	clut = NULL;
	pixels = NULL;

}

TimLoaderClass::~TimLoaderClass() {

	if (clut != NULL)
		free(clut);

	if (pixels != NULL)
		free(pixels);

}

TIM_ERROR TimLoaderClass::LoadFile(const char* filename) {

    FILE* fp = fopen(filename, "rb");

	if (fp == 0)
		return TIM_ERROR_NOTFOUND;

	fread(&header, sizeof(TIM_HEADER), 1, fp);

	if (header.id != 0x00000010) {

		fclose(fp);
		return TIM_ERROR_INVALID;

	}

	if (clut != NULL)
		free(clut);

	if (pixels != NULL)
		free(pixels);

	// Is CLUT present?
	if (header.flag.cf) {

        fread(&clutBlock, sizeof(TIM_BLOCK), 1, fp);

		/*
		printf("CLUT Stats:\n");
        printf("BNUM:%d\n", clutBlock.bnum);
        printf("X:%d Y:%d\n", clutBlock.x, clutBlock.y);
        printf("W:%d H:%d\n", clutBlock.w, clutBlock.h);
        */

		clut = (TIM_PIXEL*)malloc(sizeof(TIM_PIXEL)*clutBlock.w);
		fread(clut, sizeof(TIM_PIXEL), clutBlock.w, fp);

	}

	fread(&pixelBlock, sizeof(TIM_BLOCK), 1, fp);

	/*
	printf("PIXEL Stats:\n");
	printf("BNUM:%d\n", pixelBlock.bnum);
	printf("X:%d Y:%d\n", pixelBlock.x, pixelBlock.y);
	printf("W:%d H:%d\n", pixelBlock.w, pixelBlock.h);
	*/

	pixels = malloc((2*pixelBlock.w)*pixelBlock.h);
	fread(pixels, 2*pixelBlock.w, pixelBlock.h, fp);

	fclose(fp);

	return TIM_ERROR_NONE;

}

void TimLoaderClass::ConvertToRGBA(void *output) {

	typedef struct {
		unsigned char	r;
		unsigned char	g;
		unsigned char	b;
		unsigned char	a;
	} PIXEL_RGB;

	switch(header.flag.pmode) {
	case 0:		// 4-bit
	{

		unsigned char	*srcpix = (unsigned char*)pixels;
		PIXEL_RGB		*dstpix = (PIXEL_RGB*)output;

        for(int py=0; py<pixelBlock.h; py++) {

			for(int px=0; px<pixelBlock.w; px++) {

				dstpix[0].r = clut[srcpix[0]&0xf].r<<3;
				dstpix[0].g = clut[srcpix[0]&0xf].g<<3;
				dstpix[0].b = clut[srcpix[0]&0xf].b<<3;
				if ( clut[srcpix[0]&0xf].i ) {
					dstpix[0].a = 255;
				} else {
					if ( ( dstpix[0].r + dstpix[0].g + dstpix[0].b ) == 0 ) {
						dstpix[0].a = 0;
					} else {
						dstpix[0].a = 255;
					}					
				}
				

				dstpix[1].r = clut[srcpix[0]>>4].r<<3;
				dstpix[1].g = clut[srcpix[0]>>4].g<<3;
				dstpix[1].b = clut[srcpix[0]>>4].b<<3;
				
				if ( clut[srcpix[0]>>4].i ) {
					dstpix[1].a = 255;
				} else {
					if ( ( dstpix[1].r + dstpix[1].g + dstpix[1].b ) == 0 ) {
						dstpix[1].a = 0;
					} else {
						dstpix[1].a = 255;
					}
				}
				
				dstpix[2].r = clut[srcpix[1]&0xf].r<<3;
				dstpix[2].g = clut[srcpix[1]&0xf].g<<3;
				dstpix[2].b = clut[srcpix[1]&0xf].b<<3;
				
				if ( clut[srcpix[1]&0xf].i ) {
					dstpix[2].a = 255;
				} else {
					if ( ( dstpix[2].r + dstpix[2].g + dstpix[2].b ) == 0 ) {
						dstpix[2].a = 0;
					} else {
						dstpix[2].a = 255;
					}
				}
				
				dstpix[3].r = clut[srcpix[1]>>4].r<<3;
				dstpix[3].g = clut[srcpix[1]>>4].g<<3;
				dstpix[3].b = clut[srcpix[1]>>4].b<<3;
				
				if ( clut[srcpix[1]>>4].i ) {
					dstpix[3].a = 255;
				} else {
					if ( ( dstpix[3].r + dstpix[3].g + dstpix[3].b ) == 0 ) {
						dstpix[3].a = 0;
					} else {
						dstpix[3].a = 255;
					}
				}
				
				srcpix += 2;
				dstpix += 4;

			}

        }

	} break;

	case 1:		// 8-bit
	{

		unsigned char	*srcpix = (unsigned char*)pixels;
		PIXEL_RGB		*dstpix = (PIXEL_RGB*)output;

        for(int py=0; py<pixelBlock.h; py++) {

			for(int px=0; px<pixelBlock.w; px++) {

				dstpix[0].r = clut[srcpix[0]].r<<3;
				dstpix[0].g = clut[srcpix[0]].g<<3;
				dstpix[0].b = clut[srcpix[0]].b<<3;
				dstpix[0].a = 255;

				dstpix[1].r = clut[srcpix[1]].r<<3;
				dstpix[1].g = clut[srcpix[1]].g<<3;
				dstpix[1].b = clut[srcpix[1]].b<<3;
				dstpix[1].a = 255;

				srcpix += 2;
				dstpix += 2;

			}

        }

	} break;

	case 2:		// 15-bit
		
		unsigned short	*srcpix = (unsigned short*)pixels;
		PIXEL_RGB		*dstpix = (PIXEL_RGB*)output;

        for(int py=0; py<pixelBlock.h; py++) {

			for(int px=0; px<pixelBlock.w; px++) {

				dstpix[0].r = (*srcpix&0x1f)<<3;
				dstpix[0].g = ((*srcpix>>5)&0x1f)<<3;
				dstpix[0].b = ((*srcpix>>10)&0x1f)<<3;
				dstpix[0].a = 255;

				srcpix += 1;
				dstpix += 1;

			}

        }
		
		break;
	}

}

int TimLoaderClass::GetWidth() {

	switch(header.flag.pmode) {
	case 0:
		return pixelBlock.w*4;
	case 1:
		return pixelBlock.w*2;
	case 2:
		return pixelBlock.w;
	}

	return -1;

}

int TimLoaderClass::GetHeight() {

	return pixelBlock.h;

}
