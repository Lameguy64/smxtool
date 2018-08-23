/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   texturepreview.cpp
 * Author: lameguy64
 * 
 * Created on April 17, 2018, 4:20 PM
 */

#include <Fl/fl_draw.H>
#include <Fl/Fl_Scrollbar.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <iostream>
#include "texturepreview.h"
#include "TextureClass.h"
#include "global.h"

int inbox(int x, int y, int bx, int by, int bw, int bh) {
	
	if ( ( x >= bx ) && ( x <= bx+(bw-1) ) ) {
		if ( ( y >= by ) && ( y <= by+(bh-1) ) ) {
			return 1;
		}
	}
	
	return 0;
	
}

int pointInPolygon(int x, int y, VECTOR2D* points, int num_points ) {

	int c = 0;
	int i,j;
	
	for( i = 0, j = num_points - 1; i < num_points; j = i++) {
		if ( ( (points[i].y >= y ) != (points[j].y >= y) )
			&& (x <= (points[j].x - points[i].x) 
			* (y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
		) {
		  c = !c;
		}
	}
	
	return c;
}

void expandTexButton_cb(Fl_Button* widget, void* userdata) {
	
	for( int i=0; i<project.model.primitives.size(); i++ ) {
		
		if ( project.model.primitives[i].selected ) {
			
			if ( project.model.IsTextured( 
				&project.model.primitives[i] ) ) {
				
				TexImageClass* tex 
					= (TexImageClass*)project.model.textures[ 
						project.model.primitives[i].texnum ]->userdata2;
				
				if ( tex != nullptr ) {

					project.model.primitives[i].t[0].u = 0;
					project.model.primitives[i].t[0].v = 0;

					project.model.primitives[i].t[1].u = tex->w-1;
					project.model.primitives[i].t[1].v = 0;

					project.model.primitives[i].t[2].u = 0;
					project.model.primitives[i].t[2].v = tex->h-1;

					if ( ( project.model.primitives[i].primtype 
						== SmxClass::PRIM_QUAD_FT ) ||
						( project.model.primitives[i].primtype 
						== SmxClass::PRIM_QUAD_GT ) ) {

						project.model.primitives[i].t[3].u = tex->w-1;
						project.model.primitives[i].t[3].v = tex->h-1;

					}
					
				}
			}
			
		}
		
	}
	
	ui->glviewport->redraw();
	ui->texturePreviewer->redraw();
	
}

void textureBrowser_cb(Fl_Browser* widget, void* userdata) {

	if ( ( project.last_selected < 0 ) || ( widget->value() == 0 ) ) {
		return;
	}
	
	if ( !project.model.IsTextured( &project.model.primitives[project.last_selected] ) ) {
		return;
	}
	
	int texnum = widget->value()-1;
	for( int i=0; i<project.model.primitives.size(); i++ ) {
		
		if ( project.model.primitives[i].selected ) {
			project.model.primitives[i].texnum = texnum;
		}
		
	}
	
	project.temp = project.model.primitives[project.last_selected];
	
	ui->texturePreviewer->redraw();
	ui->glviewport->redraw();
	
}

void rotateTexButton_cb(Fl_Button* widget, long userdata) {
	
	if ( project.last_selected < 0 ) {
		return;
	}
	
	SmxClass::PRIMITIVE *p = &project.model.primitives[project.last_selected];
	
	if ( userdata == 0 ) {
		
		if ( ( p->primtype == SmxClass::PRIM_TRI_FT ) ||
			( p->primtype == SmxClass::PRIM_TRI_GT ) ) {
			
			SmxClass::UV t;
			
			t = p->t[0];
			p->t[0] = p->t[1];
			p->t[1] = p->t[2];
			p->t[2] = t;
			
			ui->glviewport->redraw();
			
		} else if ( ( p->primtype == SmxClass::PRIM_QUAD_FT ) ||
			( p->primtype == SmxClass::PRIM_QUAD_GT ) ) {
			
			SmxClass::UV t;
			
			t = p->t[0];
			p->t[0] = p->t[1];
			p->t[1] = p->t[3];
			p->t[3] = p->t[2];
			p->t[2] = t;
			
			ui->glviewport->redraw();
			
		}
		
	} else {
		
		if ( ( p->primtype == SmxClass::PRIM_TRI_FT ) ||
			( p->primtype == SmxClass::PRIM_TRI_GT ) ) {
			
			SmxClass::UV t;
			
			t = p->t[2];
			p->t[2] = p->t[1];
			p->t[1] = p->t[0];
			p->t[0] = t;
			
			ui->glviewport->redraw();
			
		} else if ( ( p->primtype == SmxClass::PRIM_QUAD_FT ) ||
			( p->primtype == SmxClass::PRIM_QUAD_GT ) ) {
			
			SmxClass::UV t;
			
			t = p->t[2];
			p->t[2] = p->t[3];
			p->t[3] = p->t[1];
			p->t[1] = p->t[0];
			p->t[0] = t;
			
			ui->glviewport->redraw();
			
		}
		
	}
	
	project.temp = project.model.primitives[project.last_selected];
}

void textureZoom_cb(Fl_Spinner* widget, void* userdata) {
	
	ui->texturePreviewer->SetZoom( widget->value() );
	
}

static void scroll_cb(Fl_Widget* widget, void* userdata) {
	
	ui->texturePreviewer->redraw();
	
}

static void draw_texture(void* userdata, int feed_X, int feed_Y, 
	int feed_W, uchar* out_buf)
{
	TexturePreview* widget = (TexturePreview*)userdata;
	SmxClass::RGB* p = (SmxClass::RGB*)out_buf;
	
	TexImageClass* tex 
		= (TexImageClass*)project.model.textures
		[project.model.primitives[project.last_selected].texnum]->userdata2;
	
	if ( tex == nullptr ) {
		
		for( int i=feed_X; i<feed_X+feed_W; i++ ) {
			p[i-feed_X].r = (Fl::get_color(widget->color())>>8)&0xff;
			p[i-feed_X].g = (Fl::get_color(widget->color())>>16)&0xff;
			p[i-feed_X].b = (Fl::get_color(widget->color())>>24);
		}
		
		return;
	}
	
	if ( feed_Y+widget->_vscrollbar.value() >= tex->h*widget->zoom ) {
		
		for( int i=feed_X; i<feed_X+feed_W; i++ ) {
			p[i-feed_X].r = (Fl::get_color(widget->color())>>8)&0xff;
			p[i-feed_X].g = (Fl::get_color(widget->color())>>16)&0xff;
			p[i-feed_X].b = (Fl::get_color(widget->color())>>24);
		}
		
		return;
	}
	
	for( int i=feed_X; i<feed_X+feed_W; i++ ) {
		
		unsigned int pix = tex->ReadPixel( 
			(i+widget->_hscrollbar.value())/widget->zoom, 
			(feed_Y+widget->_vscrollbar.value())/widget->zoom );
		
		if ( ( i < tex->w*widget->zoom ) && ( ALPHA(pix) > 127 ) ) {
			
			p[i-feed_X].r = RED( pix );
			p[i-feed_X].g = GREEN( pix );
			p[i-feed_X].b = BLUE( pix );
			
		} else {
			
			p[i-feed_X].r = (Fl::get_color(widget->color())>>8)&0xff;
			p[i-feed_X].g = (Fl::get_color(widget->color())>>16)&0xff;
			p[i-feed_X].b = (Fl::get_color(widget->color())>>24);
			
		}
		
	}
	
}

TexturePreview::TexturePreview(int x, int y, int w, int h, const char* l) 
	: Fl_Group( x, y, w, h, l ), _hscrollbar( 0, 0, 0, 0 ), _vscrollbar( 0, 0, 0, 0 ) {
	
	zoom = 1;
	
	//_hscrollbar = new Fl_Scrollbar( 0, 0, 0, 0 );
	//_hscrollbar->hide();
	
	//_vscrollbar = new Fl_Scrollbar( 0, 0, 30, 30 );
	//_vscrollbar->hide();
	
	_hscrollbar.type( FL_HORIZONTAL );
	_hscrollbar.hide();
	_hscrollbar.callback( scroll_cb );
	_vscrollbar.hide();
	_vscrollbar.callback( scroll_cb );
	
	calculateScrollbarPos();
			
	_isdragging = false;
	
}

TexturePreview::~TexturePreview() {
}

void TexturePreview::SetZoom(int scale) {
	
	zoom = scale;
	
	testScrollbars();
	redraw();
	
}

int TexturePreview::handle(int event) {

	VECTOR2D temp[4];
	int ret;
	
	if ( project.last_selected < 0 ) {
		return Fl_Group::handle( event );
	}
	
	int points = 0;
	SmxClass::PRIMITIVE *p = &project.model.primitives[project.last_selected];
	
	if ( ( p->primtype == SmxClass::PRIM_TRI_FT ) || 
		( p->primtype == SmxClass::PRIM_TRI_GT ) ) {
		
		points = 3;
		
	} else if ( ( p->primtype == SmxClass::PRIM_QUAD_FT ) || 
		( p->primtype == SmxClass::PRIM_QUAD_GT ) ) {
		
		points = 4;
		
	}
	
	if ( points == 0 ) {
		return Fl_Group::handle( event );
	}
	
	ret = Fl_Group::handle( event );
	
	if ( ret ) {
		return ret;
	}
	
	switch( event ) {
		case FL_PUSH:
			
			for( int i=0; i<points; i++ ) {
				
				if ( inbox( Fl::event_x(), Fl::event_y(),
					(1+x()+(p->t[i].u*zoom)+(zoom/2)-3)-_hscrollbar.value(), 
					(1+y()+(p->t[i].v*zoom)+(zoom/2)-3)-_vscrollbar.value(),
					8, 8 ) ) {
					
					_dragx = Fl::event_x();
					_dragy = Fl::event_y();
					_isdragging = 1;
					_drag = i;
					
					return 1;
					
				}
				
			}
			
			if ( points == 3 ) {
			
				temp[0].x = (x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[0].y = (y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value();
				temp[1].x = (x()+(p->t[1].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[1].y = (y()+(p->t[1].v*zoom)+(zoom/2))-_vscrollbar.value();
				temp[2].x = (x()+(p->t[2].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[2].y = (y()+(p->t[2].v*zoom)+(zoom/2))-_vscrollbar.value();
			
			} else if ( points == 4 ) {
				
				temp[0].x = (x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[0].y = (y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value();
				temp[1].x = (x()+(p->t[1].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[1].y = (y()+(p->t[1].v*zoom)+(zoom/2))-_vscrollbar.value();
				temp[2].x = (x()+(p->t[3].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[2].y = (y()+(p->t[3].v*zoom)+(zoom/2))-_vscrollbar.value();
				temp[3].x = (x()+(p->t[2].u*zoom)+(zoom/2))-_hscrollbar.value();
				temp[3].y = (y()+(p->t[2].v*zoom)+(zoom/2))-_vscrollbar.value();
				
			}
			
			if ( pointInPolygon( Fl::event_x(), Fl::event_y(), temp, points ) ) {
				
				for( int i=0; i<points; i++ ) {
					_draguv[i].x = p->t[i].u;
					_draguv[i].y = p->t[i].v;
				}
				
				_dragx = Fl::event_x();
				_dragy = Fl::event_y();
				_dragpoints = points;
				
				_isdragging = 2;
				
				return 1;
			}
			
			break;
		case FL_DRAG:
			
			if ( _isdragging == 1 ) {
				
				temp[0].x = (((Fl::event_x()-(x()+1))+_hscrollbar.value())/zoom);
				temp[0].y = (((Fl::event_y()-(y()+1))+_vscrollbar.value())/zoom);
				
				if ( temp[0].x < 0 ) {
					temp[0].x = 0;
				}
				if ( temp[0].y < 0 ) {
					temp[0].y = 0;
				}
				
				if ( temp[0].x > 255 ) {
					temp[0].x = 255;
				}
				if ( temp[0].y > 255 ) {
					temp[0].y = 255;
				}
				
				p->t[_drag].u = temp[0].x;
				p->t[_drag].v = temp[0].y;
				
				for( int i=0; i<project.model.primitives.size(); i++ ) {
					
					if ( ( project.model.primitives[i].selected ) &&
						( project.model.primitives[i].primtype == p->primtype ) ) {
						
						project.model.primitives[i].t[_drag] = p->t[_drag];
						
					}
					
				}
				
				ui->glviewport->redraw();
				redraw();
				
			} else if ( _isdragging == 2 ) {
				
				int pu,pv;
				pu = (Fl::event_x()-_dragx)/zoom;
				pv = (Fl::event_y()-_dragy)/zoom;
				
				for( int i=0; i<_dragpoints; i++ ) {
					
					int tempx,tempy;
					
					tempx = _draguv[i].x+pu;
					tempy = _draguv[i].y+pv;
					
					if ( tempx < 0 ) {
						tempx = 0;
					}
					if ( tempy < 0 ) {
						tempy = 0;
					}
					if ( tempx > 255 ) {
						tempx = 255;
					}
					if ( tempy > 255 ) {
						tempy = 255;
					}
					
					p->t[i].u = tempx;
					p->t[i].v = tempy;
				}
				
				ui->glviewport->redraw();
				redraw();
				
			}
			
			break;
		case FL_RELEASE:
			
			if ( _isdragging ) {
				
				_isdragging = false;
				
			}
			
			break;
	}
	
	return Fl_Group::handle( event );
	
}

void TexturePreview::draw() {
	
	testScrollbars();
	calculateScrollbarPos();
	
	int max_w = w()-2;
	int max_h = h()-2;
	
	if ( _vscrollbar.visible() ) {
		max_w -= Fl::scrollbar_size();
	}
	
	if ( _hscrollbar.visible() ) {
		max_h -= Fl::scrollbar_size();
	}
	
	draw_box( box(), x(), y(), w(), h(), color() );
	
	fl_color( FL_BLACK );
	
	if ( project.last_selected >= 0 ) {
		
		if ( project.model.IsTextured( &project.model.primitives[project.last_selected] ) ) {
		
			SmxClass::PRIMITIVE *p 
				= &project.model.primitives[project.last_selected];
			
			fl_push_clip( x()+1, y()+1, max_w, max_h );

			fl_draw_image( draw_texture, this, x()+1, y()+1, 
				max_w, max_h, 3 );
			
			int c;
			
			if ( ( project.model.primitives[project.last_selected].primtype 
				== SmxClass::PRIM_QUAD_FT ) ||
				( project.model.primitives[project.last_selected].primtype 
				== SmxClass::PRIM_QUAD_GT ) ) {
				
				c = 4;
				
				fl_color( FL_MAGENTA );
				
				fl_line( 
					(1+x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[1].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[1].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[3].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[3].v*zoom)+(zoom/2))-_vscrollbar.value() );
				
				fl_line( 
					(1+x()+(p->t[3].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[3].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[2].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[2].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value() );
				
			} else {
				
				c = 3;
				
				fl_color( FL_MAGENTA );
				
				fl_line( 
					(1+x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[1].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[1].v*zoom)+(zoom/2))-_vscrollbar.value(),
					(1+x()+(p->t[2].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[2].v*zoom)+(zoom/2))-_vscrollbar.value() );
				
				fl_line(
					(1+x()+(p->t[2].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[2].v*zoom)+(zoom/2))-_vscrollbar.value(), 
					(1+x()+(p->t[0].u*zoom)+(zoom/2))-_hscrollbar.value(), 
					(1+y()+(p->t[0].v*zoom)+(zoom/2))-_vscrollbar.value() );
				
			}
			
			for( int i=0; i<c; i++ ) {
				
				fl_draw_box( FL_BORDER_FRAME, 
					(1+x()+(p->t[i].u*zoom)+(zoom/2)-3)-_hscrollbar.value(), 
					(1+y()+(p->t[i].v*zoom)+(zoom/2)-3)-_vscrollbar.value(), 
					8, 8, FL_MAGENTA );
				
			}
			
			fl_pop_clip();
			
		}
		
	}
	
	draw_child( _hscrollbar );
	draw_child( _vscrollbar );
	
}

void TexturePreview::resize(int x, int y, int w, int h) {
	
	Fl_Group::resize( x, y, w, h );
	calculateScrollbarPos();
	
}

void TexturePreview::calculateScrollbarPos() {

	_hscrollbar.resize( Fl_Group::x()+1, Fl_Group::y()+Fl_Group::h()-Fl::scrollbar_size()-1, 
		Fl_Group::w()-2, Fl::scrollbar_size() );
	
	if ( _hscrollbar.visible() ) {
		_vscrollbar.resize( Fl_Group::x()+Fl_Group::w()-Fl::scrollbar_size()-1, 
			Fl_Group::y()+1, Fl::scrollbar_size(), (Fl_Group::h()-2)-Fl::scrollbar_size() );
	} else {
		_vscrollbar.resize( Fl_Group::x()+Fl_Group::w()-Fl::scrollbar_size()-1, 
			Fl_Group::y()+1, Fl::scrollbar_size(), Fl_Group::h()-2 );
	}
	
}

void TexturePreview::testScrollbars() {
	
	int max_w = w()-2;
	int max_h = h()-2;
	
	if ( _vscrollbar.visible() ) {
		max_w -= Fl::scrollbar_size();
	}
	
	if ( _hscrollbar.visible() ) {
		max_h -= Fl::scrollbar_size();
	}
	
	if ( project.last_selected >= 0 ) {
		
		if ( project.model.IsTextured( 
			&project.model.primitives[project.last_selected] ) ) {
			
			TexImageClass* tex 
				= (TexImageClass*)project.model.textures
				[project.model.primitives[project.last_selected].texnum]->userdata2;

			if ( tex == nullptr ) {
				return;
			}
			
			if ( ( tex->w*zoom ) > max_w ) {

				_hscrollbar.minimum( 0 );
				_hscrollbar.value( _hscrollbar.value(), max_w, 0, tex->w*zoom );
				_hscrollbar.show();
				
			} else {

				_hscrollbar.value( 0 );
				_hscrollbar.hide();

			}
			
			if ( ( tex->h*zoom ) > max_h ) {

				_vscrollbar.minimum( 0 );
				_vscrollbar.value( _vscrollbar.value(), max_h, 0, tex->h*zoom );
				_vscrollbar.show();
				
			} else {

				_vscrollbar.value( 0 );
				_vscrollbar.hide();

			}
		}
	}
	
}