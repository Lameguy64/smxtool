/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigClass.h
 * Author: lameguy64
 *
 * Created on April 17, 2018, 10:41 AM
 */

#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H

#include <string>

class ConfigClass {
public:
	
	ConfigClass();
	virtual ~ConfigClass();
	
	std::string program_path;
	std::string config_file;
	
	int systemcolors_enable;
	int nsfw_enable; // :)
	int glsl_enable;
	int singlebuffer_enable;
	
	int LoadConfig();
	int Saveconfig();
	
private:

};

#endif /* CONFIGCLASS_H */

