/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   context.h
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 2:04 PM
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "ModelClass.h"

#define FILE_TYPE_SMX	0
#define FILE_TYPE_RSD	1

class ContextClass {
public:
	ContextClass();
	virtual ~ContextClass();
	
	ModelClass model;
	ModelClass bg_model;
	
	int last_selected;
	
	std::string file_name;
	
	SmxClass::PRIMITIVE *active_primitive;
	SmxClass::PRIMITIVE temp;
	
private:

};

#endif /* CONTEXT_H */

