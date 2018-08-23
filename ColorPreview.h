/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colorpreview.h
 * Author: Lameguy64
 *
 * Created on April 11, 2018, 10:25 AM
 */

#ifndef COLORPREVIEW_H
#define COLORPREVIEW_H

#include <Fl/Fl_Group.H>

class ColorPreview : public Fl_Group {
public:
	ColorPreview(int x, int y, int w, int h, const char* l = nullptr);
	virtual ~ColorPreview();
	void draw();
private:
	
};

#endif /* COLORPREVIEW_H */

