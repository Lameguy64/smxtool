/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   context.cpp
 * Author: Lameguy64
 * 
 * Created on March 13, 2018, 2:04 PM
 */

#include "ContextClass.h"

ContextClass::ContextClass() {
	last_selected = -1;
	active_primitive = &temp;
}

ContextClass::~ContextClass() {
}

