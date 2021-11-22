/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TextureClass.h
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 2:28 PM
 */

#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <string>
#include <FL/gl.h>
#include "common/timloader.h"

#define RED(c) (c&0xff)
#define GREEN(c) ((c>>8)&0xff)
#define BLUE(c) ((c>>16)&0xff)
#define ALPHA(c) (c>>24)

class TexImageClass {
public:
	TexImageClass();
	virtual ~TexImageClass();
	
	int LoadTexture(const char* file_name);
	unsigned int ReadPixel(int x, int y);
	
	GLuint gl_texture;
	void* pixels;
	int w,h;
	
	int index;
	
	std::string file;
		
private:
};

#endif /* TEXTURECLASS_H */

