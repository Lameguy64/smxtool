/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colorpreview.cpp
 * Author: Lameguy64
 * 
 * Created on April 11, 2018, 10:25 AM
 */

#include <math.h>
#include <iostream>
#include <FL/fl_draw.H>
#include <FL/Fl_Color_Chooser.H>
#include "ColorPreview.h"
#include "global.h"

void planeSettingChange_cb(Fl_Check_Button* widget, long userdata ) {
	
	if ( project.active_primitive == nullptr ) {
		return;
	}
	
	if ( userdata == 0 ) {
		
		project.active_primitive->doublesided = widget->value();
		
		for ( int i=0; i<project.model.primitives.size(); i++ ) {
			if ( project.model.primitives[i].selected ) {
				project.model.primitives[i].doublesided = widget->value();
			}
		}
		
		if ( ui->paintModeToggle->value() ) {
			project.temp = project.model.primitives[project.last_selected];
		}
		
		ui->glviewport->redraw();
		
	}
	
}

void blendModeSet_cb(Fl_Round_Button* widget, long userdata) {
	
	if ( project.active_primitive == nullptr ) {
		return;
	}
	
	project.active_primitive->blendmode = userdata;
	
	for ( int i=0; i<project.model.primitives.size(); i++ ) {
		if ( project.model.primitives[i].selected ) {
			project.model.primitives[i].blendmode = userdata;
		}
	}
	
	project.temp = project.model.primitives[project.last_selected];
	ui->glviewport->redraw();
	
}

void setPrimTexture_cb(Fl_Check_Button* widget, long userdata) {
	
	if ( ui->primTexturedToggle->value() ) {
		
		for( int i=0; i<project.model.primitives.size(); i++ ) {
			if ( project.model.primitives[i].selected ) {
				project.model.SetTexturing( &project.model.primitives[i], 1, 0 );
			}
		}
		
		ui->texturePreviewer->redraw();
		ui->glviewport->redraw();
		
	} else {
		
		for( int i=0; i<project.model.primitives.size(); i++ ) {
			if ( project.model.primitives[i].selected ) {
				project.model.SetTexturing( &project.model.primitives[i], 0, 0 );
			}
		}
		
		ui->texturePreviewer->redraw();
		ui->glviewport->redraw();
		
	}
	
}

void setPrimType_cb(Fl_Round_Button* widget, long userdata) {
	
	if ( userdata ) {
		
		for( int i=0; i<project.model.primitives.size(); i++ ) {
			if ( project.model.primitives[i].selected ) {
				project.model.SetShading( &project.model.primitives[i], 1 );
			}
		}
		
		ui->setColor[1]->activate();
		ui->setColor[2]->activate();
		ui->setColor[3]->activate();
		
	} else {
		
		for( int i=0; i<project.model.primitives.size(); i++ ) {
			if ( project.model.primitives[i].selected ) {
				project.model.SetShading( &project.model.primitives[i], 0 );
			}
		}
		
		ui->setColor[1]->deactivate();
		ui->setColor[2]->deactivate();
		ui->setColor[3]->deactivate();
		
	}
	
	
	if ( ui->paintModeToggle->value() == 0 ) {
		
		project.temp = project.model.primitives[project.last_selected];
		
	}
	
	ui->glviewport->redraw();
	ui->colorPreviewer->redraw();
}

void colorSlider_cb(Fl_Value_Slider* widget, long userdata) {
	
	if ( project.active_primitive == nullptr ) {
		return;
	}
	
	int index=0,is_flat=false;
	
	if ( ui->setColor[1]->value() ) {
		index = 1;
	} else if ( ui->setColor[2]->value() ) {
		index = 2;
	} else if ( ui->setColor[3]->value() ) {
		index = 3;
	}
	
	project.active_primitive->c[index].r = ui->colorSlider[0]->value();
	project.active_primitive->c[index].g = ui->colorSlider[1]->value();
	project.active_primitive->c[index].b = ui->colorSlider[2]->value();
	
	if ( index == 0 ) {
		
		switch( project.active_primitive->primtype ) {
			case SmxClass::PRIM_TRI_F:
			case SmxClass::PRIM_TRI_FT:
			case SmxClass::PRIM_QUAD_F:
			case SmxClass::PRIM_QUAD_FT:
				project.active_primitive->c[1] 
					= project.active_primitive->c[index];
				project.active_primitive->c[2] 
					= project.active_primitive->c[index];
				project.active_primitive->c[3] 
					= project.active_primitive->c[index];
				is_flat = true;
				break;
		}
		
	}
	
	for ( int i=0; i<project.model.primitives.size(); i++ ) {
		
		if ( i == project.last_selected )
			continue;
		
		if ( project.model.primitives[i].selected ) {
			
			if ( ( ( project.model.primitives[i].primtype & SMX_PRIM_GOURAUD )
				== ( project.active_primitive->primtype & SMX_PRIM_GOURAUD ) )
				&& ( ( project.model.primitives[i].primtype & SMX_PRIM_QUAD )
				== ( project.active_primitive->primtype & SMX_PRIM_QUAD ) ) ) {
			
				project.model.primitives[i].c[index]
					= project.active_primitive->c[index];

				if ( is_flat ) {
					project.model.primitives[i].c[1] 
						= project.active_primitive->c[0];
					project.model.primitives[i].c[2] 
						= project.active_primitive->c[0];
					project.model.primitives[i].c[3] 
						= project.active_primitive->c[0];
				}
				
			}
		}
		
	}
	
	if ( ui->paintModeToggle->value() == 0 ) {
		
		project.temp = project.model.primitives[project.last_selected];
		
	}
	
	ui->colorPreviewer->redraw();
	ui->glviewport->redraw();
	
}

void setVcolor_cb(Fl_Button* widget, long userdata) {
	
	if ( ( userdata == 0 ) || ( userdata == 2 ) ) {
		ui->setColor[1]->value( 0 );
		ui->setColor[3]->value( 0 );
	} else if ( ( userdata == 1 ) || ( userdata == 3 ) ) {
		ui->setColor[0]->value( 0 );
		ui->setColor[2]->value( 0 );
	}
	
	ui->colorSlider[0]->value(  project.active_primitive->c[userdata].r );
	ui->colorSlider[1]->value(  project.active_primitive->c[userdata].g );
	ui->colorSlider[2]->value(  project.active_primitive->c[userdata].b );
	
	if ( ui->paintModeToggle->value() == 0 ) {
		project.temp = project.model.primitives[project.last_selected];
	}
}

static void draw_shaded_triangle(void* userdata, int feed_X, int feed_Y, 
	int feed_W, uchar* buf) {
	
	PIXEL* p = (PIXEL*)buf;
	ColorPreview* widget = (ColorPreview*)userdata;
	
	SmxClass::RGB rgb[3] = {
		project.active_primitive->c[0],
		project.active_primitive->c[1],
		project.active_primitive->c[2]
	};
	
	int ww = widget->w()-8;
	int wh = widget->h()-8;
	
	int tw = floor( ww*(1.f-(feed_Y/(float)wh)) );
			
	for ( int xx=feed_X; xx<feed_X+feed_W; xx++ ) {

		if ( xx < tw ) {
			
			p[xx-feed_X].r = (((rgb[0].r*(1.f-(xx/(float)ww)))
				+(rgb[1].r*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
				+(rgb[2].r*(feed_Y/(float)wh));
			
			p[xx-feed_X].g = (((rgb[0].g*(1.f-(xx/(float)ww)))
				+(rgb[1].g*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
				+(rgb[2].g*(feed_Y/(float)wh));
			
			p[xx-feed_X].b = (((rgb[0].b*(1.f-(xx/(float)ww)))
				+(rgb[1].b*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
				+(rgb[2].b*(feed_Y/(float)wh));
			
		} else {
			
			p[xx-feed_X].r = (Fl::get_color(widget->color())>>8)&0xff;
			p[xx-feed_X].g = (Fl::get_color(widget->color())>>16)&0xff;
			p[xx-feed_X].b = (Fl::get_color(widget->color())>>24)&0xff;
			
		}
	}
	
}

static void draw_shaded_quad(void* userdata, int feed_X, int feed_Y, 
	int feed_W, uchar* buf) {
	
	PIXEL* p = (PIXEL*)buf;
	ColorPreview* widget = (ColorPreview*)userdata;
	
	SmxClass::RGB rgb[4] = {
		project.active_primitive->c[0],
		project.active_primitive->c[1],
		project.active_primitive->c[2],
		project.active_primitive->c[3]
	};
	
	int ww = widget->w()-8;
	int wh = widget->h()-8;
	
	int tw = floor( ww*(1.f-(feed_Y/(float)wh)) );
			
	for ( int xx=feed_X; xx<feed_X+feed_W; xx++ ) {
			
		p[xx-feed_X].r = (((rgb[0].r*(1.f-(xx/(float)ww)))+(rgb[1].r*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
			+(((rgb[2].r*(1.f-(xx/(float)ww)))+(rgb[3].r*(xx/(float)ww)))*(feed_Y/(float)wh));
		p[xx-feed_X].g = (((rgb[0].g*(1.f-(xx/(float)ww)))+(rgb[1].g*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
			+(((rgb[2].g*(1.f-(xx/(float)ww)))+(rgb[3].g*(xx/(float)ww)))*(feed_Y/(float)wh));
		p[xx-feed_X].b = (((rgb[0].b*(1.f-(xx/(float)ww)))+(rgb[1].b*(xx/(float)ww)))*(1.f-(feed_Y/(float)wh)))
			+(((rgb[2].b*(1.f-(xx/(float)ww)))+(rgb[3].b*(xx/(float)ww)))*(feed_Y/(float)wh));

	}
	
}

ColorPreview::ColorPreview(int x, int y, int w, int h, const char* l) 
	: Fl_Group( x, y, w, h, l ) {
	
	end();
}

ColorPreview::~ColorPreview() {
}

void ColorPreview::draw() {
	
	draw_box( box(), x(), y(), w(), h(), color() );
	
	fl_color( FL_BLACK );
			
	if ( project.active_primitive != nullptr ) {
		
		switch( project.active_primitive->primtype )
		{
			case SmxClass::PRIM_TRI_F:
			case SmxClass::PRIM_TRI_FT:
				fl_color( 
					project.active_primitive->c[0].r,
					project.active_primitive->c[0].g,
					project.active_primitive->c[0].b );
				fl_polygon( x()+4, y()+4, 
					(x()+w())-8, y()+4, 
					x()+4, (y()+h())-5, 
					x()+4, (y()+h())-5 );
				break;
			case SmxClass::PRIM_QUAD_F:
			case SmxClass::PRIM_QUAD_FT:
				fl_color( 
					project.active_primitive->c[0].r,
					project.active_primitive->c[0].g,
					project.active_primitive->c[0].b );
				fl_polygon( x()+4, y()+4, 
					(x()+w())-5, y()+4, 
					(x()+w())-5, (y()+h())-5, 
					x()+4, (y()+h())-5 );
				break;
			case SmxClass::PRIM_TRI_G:
			case SmxClass::PRIM_TRI_GT:
				fl_draw_image( draw_shaded_triangle, this, 
					x()+4, y()+4, w()-8, h()-8, 3 );
				break;
			case SmxClass::PRIM_QUAD_G:
			case SmxClass::PRIM_QUAD_GT:
				fl_draw_image( draw_shaded_quad, this, 
					x()+4, y()+4, w()-8, h()-8, 3 );
				break;
		}
		
	}
	
}