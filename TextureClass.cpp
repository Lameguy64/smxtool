/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TextureClass.cpp
 * Author: Lameguy64
 * 
 * Created on March 13, 2018, 2:28 PM
 */

#include <iostream>
#include "TextureClass.h"
#include "common/smx.h"

TexImageClass::TexImageClass() {
	gl_texture = 0;
	pixels = nullptr;
}

TexImageClass::~TexImageClass() {
	if ( gl_texture != 0 ) {
		glDeleteTextures( 1, &gl_texture );
	}
	if ( pixels != nullptr ) {
		free( pixels );
	}

#ifdef DEBUG
	std::cout << "Freed texture " << file << std::endl;
#endif
}

int TexImageClass::LoadTexture(const char* file_name) {
	TimLoaderClass tim;
	
    switch( tim.LoadFile( file_name ) )
    {
		case TIM_ERROR_NOTFOUND:
			return -1;
		case TIM_ERROR_INVALID:
			return -1;
		default:
			break;
    };
	
	file = file_name;

	//tim_header	= tim.header;
	//tim_pixels	= tim.pixelBlock;
	//tim_clut	= tim.clutBlock;
	
    w = tim.GetWidth();
    h = tim.GetHeight();

	/*tyoffs = tim.pixelBlock.y%256;

	switch(tim.header.flag.pmode) {
	case 0:	// 4-bit
		txoffs = (tim.pixelBlock.x*4)%256;
		break;
	case 1: // 8-bit
		txoffs = (tim.pixelBlock.x*2)%128;
		break;
	case 2: // 16-bit
		txoffs = tim.pixelBlock.x%64;
		break;
	}*/
	
    pixels = malloc( 4*(w*h) );
    tim.ConvertToRGBA( pixels );

    glGenTextures( 1, &gl_texture );
    glBindTexture( GL_TEXTURE_2D, gl_texture );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, pixels );
	
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	return 0;
}

unsigned int TexImageClass::ReadPixel(int x, int y) {
	
	if ( x < 0 )
		return 0;
	if ( y < 0 )
		return 0;
	if ( x >= w )
		return 0;
	if ( y >= h )
		return 0;
	
	return ((unsigned int*)pixels)[x+(w*y)];
	
}