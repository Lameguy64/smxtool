/* 
 * File:   main.cpp
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 1:01 PM
 */


#include <iostream>
#include <fstream>
#include <unistd.h>
#include <Fl/fl_ask.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <common.h>
#include "global.h"
#include "about.h"

ConfigClass config;
ContextClass project;

MainUI* ui;


void ParseTextures() {
	
	ui->textureBrowser->clear();
	
	for( int i=0; i<project.model.textures.size(); i++ ) {
		
		ui->textureBrowser->add( project.model.textures[i]->filename.c_str(), 
			nullptr );
		
	}
	
	ui->textureBrowser->redraw();
	
}

int DeactivateAll() {
	
	if ( project.last_selected >= 0 ) {
		
		unsetFace();
		
		return 1;
	}
	
	if ( ui->paintModeToggle->value() ) {
		
		ui->paintModeToggle->value( 0 );
		ui->colorPreviewer->redraw();
		ui->texturePreviewer->redraw();
		return 1;
		
	}
	
	return 0;
}

void aboutItem_cb(Fl_Menu_* widget, void* userdata) {
	
	AboutUI *about = new AboutUI();
	
	about->versionText->value( VERSION );
	
	about->position( (ui->x()+(ui->w()/2))-(about->w()/2), 
		(ui->y()+(ui->h()/2))-(about->h()/2) );
	
	about->show();
	
	while( about->shown() ) {
		Fl::wait();
	}
	
	delete about;
	
}

void aboutButton_cb(Fl_Button* widget, void* userdata) {
	
	aboutItem_cb( (Fl_Menu_*)widget, userdata );
	
}

void addTextureButton_cb(Fl_Button* widget, void* userdata) {
	
	DeactivateAll();
	
	if ( project.file_name.empty() ) {
		fl_message_title( "No model loaded" );
		fl_message( "You cannot add textures without a model loaded." );
		return;
	}
	
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Add Texture File" );
	file_chooser.filter( "Texture Image File\t*.tim\n" );
	file_chooser.type( file_chooser.BROWSE_FILE );
	
	switch( file_chooser.show() ) {
		case -1:
		case 1:
			break;
		default:
			
#ifdef DEBUG
			std::cout << "Add texture file " 
				<< file_chooser.filename() << std::endl;
#endif
			
			project.model.AddTexture( file_chooser.filename() );
			ParseTextures();
			
			break;
	}
	
}

void replaceTextureButton_cb(Fl_Button* widget, void* userdata) {
	
	DeactivateAll();
	
	if ( project.file_name.empty() ) {
		fl_message_title( "No model loaded" );
		fl_message( "You cannot add textures without a model loaded." );
		return;
	}
	
	int replace_index = ui->textureBrowser->value()-1;
	
	if ( replace_index < 0 ) {
		fl_message_title( "No texture selected" );
		fl_message( "Select a texture to replace." );
		return;
	}
	
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Replace Texture File" );
	file_chooser.filter( "Texture Image File\t*.tim\n" );
	file_chooser.type( file_chooser.BROWSE_FILE );
	
	switch( file_chooser.show() ) {
		case -1:
		case 1:
			break;
		default:
			
#ifdef DEBUG
			std::cout << "Add texture file " 
				<< file_chooser.filename() << std::endl;
#endif
			
			if ( project.model.ReplaceTexture( replace_index, file_chooser.filename() ) ) {
				ParseTextures();
				ui->texturePreviewer->redraw();
				ui->glviewport->redraw();
			}
			
			break;
	}
	
}

void deleteTextureButton_cb(Fl_Button* widget, void* userdata) {
	
	if ( project.model.textures.size() == 0 ) {
		fl_message_title( "No textures" );
		fl_message( "No textures to delete." );
		return;
	}
	
	int index = ui->textureBrowser->value()-1;
	
	if ( index < 0 ) {
		fl_message_title( "No texture selected" );
		fl_message( "Select a texture to delete." );
		return;
	}
	
	fl_message_title( "Confirm texture deletion" );
	if ( fl_choice( "Primitives with this texture assigned will become untextured.\n"
		"Are you sure?", "No", "Yes", 0 ) == 0 ) {
		return;
	}
	
	project.model.DeleteTexture( index );
	ParseTextures();
	ui->glviewport->redraw();
	ui->texturePreviewer->redraw();
	
}

void saveModelFile_cb(Fl_Menu_* widget, void* userdata) {
	
	if ( project.file_name.empty() ) {
		fl_message_title( "No Model Loaded" );
		fl_message( "You cannot save an empty document." );
		return;
	}
	
	if ( project.model.SaveFile( project.file_name.c_str() )
		!= SmxClass::ERR_NONE ) {
		fl_message_title( "File save error." );
		fl_message( "Error saving selected file." );
		return;
	}
	
}

void saveModelButton_cb(Fl_Button* widget, void* userdata) {
	
	saveModelFile_cb((Fl_Menu_*)widget, userdata);
	
}

void saveModelFileAs_cb(Fl_Menu_* widget, void* userdata) {
	
	DeactivateAll();
	
	if ( project.file_name.empty() ) {
		
		fl_message_title( "No SMX Loaded" );
		fl_message( "You cannot save an empty document." );
		return;
	}
	
	std::string temp;
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Save Model File As" );
	file_chooser.filter( "Scarlet Model XML\t*.smx\nRSD Model Data\t*.rsd\n" );
	file_chooser.type( Fl_Native_File_Chooser::BROWSE_SAVE_FILE );
	file_chooser.options( Fl_Native_File_Chooser::USE_FILTER_EXT );
	
	switch ( file_chooser.show() )
	{
	case -1: return;
	case  1: return;
	default:
		
		temp = file_chooser.filename();
		
		if ( file_chooser.filter_value() == 0 ) {
			EnsureFileExtension( temp, "smx" );
		} else if ( file_chooser.filter_value() == 1 ) {
			EnsureFileExtension( temp, "rsd" );
		}
		
#ifdef DEBUG
		std::cout << temp << std::endl;
#endif
		
		if ( project.model.SaveFile( temp.c_str() ) != SmxClass::ERR_NONE ) {
			fl_message_title( "File save error" );
			fl_message( "Error saving selected file." );
			return;
		}
		
		project.file_name = temp;
		break;
	}
}

void loadModelFile_cb(Fl_Menu_* widget, void* userdata) {
	
	DeactivateAll();
			
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Open Model File" );

#ifdef __WIN32__
	file_chooser.filter( "All Supported Files\t*.smx;*.rsd\nScarlet Model XML\t*.smx\nRSD Model Data\t*.rsd\n" );
#else
	file_chooser.filter( "Scarlet Model XML\t*.smx\nRSD Model Data\t*.rsd\n" );
#endif
	
	switch ( file_chooser.show() )
	{
	case -1: return;
	case  1: return;
	default:
		
		switch( project.model.LoadFile( file_chooser.filename() ) ) {
			case ModelClass::ERR_NOTFOUND:
				fl_message_title( "No file extension" );
				fl_message( "Specified file was not found." );
				return;
			case ModelClass::ERR_INVALID:
				fl_message_title( "Invalid file" );
				fl_message( "Specified file is invalid." );
				return;
			case ModelClass::ERR_NOEXT:
				fl_message_title( "No file extension" );
				fl_message( "File extension not specified." );
				return;
		}
		
		project.file_name = file_chooser.filename();
		project.active_primitive = nullptr;
		project.last_selected = -1;
		ui->glviewport->redraw();
		ParseTextures();
		
		break;
	}
}

void loadModelButton_cb(Fl_Button* widget, void* userdata) {
	
	loadModelFile_cb((Fl_Menu_*)widget, userdata);
	
}

void mergeModelFile_cb(Fl_Menu_* widget, void* userdata) {
	
	DeactivateAll();
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Merge Model File" );
	file_chooser.filter( "Scarlet Model XML\t*.smx\n" );
	
	switch ( file_chooser.show() )
	{
	case -1: return;
	case  1: return;
	default:
		if ( project.model.MergeSmxFile( file_chooser.filename() )
			!= SmxClass::ERR_NONE ) {
			fl_message_title( "File load error." );
			fl_message( "Error loading selected file." );
			return;
		}
		ParseTextures();
		break;
	}
}

void loadBgModelFile_cb(Fl_Button* widget, void* userdata) {
	
	DeactivateAll();
	Fl_Native_File_Chooser file_chooser;
	
	file_chooser.title( "Load Model File as Background" );
#ifdef __WIN32__
	file_chooser.filter( "All Supported Files\t*.smx;*.rsd\nScarlet Model XML\t*.smx\nRSD Model Data\t*.rsd\n" );
#else
	file_chooser.filter( "Scarlet Model XML\t*.smx\nRSD Model Data\t*.rsd\n" );
#endif
	
	switch ( file_chooser.show() )
	{
		case -1: return;
		case  1: return;
		default:
			if ( project.bg_model.LoadFile( file_chooser.filename() )
				!= SmxClass::ERR_NONE ) {
				fl_message_title( "File load error." );
				fl_message( "Error loading selected file." );
				return;
			}
			ui->glviewport->redraw();
			break;
	}
	
}

void selectAllItem_cb(Fl_Menu_* widget, void* userdata) {
	
	for( int i=0; i<project.model.primitives.size(); i++ ) {
		
		project.model.primitives[i].selected = true;
		
	}
	
	ui->glviewport->redraw();
	
}

void invertSelectionItem_cb(Fl_Menu_* widget, void* userdata) {
	
	for( int i=0; i<project.model.primitives.size(); i++ ) {
		
		project.model.primitives[i].selected 
			= !project.model.primitives[i].selected;
		
	}
	
	ui->glviewport->redraw();
	
}

void selBringFront_cb(Fl_Menu_* widget, void* userdata) {
	
	ModelClass* m = &project.model;
	
	for ( int i=0; i<m->primitives.size(); i++ ) {
		for ( int j=0; j<m->primitives.size()-1; j++ ) {
			
			if ( m->primitives[j].selected < m->primitives[j+1].selected ) {
				std::swap( m->primitives[j], m->primitives[j+1] );
			}
			
		}
	}
	
	setActiveFace( 0 );
		
}

void selSendBack_cb(Fl_Menu_* widget, void* userdata) {
	
	ModelClass* m = &project.model;
	
	for ( int i=0; i<m->primitives.size(); i++ ) {
		for ( int j=0; j<m->primitives.size()-1; j++ ) {
			
			if ( m->primitives[j].selected > m->primitives[j+1].selected ) {
				std::swap( m->primitives[j], m->primitives[j+1] );
			}
			
		}
	}
	
	setActiveFace( project.model.primitives.size()-1 );
	
}

void mainUI_cb(Fl_Double_Window* widget, void* userdata) {
	
	if ( DeactivateAll() ) {
		return;
	}
	
	widget->hide();
	
}

void exitItem_cb(Fl_Menu_* widget, void* userdata ) {
	
	ui->hide();
	
}

void init() {
	
	fl_message_font( FL_HELVETICA, 12 );
	
	char currentDir[MAX_PATH];
	getcwd( currentDir, MAX_PATH );
	
	config.program_path = currentDir;
#ifdef _WIN32
	config.program_path += "\\";
#else
	config.program_path += "/";
#endif
	
	
	// Determine path where scarlet config files will be stored
#ifdef _WIN32
	if ( getenv( "APPDATA" ) != NULL ) {
		
		config.config_file = getenv( "APPDATA" );
		config.config_file += "\\scarlet";
		
	} else {
		
		config.config_file = "scarlet";
		
	}
#else
	if ( getenv( "XDG_CONFIG_HOME" ) != NULL ) {
		
		config.config_file = getenv( "XDG_CONFIG_HOME" );
		config.config_file += "/scarlet";
		
	} else if ( getenv( "HOME" ) != NULL ) {
		
		config.config_file = getenv( "HOME" );
		config.config_file += "/.config/scarlet";
		
	} else {
		
		config.config_file = config.program_path;
		
	}
#endif
	
	// Create directory to store scarlet SDK files
	if ( !FileExists( config.config_file.c_str() ) ) {
		
		// Syntax of mkdir is different between Windows and *nix 
#ifdef _WIN32
		mkdir( config.config_file.c_str() );
#else
		mkdir( config.config_file.c_str(), S_IREAD|S_IWRITE|S_IEXEC );
#endif
		
	}

	config.config_file += "/smxtool.cfg";
	
#ifdef DEBUG
	std::cout << "-- Configuration --" << std::endl;
	std::cout << "Program directory: " << config.program_path.c_str() << std::endl;
	std::cout << "Config file " << config.config_file << std::endl;
	std::cout << "-- End Configuration --" << std::endl;
#endif
	
	// Load config
	config.LoadConfig();
	
	if ( config.systemcolors_enable ) {
		Fl::get_system_colors();
	}
	
	ui = new MainUI();
	
	if ( config.singlebuffer_enable ) {	
		ui->glviewport->mode( FL_RGB|FL_DEPTH );
	}
	
}

int main(int argc, char** argv) {
	
	init();
	
	ui->show();
	Fl::wait();
	
	aboutItem_cb( nullptr, nullptr );
			
	int ret = Fl::run();
	
	return ret;
}

