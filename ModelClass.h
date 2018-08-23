/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   modelclass.h
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 2:05 PM
 */

#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <vector>
#include <smx.h>
#include "TextureClass.h"

class ModelClass : public SmxClass {
public:
	ModelClass();
	virtual ~ModelClass();
	
	enum FileType {
		FILETYPE_SMX = 0,
		FILETYPE_RSD
	};
	
	int LoadFile(const char* file_name);
	int SaveFile(const char* file_name);
	
	int MergeSmxFile(const char* file_name);
	
	SmxClass::SMX_ERROR LoadRsdData(const char* file_name);
	SmxClass::SMX_ERROR SaveRsdData(const char* file_name);
	
	void LoadTextures( const char* src_file = nullptr );
	
	void AddTexture(const char* file_name);
	int ReplaceTexture(int index, const char* file_name);
	void DeleteTexture(int index);
	
	void RenderSelected();
	void RenderTextured(int culling);
	void RenderWireframe();
	
	void SetShading(SmxClass::PRIMITIVE* prim, int shaded);
	void SetTexturing(SmxClass::PRIMITIVE* prim, int textured, int texnum);
	
	int ScanFace(SmxClass::VECTOR3D pos, SmxClass::VECTOR3D ray);
	
	void ClearSelection();
	void ClearTextures();
	
	std::vector<TexImageClass*> texture_images;
	
	std::string rsd_ply_name;
	std::string rsd_mat_name;
	int file_type;
	
private:
	
	int LoadTextureFile(const char* file_name, int index);
};

#endif /* MODELCLASS_H */

