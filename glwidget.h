/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   glwidget.h
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 1:24 PM
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include "common/smx.h"
#include "ShaderClass.h"

class glwidget : public Fl_Gl_Window {
public:
	glwidget(int x, int y, int w, int h, const char* l = nullptr);
	virtual ~glwidget();
	
	void CalculateClickRay(SmxClass::VECTOR3D* pos, SmxClass::VECTOR3D* ray);
	
	void draw();
	int handle(int event);
	
	int persp_mode;
	
	float zoom;
	float position_x,position_y,position_z;
	float rotate_x,rotate_y;
	
	SmxClass::VECTOR3D pos,ray;
	ShaderClass shader;
	
	int backface_culling;
	int wireframe_overlay;
	
private:
	
	GLdouble 	m_projection[16];
	GLdouble 	m_modelview[16];
	GLint		m_view[4];
	
	int		clickx,clicky;
	float	clickrx,clickry,clickrz;
	int		view_move;
	
	int		init_done;
	int		shift_down;
	
};

#endif /* GLWIDGET_H */

