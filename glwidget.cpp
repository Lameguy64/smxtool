/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   glwidget.cpp
 * Author: Lameguy64
 * 
 * Created on March 13, 2018, 1:24 PM
 */

#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <Fl/Fl.H>
#include <Fl/fl_message.H>

#include "glwidget.h"
#include "global.h"

#define PI 3.14159265358979f
#define ROTPI PI/180.f

void paintModeToggle_cb(Fl_Button* widget, void* userdata) {
	
	if ( project.last_selected < 0 ) {
		widget->value( 0 );
		return;
	}
	
	if ( widget->value() ) {
		
		project.model.ClearSelection();
		project.last_selected = -1;
		project.active_primitive = &project.temp;
	}
	
	ui->glviewport->redraw();
	
}

void renderModeToggle_cb(Fl_Button* widget, long userdata) {
	
	if ( userdata == 0 ) {
		
		ui->glviewport->wireframe_overlay = widget->value();
		
	} else if ( userdata == 1 ) {
		
		ui->glviewport->backface_culling = widget->value();
		
	}
	
	ui->glviewport->redraw();
	
}

void resetPosition_cb(Fl_Button* widget, void* userdata) {
	
	ui->glviewport->position_x = 0.f;
	ui->glviewport->position_y = 0.f;
	ui->glviewport->position_z = 0.f;
	
	if ( project.model.vertices.size() ) {
		
		for( int i=0; i<project.model.vertices.size(); i++ ) {

			ui->glviewport->position_x += project.model.vertices[i].x;
			ui->glviewport->position_y += project.model.vertices[i].y;
			ui->glviewport->position_z += project.model.vertices[i].z;

		}
	
	}
	
	ui->glviewport->position_x /= (float)project.model.vertices.size();
	ui->glviewport->position_y /= (float)project.model.vertices.size();
	ui->glviewport->position_z /= (float)project.model.vertices.size();
	
	ui->glviewport->redraw();
	
}

void homePosition_cb(Fl_Button* widget, void* userdata) {
	
	ui->glviewport->position_x = 0.f;
	ui->glviewport->position_y = 0.f;
	ui->glviewport->position_z = 0.f;
	ui->glviewport->redraw();
	
}

void perspModeChange_cb(Fl_Button* widget, long userdata) {
	
	if ( userdata == 0 ) {
		
		ui->glviewport->persp_mode = 0;
		
	} else {
		
		ui->glviewport->persp_mode = 1;
		
	}
	
	ui->glviewport->redraw();
	
}

void setActiveFace(int face) {
	
	project.last_selected = face;
	
	if ( project.model.primitives[project.last_selected].primtype 
		& SMX_PRIM_GOURAUD ) {
		
		ui->primTypeRadio[0]->value( 0 );
		ui->primTypeRadio[1]->value( 1 );
		ui->setColor[0]->activate();
		ui->setColor[0]->value( 1 );
		ui->setColor[1]->activate();
		ui->setColor[1]->value( 0 );
		ui->setColor[2]->activate();
		ui->setColor[2]->value( 0 );
		
		if ( project.model.primitives[project.last_selected].primtype 
			& SMX_PRIM_QUAD ) {
			ui->setColor[3]->activate();
		} else {
			ui->setColor[3]->deactivate();
		}
		ui->setColor[3]->value( 0 );
		
	} else {
		
		ui->primTypeRadio[0]->value( 1 );
		ui->primTypeRadio[1]->value( 0 );
		ui->setColor[0]->activate();
		ui->setColor[0]->value( 1 );
		ui->setColor[1]->deactivate();
		ui->setColor[1]->value( 0 );
		ui->setColor[2]->deactivate();
		ui->setColor[2]->value( 0 );
		ui->setColor[3]->deactivate();
		ui->setColor[3]->value( 0 );
		
	}
	
	ui->colorSlider[0]->value( project.model.primitives[project.last_selected].c[0].r );
	ui->colorSlider[1]->value( project.model.primitives[project.last_selected].c[0].g );
	ui->colorSlider[2]->value( project.model.primitives[project.last_selected].c[0].b );

	
	if ( project.model.primitives[project.last_selected].primtype 
		& SMX_PRIM_TEXTURED ) {
		ui->primTexturedToggle->value( 1 );
	} else {
		ui->primTexturedToggle->value( 0 );
	}
				
	
	ui->planeSettings->activate();
	ui->textureSettings->activate();
	
	ui->colorPreviewer->redraw();
	ui->texturePreviewer->redraw();
	ui->glviewport->redraw();
	
	ui->primitiveIndexOutput->value( face );
	
	ui->doubleSidedCheck->value( 
		project.model.primitives[project.last_selected].doublesided );
	
	ui->lightModeRadio[0]->value( 0 );
	ui->lightModeRadio[1]->value( 0 );
	ui->lightModeRadio[2]->value( 0 );
	int i = project.model.primitives[project.last_selected].shadetype;
	ui->lightModeRadio[i]->value( 1 );
	
	ui->blendModeRadio[0]->value( 0 );
	ui->blendModeRadio[1]->value( 0 );
	ui->blendModeRadio[2]->value( 0 );
	ui->blendModeRadio[3]->value( 0 );
	ui->blendModeRadio[4]->value( 0 );
	i = project.model.primitives[project.last_selected].blendmode;
	ui->blendModeRadio[i]->value( 1 );
	
	project.temp = project.model.primitives[project.last_selected];
	project.active_primitive = &project.model.primitives[project.last_selected];
}

void unsetFace() {
	
	project.model.ClearSelection();
	project.last_selected = -1;
	project.active_primitive = &project.temp;
	
	ui->planeSettings->deactivate();
	ui->textureSettings->deactivate();
	ui->glviewport->redraw();
	ui->colorPreviewer->redraw();
	ui->texturePreviewer->redraw();
}

glwidget::glwidget(int x, int y, int w, int h, const char* l)
	: Fl_Gl_Window( x, y, w, h, l ) {
	
	end();
	
	init_done = false;
	position_x = 0.f;
	position_y = 0.f;
	position_z = 0.f;
	view_move = false;
	shift_down = false;
	persp_mode = 0;
	
	zoom = 4.f;
	rotate_x = 0.f;
	rotate_y = 0.f;
	
	backface_culling = true;
	wireframe_overlay = false;
	
}

glwidget::~glwidget() {
}

void glwidget::draw() {
	
	if ( !Fl_Gl_Window::valid() ) {
		
		if ( !init_done ) {	
			
			if ( config.glsl_enable ) {
				
				int err = glewInit();
				if ( err ) {
					fl_message_title( "GLEW Init Failed" );
					switch( err ) {
						case GLEW_ERROR_NO_GL_VERSION:
							fl_message_title( "No OpenGL version available." );
							fl_message( "Check if you have graphics drivers installed." );
							break;
						case GLEW_ERROR_GL_VERSION_10_ONLY:
							fl_message_title( "Only OpenGL 1.0 is available.\n" );
							fl_message( "Your graphics card is too old for GLSL." );
							break;
						case GLEW_ERROR_GLX_VERSION_11_ONLY:
							fl_message_title( "Only OpenGL 1.1 is available." );
							fl_message( "Your graphics card is too old for GLSL." );
							break;
					}
				} else {
	#ifdef DEBUG
					std::cout << "GLEW initialized successfully." << std::endl;
	#endif	
					if ( shader.Initialize() ) {
						
						fl_message_title( "Shader Init Error" );
						fl_message( "Unable to initialize GLSL shader.\n"
							"Falling back to fixed-function rendering." );
						
						config.glsl_enable = false;
					}
				}

			}
			
			init_done = true;
		}
		
		glViewport( 0, 0, w(), h() );
		glEnable( GL_TEXTURE_2D );
		
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		
		glFrontFace( GL_CW );
		glCullFace( GL_BACK );
		
	}
	
	glClearColor( 0.2f, 0.0f, 0.2f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	
	// Setup projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( tan(90.f*PI/360)*atan((float)h()/w())*360/PI, 
		((float)w()/h()), 1, 32768 );
	
	// Setup modelview
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
		
	if ( project.bg_model.vertices.size() ) {
		glPushMatrix();
		glRotatef( rotate_y, 1.f, 0.f, 0.f );
		glRotatef( rotate_x, 0.f, 1.f, 0.f );
		glDisable( GL_DEPTH_TEST );
		project.bg_model.RenderTextured( 1 );
		glPopMatrix();
	}
	
	if ( persp_mode == 0 ) {
		glTranslatef( 0.f, 0.f, -zoom );
		glRotatef( rotate_y, 1.f, 0.f, 0.f );
		glRotatef( rotate_x, 0.f, 1.f, 0.f );
		glTranslatef( -position_x, position_y, position_z );
	} else {
		glRotatef( rotate_y, 1.f, 0.f, 0.f );
		glRotatef( rotate_x, 0.f, 1.f, 0.f );
		glTranslatef( -position_x, position_y, position_z );		
	}
	
	glGetDoublev( GL_PROJECTION_MATRIX, m_projection );
	glGetDoublev( GL_MODELVIEW_MATRIX, m_modelview );
	glGetIntegerv( GL_VIEWPORT, m_view );
	
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	glBindTexture( GL_TEXTURE_2D, 0 );
	
	// Draw center
	glColor4f( 0.5f, 0.f, 0.f, 1.f );
	glBegin( GL_LINES );
	
	glVertex3f( -1.f, 0.f, 0.f );
	glVertex3f( 1.f, 0.f, 0.f );
	glVertex3f( 0.f, 0.f, -1.f );
	glVertex3f( 0.f, 0.f, 1.f );
	
	glEnd();
	
	if ( ui->viewDepthToggle->value() ) {
		glEnable( GL_DEPTH_TEST );
	} else {
		glDisable( GL_DEPTH_TEST );
	}
	
	glPolygonOffset(1.0, 1.0);
	glEnable( GL_POLYGON_OFFSET_FILL );
	
	
	/*pos.x = -( (zoom*sin(rotate_x*ROTPI))*cos(rotate_y*ROTPI) );
	pos.y = -( zoom*sin(rotate_y*ROTPI) );
	pos.z = -( (zoom*cos(rotate_x*ROTPI))*cos(rotate_y*ROTPI) );

	pos.x += position_x;
	pos.y += position_y;
	pos.z += position_z;
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 
		pos.x, 
		-pos.y, 
		-pos.z, 0.0 };
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );*/
	
	project.model.RenderTextured( backface_culling );
	
	/*glDisable( GL_LIGHTING );*/
	
	
	glDisable( GL_POLYGON_OFFSET_FILL );
	
	if ( wireframe_overlay ) {
		glColor4f( 0.f, 0.f, 0.f, 1.f );
		project.model.RenderWireframe();
	}
	
	glDisable( GL_CULL_FACE );
	
	project.model.RenderSelected();
	
}

void glwidget::CalculateClickRay(SmxClass::VECTOR3D* pos, SmxClass::VECTOR3D* ray) {
	
	double ncx,ncy,ncz;
	double fcx,fcy,fcz;

	gluUnProject( Fl::event_x(), h()-Fl::event_y(), 0.f,
		m_modelview, m_projection, m_view, &ncx, &ncy, &ncz );
	gluUnProject( Fl::event_x(), h()-Fl::event_y(), 1.f,
		m_modelview, m_projection, m_view, &fcx, &fcy, &fcz );

	ray->x = ( fcx-ncx );
	ray->y = -( fcy-ncy );
	ray->z = -( fcz-ncz );
	
	// Orbital view position calculation
	if ( persp_mode == 0 ) {
		
		pos->x = -( (zoom*sin(rotate_x*ROTPI))*cos(rotate_y*ROTPI) );
		pos->y = -( zoom*sin(rotate_y*ROTPI) );
		pos->z = -( (zoom*cos(rotate_x*ROTPI))*cos(rotate_y*ROTPI) );

		pos->x += position_x;
		pos->y += position_y;
		pos->z += position_z;
		
	} else {
		
		pos->x = position_x;
		pos->y = position_y;
		pos->z = position_z;
		
	}
	
}

int glwidget::handle(int event) {
	
	SmxClass::PRIMITIVE *p = project.active_primitive;
	
	switch( event ) {
	case FL_PUSH:
		
		if ( Fl::event_clicks() ) {
			
			if ( ( p != nullptr ) && ( Fl::event_button1() ) ) {
				if ( ( p->primtype == SmxClass::PRIM_TRI_F ) || 
					( p->primtype == SmxClass::PRIM_TRI_FT ) || 
					( p->primtype == SmxClass::PRIM_TRI_G ) || 
					( p->primtype == SmxClass::PRIM_TRI_GT ) ) {

					position_x = (project.model.vertices[p->v[0]].x
						+project.model.vertices[p->v[1]].x
						+project.model.vertices[p->v[2]].x)/3;
					position_y = (project.model.vertices[p->v[0]].y
						+project.model.vertices[p->v[1]].y
						+project.model.vertices[p->v[2]].y)/3;
					position_z = (project.model.vertices[p->v[0]].z
						+project.model.vertices[p->v[1]].z
						+project.model.vertices[p->v[2]].z)/3;

				} else if ( ( p->primtype == SmxClass::PRIM_QUAD_F ) || 
					( p->primtype == SmxClass::PRIM_QUAD_FT ) || 
					( p->primtype == SmxClass::PRIM_QUAD_G ) || 
					( p->primtype == SmxClass::PRIM_QUAD_GT ) ) {

					position_x = (project.model.vertices[p->v[0]].x
						+project.model.vertices[p->v[1]].x
						+project.model.vertices[p->v[2]].x
						+project.model.vertices[p->v[3]].x)/4;
					position_y = (project.model.vertices[p->v[0]].y
						+project.model.vertices[p->v[1]].y
						+project.model.vertices[p->v[2]].y
						+project.model.vertices[p->v[3]].y)/4;
					position_z = (project.model.vertices[p->v[0]].z
						+project.model.vertices[p->v[1]].z
						+project.model.vertices[p->v[2]].z
						+project.model.vertices[p->v[3]].z)/4;

				}
			
				redraw();
			}
			
		} else {
			
			if ( Fl::event_button() == FL_LEFT_MOUSE ) {
			CalculateClickRay( &pos, &ray );

			int face = project.model.ScanFace( pos, ray );
			
			if ( face >= 0 ) {

				if ( ui->paintModeToggle->value() == 0 ) {
					
					if ( !shift_down ) {
						project.model.ClearSelection();
					}
					
					project.model.primitives[face].selected = true;
					setActiveFace( face );
					
				} else {
					
					SmxClass::PRIMITIVE temp = project.model.primitives[face];
					project.temp.selected = false;
					
					if ( ( project.temp.primtype & SMX_PRIM_QUAD ) 
						== ( project.model.primitives[face].primtype & SMX_PRIM_QUAD ) ) {
						
						project.model.primitives[face] = project.temp;

						project.model.primitives[face].v[0] = temp.v[0];
						project.model.primitives[face].v[1] = temp.v[1];
						project.model.primitives[face].v[2] = temp.v[2];
						project.model.primitives[face].v[3] = temp.v[3];

						project.model.primitives[face].n[0] = temp.n[0];
						project.model.primitives[face].n[1] = temp.n[1];
						project.model.primitives[face].n[2] = temp.n[2];
						project.model.primitives[face].n[3] = temp.n[3];
					
					}
					
					redraw();
					
					return 1;
				}
				
			} else {

				if ( ui->paintModeToggle->value() == 0 ) {
					unsetFace();
				}

			}
							
			} else if ( Fl::event_button() == FL_RIGHT_MOUSE ) {

				clickx = Fl::event_x();
				clicky = Fl::event_y();

				if ( shift_down ) {
					clickrx = position_x;
					clickry = position_y;
					clickrz = position_z;
					view_move = 2;
				} else {
					clickrx = rotate_x;
					clickry = rotate_y;
					view_move = 1;
				}

				return 1;

			}
			
		}
		break;
	case FL_SHORTCUT:
		if ( ( Fl::event_key() == FL_Shift_L ) || 
			( Fl::event_key() == FL_Shift_R ) ) {
			
			shift_down = true;
		}
		break;	
	case FL_KEYUP:
		if ( ( Fl::event_key() == FL_Shift_L ) || 
			( Fl::event_key() == FL_Shift_R ) ) {
			
			shift_down = false;
		}
		break;	
	case FL_DRAG:
			
		if ( ( ui->paintModeToggle->value() ) && ( Fl::event_button1() ) ) {

			CalculateClickRay( &pos, &ray );
					
			int face = project.model.ScanFace( pos, ray );

			if ( ( face >= 0 ) && ( project.temp.primtype & SMX_PRIM_QUAD ) 
				== ( project.model.primitives[face].primtype & SMX_PRIM_QUAD ) ) {
				
				SmxClass::PRIMITIVE temp = project.model.primitives[face];
				project.temp.selected = false;
				temp.selected = false;

				project.model.primitives[face] = project.temp;

				project.model.primitives[face].v[0] = temp.v[0];
				project.model.primitives[face].v[1] = temp.v[1];
				project.model.primitives[face].v[2] = temp.v[2];
				project.model.primitives[face].v[3] = temp.v[3];

				project.model.primitives[face].n[0] = temp.n[0];
				project.model.primitives[face].n[1] = temp.n[1];
				project.model.primitives[face].n[2] = temp.n[2];
				project.model.primitives[face].n[3] = temp.n[3];

				redraw();

			}
			
		}
		
		if ( view_move == 1 ) {
			rotate_x = clickrx+((Fl::event_x()-clickx)/4.f);
			rotate_y = clickry+((Fl::event_y()-clicky)/4.f);
			redraw();
		} else if ( view_move == 2 ) {
			position_x = clickrx-((Fl::event_x()-clickx)/20.f)*cos(rotate_x*ROTPI)
				+(((Fl::event_y()-clicky)/20.f)*sin(rotate_y*ROTPI)*sin(rotate_x*ROTPI));
			position_y = clickry-((Fl::event_y()-clicky)/20.f)*cos(rotate_y*ROTPI);
			position_z = clickrz+((Fl::event_x()-clickx)/20.f)*sin(rotate_x*ROTPI)
				+(((Fl::event_y()-clicky)/20.f)*sin(rotate_y*ROTPI)*cos(rotate_x*ROTPI));
			redraw();
		}
		break;
	case FL_MOUSEWHEEL:
		if ( persp_mode == 0 ) {
			zoom += Fl::event_dy();
		} else {
			position_x -= ((Fl::event_dy()/2.f)*sin(rotate_x*ROTPI))*cos(rotate_y*ROTPI);
			position_y -= (Fl::event_dy()/2.f)*sin(rotate_y*ROTPI);
			position_z -= ((Fl::event_dy()/2.f)*cos(rotate_x*ROTPI))*cos(rotate_y*ROTPI);
		}
		redraw();
		break;
	case FL_RELEASE:
		view_move = 0;
		break;
	}
	
	return Fl_Gl_Window::handle( event );
}