/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigClass.cpp
 * Author: lameguy64
 * 
 * Created on April 17, 2018, 10:41 AM
 */

#ifndef __WIN32__
#include <termios.h>
#endif

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>

#include <Fl/Fl_Native_File_Chooser.H>
#include <Fl/fl_message.H>
#include <Fl/fl_ask.H>
#include <tinyxml2.h>

#include "ConfigClass.h"
#include "configform.h"
#include "global.h"
#include "common.h"

ConfigClass::ConfigClass() {
	
	systemcolors_enable	= false;
	nsfw_enable			= false;
	
	singlebuffer_enable	= false;
	glsl_enable			= false;
	
}

ConfigClass::~ConfigClass() {
}

int ConfigClass::LoadConfig() {
	
	tinyxml2::XMLDocument document;
	
	if ( document.LoadFile( config_file.c_str() ) != tinyxml2::XML_SUCCESS ) {
		return 0;
	}
	
	tinyxml2::XMLElement* base = document.FirstChildElement( "smxtool_config" );
	
	if ( base ) {
		
		tinyxml2::XMLElement* o = base->FirstChildElement( "interface" );
		if ( o ) {
			
			systemcolors_enable = o->IntAttribute( "use_system_colors" );
			nsfw_enable = o->IntAttribute( "nsfw" );
			
		}
		
		o = base->FirstChildElement( "opengl" );
		if ( o ) {
			
			singlebuffer_enable = o->IntAttribute( "single_buffer" );
			glsl_enable = o->IntAttribute( "glsl" );
			
		}
		
	}
	
	return 1;
}

int ConfigClass::Saveconfig() {
	
	tinyxml2::XMLDocument document;
	
	tinyxml2::XMLElement* base = document.NewElement( "smxtool_config" );
	
	tinyxml2::XMLElement* o = document.NewElement( "interface" );
	o->SetAttribute( "use_system_colors", systemcolors_enable );
	o->SetAttribute( "nsfw", nsfw_enable );
	base->InsertEndChild( o );
	
	o = document.NewElement( "opengl" );
	o->SetAttribute( "single_buffer", singlebuffer_enable );
	o->SetAttribute( "glsl", glsl_enable );
	base->InsertEndChild( o );
	
	o = document.NewElement( "externals" );
	
	base->InsertEndChild( o );
	
	document.InsertEndChild( base );
	
	if ( document.SaveFile( config_file.c_str() ) != tinyxml2::XML_SUCCESS ) {
		return 0;
	}
	
	return 1;
}


static ConfigUI *config_ui;

void settingsItem_cb(Fl_Menu_* widget, long userdata) {
	
	config_ui = new ConfigUI();
	
	config_ui->position( (ui->x()+ui->w()/2)-config_ui->w()/2, 
		(ui->y()+ui->h()/2)-config_ui->h()/2 );
	
	config_ui->systemColorsToggle->value( config.systemcolors_enable );
	config_ui->easterEggToggle->value( config.nsfw_enable );
	config_ui->singleBufferToggle->value( config.singlebuffer_enable );
	config_ui->shadersToggle->value( config.glsl_enable );
	
	
	config_ui->show();
	
	while( config_ui->shown() ) {
		Fl::wait();
	}
	
	delete config_ui;
	
}

void configOkayButton_cb(Fl_Button* widget, void* userdata) {
	
	config.systemcolors_enable	= config_ui->systemColorsToggle->value();
	config.nsfw_enable			= config_ui->easterEggToggle->value();
	
	config.singlebuffer_enable	= config_ui->singleBufferToggle->value();
	config.glsl_enable			= config_ui->shadersToggle->value();
	
	if ( !config.Saveconfig() ) {
		fl_message_title( "Error Saving Config" );
		fl_message( "Unable to write configuration file." );
		return;
	}
	
	config_ui->hide();
}