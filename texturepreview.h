/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   texturepreview.h
 * Author: lameguy64
 *
 * Created on April 17, 2018, 4:20 PM
 */

#ifndef TEXTUREPREVIEW_H
#define TEXTUREPREVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>

typedef struct {
	int x,y;
} VECTOR2D;

class TexturePreview : public Fl_Group {
public:
	
	TexturePreview(int x, int y, int w, int h, const char* l = nullptr);
	virtual ~TexturePreview();
	void SetZoom(int scale);
	void draw();
	int handle(int event);
	void resize(int x, int y, int w, int h);

	
	int zoom;
	
	Fl_Scrollbar _hscrollbar;
	Fl_Scrollbar _vscrollbar;
	
private:
	int _dragx,_dragy;
	int _drag,_isdragging;
	int _dragpoints;
	VECTOR2D _draguv[4];
	
	void calculateScrollbarPos();
	void testScrollbars();
};

#endif /* TEXTUREPREVIEW_H */

