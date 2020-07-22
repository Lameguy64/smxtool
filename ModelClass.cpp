/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   modelclass.cpp
 * Author: Lameguy64
 * 
 * Created on March 13, 2018, 2:05 PM
 */

#include <iostream>
#include <algorithm>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <Fl/fl_message.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <GL/glew.h>

#include "global.h"
#include "ModelClass.h"
#include "mainform.h"
#include "common/common.h"

const char* GetExtension(const char* str) {
	
	for(int i=strlen(str)-1; i>=0; i--){
		
		if ( str[i] == '.' ) {
			return str+i;
		}
		
	}
	
	return nullptr;
}

void CleanString(char* str) {
	
	char* pos = strstr( str, "\n" );
	
	if ( pos ) {
		pos[0] = 0x00;
	}
	
	pos = strstr( str, "\r" );
	
	if ( pos ) {
		pos[0] = 0x00;
	}
	
}

void r_translate(SmxClass::VECTOR3D* vec, SmxClass::VECTOR3D trns)
{
	vec->x -= trns.x;
	vec->y -= trns.y;
	vec->z -= trns.z;
}

SmxClass::VECTOR3D crossProduct(SmxClass::VECTOR3D *v0, SmxClass::VECTOR3D *v1)
{
	SmxClass::VECTOR3D ret;

	ret.x = v0->y*v1->z - v0->z*v1->y;
	ret.y = v0->z*v1->x - v0->x*v1->z;
	ret.z = v0->x*v1->y - v0->y*v1->x;

	return ret;
}

float dotProduct(SmxClass::VECTOR3D *v0, SmxClass::VECTOR3D *v1)
{
	return v0->x*v1->x+v0->y*v1->y+v0->z*v1->z;
}

float distance(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

float RayIntersectsTriangle(SmxClass::VECTOR3D *r, SmxClass::VECTOR3D *v1, 
	SmxClass::VECTOR3D *v2, SmxClass::VECTOR3D *v3)
{
    // Compute vectors along two edges of the triangle.
    SmxClass::VECTOR3D edge1,edge2;

    edge1.x = v2->x - v1->x; edge1.y = v2->y - v1->y; edge1.z = v2->z - v1->z;
    edge2.x = v3->x - v1->x; edge2.y = v3->y - v1->y; edge2.z = v3->z - v1->z;

    // Compute the determinant.
    SmxClass::VECTOR3D directionCrossEdge2;
    directionCrossEdge2 = crossProduct( r, &edge2 );

    float determinant = dotProduct( &directionCrossEdge2, &edge1 );

    // If the ray and triangle are parallel, there is no collision.
    if ( determinant > -.0000001f && determinant < .0000001f )
	{
        return 0xffffffff;
	}
	
    float inverseDeterminant = 1.0f / determinant;

    // Calculate the U parameter of the intersection point.
    SmxClass::VECTOR3D distanceVector;
    distanceVector.x = r->x - v1->x;
    distanceVector.y = r->y - v1->y;
    distanceVector.z = r->z - v1->z;

    float triangleU = dotProduct( &directionCrossEdge2, &distanceVector );
    triangleU *= inverseDeterminant;

    // Make sure the U is inside the triangle.
    if ( triangleU < 0 || triangleU > 1 )
	{
        return 0xffffffff;
	}
	
    // Calculate the V parameter of the intersection point.
    SmxClass::VECTOR3D distanceCrossEdge1;
    distanceCrossEdge1 = crossProduct( &distanceVector, &edge1 );

    float triangleV = dotProduct( r, &distanceCrossEdge1 );
    triangleV *= inverseDeterminant;

    // Make sure the V is inside the triangle.
    if ( triangleV < 0 || triangleU + triangleV > 1 )
	{
        return 0xffffffff;
	}
	
    // Get the distance to the face from our ray origin
    float rayDistance = dotProduct(&distanceCrossEdge1, &edge2);
    rayDistance *= inverseDeterminant;

    // Is the triangle behind us?
    if (rayDistance < 0)
	{
        return 0xffffffff;
	}
	
    return rayDistance;

}

float RayIntersectsQuad(SmxClass::VECTOR3D *r, SmxClass::VECTOR3D *v1, 
	SmxClass::VECTOR3D *v2, SmxClass::VECTOR3D *v3, SmxClass::VECTOR3D *v4) {
	
    float value = RayIntersectsTriangle( r, v1, v2, v3 );
	
	if ( value == 0xffffffff ) {
		
		value = RayIntersectsTriangle( r, v4, v3, v2 );
		
	}
	
	return value;
}

ModelClass::ModelClass() {
}

ModelClass::~ModelClass() {
	
	if ( texture_images.size() > 0 ) {
		for ( int i=0; i<texture_images.size(); i++ ) {
			delete texture_images[i];
		}
		texture_images.clear();
	}
	
}

int ModelClass::LoadFile(const char* file_name) {
	
	Clear();
	ClearTextures();
	
	rsd_ply_name.clear();
	rsd_mat_name.clear();
	
	int err;
	const char* ext = GetExtension( file_name );
		
	if ( ext ) {

		if ( strcasecmp( ext, ".smx" ) == 0 ) {

			err = SmxClass::LoadFile( file_name );
			
			if ( err != SmxClass::ERR_NONE ) {
				return err;
			}
			
			file_type = FILETYPE_SMX;

		} else if ( strcasecmp( ext, ".rsd" ) == 0 ) {

			err = LoadRsdData( file_name );
					
			if ( err != SmxClass::ERR_NONE ) {
				return err;
			}
			
			file_type = FILETYPE_RSD;

		} else {

			return ERR_UNKNOWN;
			
		}
		
	} else {
		
		return ERR_NOEXT;
		
	}
	
	LoadTextures( file_name );
	
	return err;
}

int ModelClass::SaveFile(const char* file_name) {
	
	int err;
	const char* ext = GetExtension( file_name );
		
	if ( ext ) {

		if ( strcasecmp( ext, ".smx" ) == 0 ) {

			err = SmxClass::SaveFile( file_name );
			
			if ( err != SmxClass::ERR_NONE ) {
				return err;
			}
			
			rsd_ply_name.clear();
			rsd_mat_name.clear();
			
			file_type = FILETYPE_SMX;

		} else if ( strcasecmp( ext, ".rsd" ) == 0 ) {

			err = SaveRsdData( file_name );
					
			if ( err != SmxClass::ERR_NONE ) {
				return err;
			}
			
			file_type = FILETYPE_RSD;

		} else {

			return ERR_UNKNOWN;
			
		}
		
	} else {
		
		return ERR_NOEXT;
		
	}
	
	return SmxClass::ERR_NONE;
}

SmxClass::SMX_ERROR ModelClass::LoadRsdData(const char* file_name) {
	
	std::string ply_file;
	std::string mat_file;
	std::string temp_name;
	char* line_buff;
	size_t line_len;
	int found_header = false;
	int num_textures = false;
	
	FILE* fp = fopen( file_name, "r" );
	line_buff = (char*)malloc( 256 );
	
	while ( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( strncmp( line_buff, "@RSD940102", 10 ) == 0 ) {
			found_header = true;
			break;
		}
		
	}
	
	if ( !found_header ) {
		
		fclose( fp );
		free( line_buff );
		return SmxClass::ERR_INVALID;
		
	}
	
	// Get PLY and MAT file names
	while ( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( strncmp( line_buff, "PLY=", 4 ) == 0 ) {
			ply_file = &line_buff[4];
#ifdef DEBUG
			std::cout << "PLY file: " << ply_file << std::endl;
#endif
		} else if ( strncmp( line_buff, "MAT=", 4 ) == 0 ) {
			mat_file = &line_buff[4];
#ifdef DEBUG
			std::cout << "MAT file: " << mat_file << std::endl;
#endif
		} else if ( strncmp( line_buff, "NTEX=", 5 ) == 0 ) {
			
			num_textures = atoi( &line_buff[5] );
#ifdef DEBUG
			std::cout << "Number of textures: " << num_textures << std::endl;
#endif
			if ( num_textures > 0 ) {
				
				for( int i=0; i<num_textures; i++ ) {

					getline( &line_buff, &line_len, fp );
					CleanString( line_buff );
					const char* pos = strstr( line_buff, "]=" );

					if ( pos != nullptr ) {
						SmxClass::TextureClass* texture = new SmxClass::TextureClass();
						texture->filename = (pos+2);
						textures.push_back( texture );
#ifdef DEBUG
						std::cout << "Found texture " << texture->filename << std::endl;
#endif
					}

				}
				
			}
			
		}
		
	}
	
	fclose( fp );
	
	
	// Parse PLY file
	temp_name = file_name;
	StripFilename( temp_name );
	temp_name += ply_file;
	
#ifdef DEBUG
	std::cout << "Open " << temp_name << std::endl;
#endif
	
	fp = fopen( temp_name.c_str(), "r" );
	
	if ( fp == nullptr ) {
		free( line_buff );
		return SmxClass::ERR_NOTFOUND;
	}
	
	found_header = false;
	while( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( strcasecmp( line_buff, "@PLY940102" ) == 0 ) {
			found_header = true;
			break;
		}
		
	}
	
	if ( !found_header ) {
		fclose( fp );
		free( line_buff );
		return SmxClass::ERR_INVALID;
	}
	
	int vertex_count=0,normal_count=0,poly_count=0;
	
	while( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
	
		if ( line_buff[0] == '#' ) {
			continue;
		}
		
		if ( strlen( line_buff ) > 0 ) {
			sscanf( line_buff, "%d %d %d", &vertex_count, &normal_count, &poly_count );
			break;
		}
		
	}

#ifdef DEBUG
	std::cout << "Verts=" << vertex_count << " Norms=" << normal_count 
		<< " Polys=" << poly_count << std::endl;
#endif
	
	if ( vertex_count == 0 ) {
		fclose(fp);
		free( line_buff );
		return SmxClass::ERR_NO_VERTS;
	}
	
	if ( poly_count == 0 ) {
		fclose(fp);
		free( line_buff );
		return SmxClass::ERR_NO_PRIMS;
	}
	
	// Load vertices
	for( int i=0; i<vertex_count; i++ ) {
		
		SmxClass::VECTOR3D v;
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( line_buff[0] == '#' ) {
			i--;
			continue;
		}
		
		if ( strlen( line_buff ) > 0 ) {
			sscanf( line_buff, "%E %E %E", &v.x, &v.y, &v.z );
			vertices.push_back( v );
		}
		
	}
	
	// Load normals
	for( int i=0; i<normal_count; i++ ) {
		
		SmxClass::VECTOR3D v;
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( line_buff[0] == '#' ) {
			i--;
			continue;
		}
		
		if ( strlen( line_buff ) > 0 ) {
			sscanf( line_buff, "%E %E %E", &v.x, &v.y, &v.z );
			normals.push_back( v );
		}
		
	}
	
	// Load polygons
	for( int i=0; i<poly_count; i++ ) {
		
		SmxClass::PRIMITIVE prim;
		int type;
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( line_buff[0] == '#' ) {
			i--;
			continue;
		}
		
		if ( strlen( line_buff ) > 0 ) {
			
			memset( &prim, 0x0, sizeof(SmxClass::PRIMITIVE) );
			
			sscanf( line_buff, "%d %d %d %d %d %d %d %d %d", &type, 
				&prim.v[0], &prim.v[1], &prim.v[2], &prim.v[3], 
				&prim.n[0], &prim.n[1], &prim.n[2], &prim.n[3] );
			
			if ( type == 0 ) {
				
				prim.primtype = SmxClass::PRIM_TRI_F;
				
			} else if ( type == 1 ) {
				
				prim.primtype = SmxClass::PRIM_QUAD_F;
				
			}
			
			prim.c[0].r = 255;
			prim.c[0].g = 255;
			prim.c[0].b = 255;
			prim.c[1] = prim.c[0];
			prim.c[2] = prim.c[0];
			prim.c[3] = prim.c[0];
			
			prim.texnum = 0;
			prim.t[0].u = 0;
			prim.t[0].v = 0;
			prim.t[1] = prim.t[0];
			prim.t[2] = prim.t[0];
			prim.t[3] = prim.t[0];
			
			primitives.push_back( prim );
			
		}
		
	}
	
#ifdef DEBUG
	std::cout << "Verts=" << vertices.size() << " Norms=" << normals.size()
		<< " Polys=" << primitives.size() << std::endl;
#endif
	
	fclose( fp );
	
	
	// Parse MAT file
	temp_name = file_name;
	StripFilename( temp_name );
	temp_name += mat_file;
	
#ifdef DEBUG
	std::cout << "Open " << temp_name << std::endl;
#endif
	
	fp = fopen( temp_name.c_str(), "r" );
	
	if ( fp == nullptr ) {
		fclose( fp );
		free( line_buff );
		Clear();
		return SmxClass::ERR_NOTFOUND;
	}
	
	found_header = false;
	while( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		if ( strcasecmp( line_buff, "@MAT940801" ) == 0 ) {
			found_header = true;
			break;
		}
		
	}
	
	if ( !found_header ) {
		fclose( fp );
		free( line_buff );
		Clear();
		return SmxClass::ERR_INVALID;
	}
	
	while( !feof( fp ) ) {
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
	
		if ( line_buff[0] == '#' ) {
			continue;
		}
		
		if ( strlen( line_buff ) > 0 ) {
			sscanf( line_buff, "%d", &poly_count );
			break;
		}
		
	}
	
#ifdef DEBUG
	std::cout << "Material entries: " << poly_count << std::endl;
#endif
	
	for( int i=0; i<poly_count; i++ ) {
		
		int index,flag;
		char shading,material;
		
		line_len = 256;
		getline( &line_buff, &line_len, fp );
		CleanString( line_buff );
		
		sscanf( line_buff, "%d %d %c %c", &index, &flag, &shading, &material );
		
		if ( !(flag & 0x1) ) {	// Light source calculation
			if ( toupper( shading ) == 'G' ) {
				primitives[index].shadetype = SHADE_SMOOTH;
			} else if ( toupper( shading ) == 'F' ) {
				primitives[index].shadetype = SHADE_FLAT;
			}
		} else {
			primitives[index].shadetype = SHADE_NONE;
		}
		
		if ( flag & 0x2 ) {		// Double sided
			primitives[index].doublesided = true;
		} else {
			primitives[index].doublesided = false;
		}
		
		if ( flag & 0x4 ) {		// Semi-transparency
			primitives[index].blendmode = 1+(( flag>>3 )&0x7);
		} else {
			primitives[index].blendmode = 0;
		}
		
		const char* param_pos = strchr( line_buff, material );
		
		if ( toupper( material ) == 'C' ) { // Color
			
			int r,g,b;
			sscanf( param_pos+1, "%d %d %d", &r, &g, &b );
			
			primitives[index].c[0].r = r;
			primitives[index].c[0].g = g;
			primitives[index].c[0].b = b;
			
			primitives[index].c[1] = primitives[index].c[0];
			primitives[index].c[2] = primitives[index].c[0];
			primitives[index].c[3] = primitives[index].c[0];
			
		} else if ( toupper( material ) == 'G' ) { // Shaded color
			
			int r[4],g[4],b[4];
			sscanf( param_pos+1, "%d %d %d %d %d %d %d %d %d %d %d %d", 
				&r[0], &g[0], &b[0], &r[1], &g[1], &b[1], 
				&r[2], &g[2], &b[2], &r[3], &g[3], &b[3] );
			
			for( int j=0; j<4; j++ ) {
				primitives[i].c[j].r = r[j];
				primitives[i].c[j].g = g[j];
				primitives[i].c[j].b = b[j];
			}
			
			if ( primitives[index].primtype == PRIM_TRI_F ) {
				primitives[index].primtype = PRIM_TRI_G;
			} else if ( primitives[index].primtype == PRIM_QUAD_F ) {
				primitives[index].primtype = PRIM_QUAD_G;
			}
			
		} else if ( toupper( material ) == 'T' ) { // Texture
			
			int texnum,u[4],v[4];
			
			sscanf( param_pos+1, "%d %d %d %d %d %d %d %d %d", &texnum, 
				&u[0], &v[0], &u[1], &v[1], 
				&u[2], &v[2], &u[3], &v[3] );
			
			primitives[index].texnum = texnum;
			
			for ( int j=0; j<4; j++ ) {
				primitives[index].t[j].u = u[j];
				primitives[index].t[j].v = v[j];
			}
			
			if ( primitives[index].primtype == PRIM_TRI_F ) {
				primitives[index].primtype = PRIM_TRI_FT;
			} else if ( primitives[index].primtype == PRIM_QUAD_F ) {
				primitives[index].primtype = PRIM_QUAD_FT;
			}
			
			primitives[index].c[0].r = primitives[index].c[0].g 
				= primitives[index].c[0].b = 128;
			
			primitives[index].c[1] = primitives[index].c[0];
			primitives[index].c[2] = primitives[index].c[0];
			primitives[index].c[3] = primitives[index].c[0];
			
		} else if ( toupper( material ) == 'D' ) { // Colored texture
			
			int r,g,b;
			int texnum,u[4],v[4];
			
			sscanf( param_pos+1, "%d %d %d %d %d %d %d %d %d %d %d %d", &texnum, 
				&u[0], &v[0], &u[1], &v[1], 
				&u[2], &v[2], &u[3], &v[3], &r, &g, &b );
			
			primitives[index].texnum = texnum;
			
			for ( int j=0; j<4; j++ ) {
				primitives[index].t[j].u = u[j];
				primitives[index].t[j].v = v[j];
			}
			
			if ( primitives[index].primtype == PRIM_TRI_F ) {
				primitives[index].primtype = PRIM_TRI_FT;
			} else if ( primitives[index].primtype == PRIM_QUAD_F ) {
				primitives[index].primtype = PRIM_QUAD_FT;
			}
			
			primitives[index].c[0].r = r;
			primitives[index].c[0].g = g;
			primitives[index].c[0].b = b;
			
			primitives[index].c[1] = primitives[index].c[0];
			primitives[index].c[2] = primitives[index].c[0];
			primitives[index].c[3] = primitives[index].c[0];
			
		} else if ( toupper( material ) == 'H' ) {
			
			int r[4],g[4],b[4];
			int texnum,u[4],v[4];
			
			sscanf( param_pos+1, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &texnum, 
				&u[0], &v[0], &u[1], &v[1], 
				&u[2], &v[2], &u[3], &v[3], 
				&r[0], &g[0], &b[0],
				&r[1], &g[1], &b[1],
				&r[2], &g[2], &b[2],
				&r[3], &g[3], &b[3] );
			
			primitives[index].texnum = texnum;
			
			for ( int j=0; j<4; j++ ) {
				primitives[index].t[j].u = u[j];
				primitives[index].t[j].v = v[j];
				primitives[index].c[j].r = r[j];
				primitives[index].c[j].g = g[j];
				primitives[index].c[j].b = b[j];
			}
			
			if ( primitives[index].primtype == PRIM_TRI_F ) {
				primitives[index].primtype = PRIM_TRI_GT;
			} else if ( primitives[index].primtype == PRIM_QUAD_F ) {
				primitives[index].primtype = PRIM_QUAD_GT;
			}
						
		}
		
	}
	
	fclose( fp );
	free( line_buff );
	
	rsd_ply_name = ply_file;
	rsd_mat_name = mat_file;
	
	return SmxClass::ERR_NONE;
}

std::string makeLower( const char* file_name ) {
	
	std::string result;
	while( *file_name != 0x0 ) {
		result += tolower( *file_name );
		file_name++;
	}
	
	return result;
}

SmxClass::SMX_ERROR ModelClass::SaveRsdData(const char* file_name) {
	
	std::string path;
	
	if ( rsd_ply_name.empty() ) {
		
		rsd_ply_name = file_name;
		rsd_mat_name = rsd_ply_name;
		EnsureFileExtension( rsd_ply_name, "ply" );
		EnsureFileExtension( rsd_mat_name, "mat" );

#ifdef DEBUG
		std::cout << "Generated names:" << std::endl;
		std::cout << "PLY=" << rsd_ply_name << std::endl;
		std::cout << "MAT=" << rsd_mat_name << std::endl;
#endif

	}
		
	// Write RSD
	
	FILE* fp = fopen( file_name, "w" );
	
	if ( fp == nullptr ) {
		return ERR_INVALID;
	}
	
	fputs( "# Created by SMXTOOL " VERSION " by Lameguy64\n", fp );
	fputs( "@RSD940102\n", fp );
	
	fprintf( fp, "PLY=%s\n", rsd_ply_name.c_str() );
	fprintf( fp, "MAT=%s\n", rsd_mat_name.c_str() );
	
	fprintf( fp, "NTEX=%d\n", textures.size() );
	for( int i=0; i<textures.size(); i++ ) {
		
		std::string texfile = makeLower( textures[i]->filename.c_str() );
		
		EnsureFileExtension( texfile, "tim" );
		
		fprintf( fp, "TEX[%d]=%s\n", i, texfile.c_str() );
	}
	
	fclose( fp );
	
	// Add paths
	path = file_name;
	StripFilename( path );
	std::string ply_file = path + rsd_ply_name;
	std::string mat_file = path + rsd_mat_name;
	
	// Write PLY
	fp = fopen( ply_file.c_str(), "w" );
	
	fputs( "# Created by SMXTOOL " VERSION " by Lameguy64\n", fp );
	fputs( "@PLY940102\n", fp );
	
	fprintf( fp, "%d %d %d\n", vertices.size(), normals.size(), primitives.size() );
	
	// Write vertices
	fputs( "# Vertices\n", fp );
	for( int i=0; i<vertices.size(); i++ ) {
		fprintf( fp, "%E %E %E\n", vertices[i].x, vertices[i].y, vertices[i].z );
	}
	
	// Write normals
	fputs( "# Normals\n", fp );
	for( int i=0; i<normals.size(); i++ ) {
		fprintf( fp, "%E %E %E\n", normals[i].x, normals[i].y, normals[i].z );
	}
	
	// Write primitives
	fputs( "# Primitives\n", fp );
	for( int i=0; i<primitives.size(); i++ ) {
		
		int type;
		
		switch( primitives[i].primtype ) {
			case PRIM_TRI_F:
			case PRIM_TRI_FT:
			case PRIM_TRI_G:
			case PRIM_TRI_GT:
				type = 0;
				break;
			case PRIM_QUAD_F:
			case PRIM_QUAD_FT:
			case PRIM_QUAD_G:
			case PRIM_QUAD_GT:
				type = 1;
				break;
		}
		
		fprintf( fp, "%d %d %d %d %d %d %d %d %d\n", type,
			primitives[i].v[0], primitives[i].v[1],
			primitives[i].v[2], primitives[i].v[3],
			primitives[i].n[0], primitives[i].n[1],
			primitives[i].n[2], primitives[i].n[3] );
		
	}
	
	fclose( fp );
	
	
	// Write MAT
	
	fp = fopen( mat_file.c_str(), "w" );
	
	if ( fp == nullptr ) {
		return ERR_INVALID;
	}
	
	fputs( "# Created by SMXTOOL " VERSION " by Lameguy64\n", fp );
	fputs( "@MAT940801\n", fp );
	
	fprintf( fp, "%d\n", primitives.size() );
	
	for( int i=0; i<primitives.size(); i++ ) {
		
		int flag;
		char shade;
		
		flag = 0;
		shade = 'F';
		
		if ( primitives[i].shadetype == SHADE_NONE ) {
			flag |= 0x1;
		} else if ( primitives[i].shadetype == SHADE_SMOOTH ) {
			shade = 'G';
		}
		
		if ( primitives[i].doublesided ) {
			flag |= 0x2;
		}
		
		if ( primitives[i].blendmode > 0 ) {
			flag |= 0x4;
			flag |= (primitives[i].blendmode-1)<<3;
		}
		
		fprintf( fp, "%d\t %d %c ", i, flag, shade );
		
		switch( primitives[i].primtype ) {
			case PRIM_TRI_F:
			case PRIM_QUAD_F:
				fprintf( fp, "C %d %d %d",
					primitives[i].c[0].r,
					primitives[i].c[0].g,
					primitives[i].c[0].b );
				break;
			case PRIM_TRI_G:
			case PRIM_QUAD_G:
				fprintf( fp, "G " );
				for( int j=0; j<4; j++ ) {
					fprintf( fp, "%d %d %d ",
						primitives[i].c[j].r, 
						primitives[i].c[j].g, 
						primitives[i].c[j].b );
				}
				break;
			case PRIM_TRI_FT:
			case PRIM_QUAD_FT:
				fprintf( fp, "D %d ", primitives[i].texnum );
				for( int j=0; j<4; j++ ) {
					fprintf( fp, "%d %d ",
						primitives[i].t[j].u, 
						primitives[i].t[j].v );
				}
				fprintf( fp, "%d %d %d",
					primitives[i].c[0].r,
					primitives[i].c[0].g,
					primitives[i].c[0].b );
				break;
			case PRIM_TRI_GT:
			case PRIM_QUAD_GT:
				fprintf( fp, "H %d ", primitives[i].texnum );
				for( int j=0; j<4; j++ ) {
					fprintf( fp, "%d %d ",
						primitives[i].t[j].u, 
						primitives[i].t[j].v );
				}
				for( int j=0; j<4; j++ ) {
					fprintf( fp, "%d %d %d ",
						primitives[i].c[j].r, 
						primitives[i].c[j].g, 
						primitives[i].c[j].b );
				}
				break;
		}
		
		fprintf( fp, "\n" );
		
	}
	
	fclose( fp );
	
	return ERR_NONE;
	
}

int ModelClass::MergeSmxFile(const char* file_name) {
	
	SmxClass smx_src;
	
	int err = smx_src.LoadFile( file_name );
	
	if ( err != ERR_NONE ) {
		return err;
	}
	
	// Append textures (if any)
	int textures_base = textures.size();
	
	for ( int i=0; i<smx_src.textures.size(); i++ ) {
		TextureClass* tex = new TextureClass;
		tex->filename = smx_src.textures[i]->filename;
#ifdef DEBUG
		std::cout << "Imported texture " << tex->filename << "." << std::endl;
#endif
		textures.push_back( tex );
	}
	
	// Append primitives
	for ( int p=0; p<smx_src.primitives.size(); p++ ) {
		
		PRIMITIVE pri;
		int counts;
		
		memcpy( &pri, &smx_src.primitives[p], sizeof(PRIMITIVE) );
		
		switch ( pri.primtype ) {
			case PRIM_TRI_F:
			case PRIM_TRI_FT:
			case PRIM_TRI_G:
			case PRIM_TRI_GT:
				counts = 3;
				break;
			case PRIM_QUAD_F:
			case PRIM_QUAD_FT:
			case PRIM_QUAD_G:
			case PRIM_QUAD_GT:
				counts = 4;
				break;
		}
		
		// Sort out vertices
		for ( int i=0; i<counts; i++ ) {
			
			int result = FindVertexIndex( &smx_src.vertices[pri.v[i]] );
			
			if ( result < 0 ) {
				
				vertices.push_back( smx_src.vertices[pri.v[i]] );
				pri.v[i] = vertices.size()-1;
				
			} else {
				
				pri.v[i] = result;
				
			}
			
		}
		
		// Sort out normals
		if ( pri.shadetype == SHADE_FLAT ) {
	
#ifdef DEBUG
			std::cout << "Normal index: " << std::to_string( pri.n[0] ) << std::endl;
#endif
			
			int result = FindNormalIndex( &smx_src.normals[pri.n[0]] );
			
			if ( result < 0 ) {
				
				normals.push_back( smx_src.normals[pri.n[0]] );
				pri.n[0] = vertices.size()-1;
				
			} else {
				
				pri.n[0] = result;
				
			}
			
		} else if ( pri.shadetype == SHADE_SMOOTH ) {
			
			for ( int i=0; i<counts; i++ ) {
				
#ifdef DEBUG
				std::cout << "Normal index: " << std::to_string( pri.n[i] ) << std::endl;
#endif

				int result = FindNormalIndex( &smx_src.normals[pri.n[i]] );

				if ( result < 0 ) {

					normals.push_back( smx_src.normals[pri.v[i]] );
					pri.n[i] = normals.size()-1;

				} else {

					pri.n[i] = result;

				}

			}	
			
		}
		
		// Sort out texture index
		if ( ( pri.primtype == PRIM_TRI_FT ) || ( pri.primtype == PRIM_TRI_GT ) ||
			( pri.primtype == PRIM_QUAD_FT ) || ( pri.primtype == PRIM_QUAD_GT ) ) {
			pri.texnum = textures_base+pri.texnum;
		}
		
		primitives.push_back( pri );
	}
	
	LoadTextures();
	
	return ERR_NONE;
}

void ModelClass::AddTexture(const char* file_name) {
	
#ifdef DEBUG
	std::cout << "Adding " << std::to_string( textures.size() ) 
		<< " texture(s)" << std::endl;
#endif
	
	int new_index = textures.size();
	
	if ( LoadTextureFile( file_name, new_index ) ) {
		
		fl_message_title( "Cannot add file" );
		fl_message( "Unable to add texture file." );
		return;
		
	}
	
	SmxClass::TextureClass* tex_item = new SmxClass::TextureClass();
	
	tex_item->filename = file_name;
	StripFileExtension( tex_item->filename );
	StripPathname( tex_item->filename );
	tex_item->userdata2 = texture_images[texture_images.size()-1];
	
	textures.push_back( tex_item );
	
}

int ModelClass::ReplaceTexture(int index, const char* file_name) {
	
	if ( LoadTextureFile( file_name, index ) ) {
		
		fl_message_title( "Cannot add file" );
		fl_message( "Unable to add texture file." );
		return 0;
		
	}
	
	textures[index]->filename = file_name;
	StripFileExtension( textures[index]->filename );
	StripPathname( textures[index]->filename );
	textures[index]->userdata2 = texture_images[texture_images.size()-1];
	
	return 1;
}

void ModelClass::DeleteTexture(int index) {
		
	for( int i=0; i<primitives.size(); i++ ) {

		if ( primitives[i].texnum == index ) {
			primitives[i].primtype &= ~SMX_PRIM_TEXTURED;
			primitives[i].texnum = 0;
		}
		
		if ( primitives[i].texnum > index ) {
			primitives[i].texnum--;
		}

	}
	
	delete textures[index];
	textures.erase( textures.begin()+index );
	
	for( int i=0; i<texture_images.size(); i++ ) {
		
		if ( texture_images[i]->index == index ) {
			
			delete texture_images[i];
			texture_images.erase( texture_images.begin()+i );
			break;
			
		}
		
	}
	
}

void ModelClass::LoadTextures( const char* src_file ) {
	
#ifdef DEBUG
	std::cout << "Loading " << std::to_string( textures.size() ) 
		<< " texture(s)" << std::endl;
#endif
	
	for( int i=0; i<textures.size(); i++ ) {
		if ( textures[i]->userdata2 == nullptr ) {
			
			std::string file;
			if ( src_file != nullptr ) {
				file = src_file;
				StripFilename( file );
				file += textures[i]->filename;
			} else {
				file = textures[i]->filename;
			}
			
			if ( GetExtension( file.c_str() ) == nullptr ) {
				file += ".tim";
			}
			
#ifdef DEBUG
			std::cout << "Load texture file: " << file << std::endl;
#endif
			
			if( LoadTextureFile( file.c_str(), i ) != 0 ) {
				
				fl_message_title( "Texture File Not Found" );
				if( fl_choice( "Unable to load texture file\n%s", 
					"Ignore", "Locate", 0, 
					textures[i]->filename.c_str() ) ) {
					
					Fl_Native_File_Chooser file_chooser;
	
					file_chooser.title( "Locate Texture File" );
					file_chooser.filter( "Texture Image File\t*.tim\n" );
					file_chooser.type( file_chooser.BROWSE_FILE );
					
					if( file_chooser.show() == 0 ) {
						
						textures[i]->filename = file_chooser.filename();
						
						if( LoadTextureFile( file_chooser.filename(), i ) != 0 ) {
							
							fl_message_title( "Error Loading Texture" );
							fl_message( "Cannot load texture file." );
							
						} else {
							
							textures[i]->filename = file_chooser.filename();
							StripPathname( textures[i]->filename );
							StripFileExtension( textures[i]->filename );
							
							textures[i]->userdata2 = texture_images[texture_images.size()-1];
							
						}
						
					}
					
				}
				
			} else {
				
				textures[i]->userdata2 = texture_images[texture_images.size()-1];
				
			}
		}
	}
}

int ModelClass::LoadTextureFile(const char* file_name, int index) {
	
	TexImageClass* tex = new TexImageClass;
#ifdef DEBUG
	std::cout << "Texture " << file_name << "... ";
#endif
	
	if ( tex->LoadTexture( file_name ) != 0 ) {
		
		delete tex;
		
#ifdef DEBUG
		std::cout << "Load fail." << std::endl;
#endif
				
		return -1;
	}
	
#ifdef DEBUG
	std::cout << "Load ok." << std::endl;
#endif
	tex->index = index;
	
	texture_images.push_back( tex );
	
	return 0;
}

void ModelClass::RenderSelected() {
	
	if ( vertices.empty() ) {
		return;
	}
	
	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f( 1.f, 1.f, 0.f, 1.f );
	
	glLineWidth( 3.f );
	
	glBegin( GL_LINES );
	
    for ( int i=0; i<primitives.size(); i++ ) {
		
		SmxClass::PRIMITIVE *p = &primitives[i];
		
		if ( p->selected ) {
			switch ( primitives[i].primtype ) {
				case SmxClass::PRIM_TRI_F:
				case SmxClass::PRIM_TRI_FT:
				case SmxClass::PRIM_TRI_G:
				case SmxClass::PRIM_TRI_GT:

					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z );
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z);
					
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z);
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );
					
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );
					
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z );
					break;

				case SmxClass::PRIM_QUAD_F:
				case SmxClass::PRIM_QUAD_FT:
				case SmxClass::PRIM_QUAD_G:
				case SmxClass::PRIM_QUAD_GT:

					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z );
					
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z );
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );
					
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );
					
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );

					break;
			}
		}
    }
	
    glEnd();
	
	glLineWidth( 1.f );
	
}

void ModelClass::RenderWireframe() {
	
	if ( vertices.empty() )
	{
		return;
	}
	
	glBegin( GL_LINES );
	
    for ( int i=0; i<primitives.size(); i++ )
    {
		SmxClass::PRIMITIVE *p = &primitives[i];
		
		switch(primitives[i].primtype)
		{
			case SmxClass::PRIM_TRI_F:
			case SmxClass::PRIM_TRI_G:
			case SmxClass::PRIM_TRI_FT:
			case SmxClass::PRIM_TRI_GT:
				
				glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
					-vertices[p->v[0]].z );
				glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
					-vertices[p->v[1]].z);
				
				glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
					-vertices[p->v[1]].z);
				glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
					-vertices[p->v[2]].z );
				
				glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
					-vertices[p->v[2]].z );
				glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
					-vertices[p->v[0]].z );
				break;
				
			case SmxClass::PRIM_QUAD_F:
			case SmxClass::PRIM_QUAD_G:
			case SmxClass::PRIM_QUAD_FT:
			case SmxClass::PRIM_QUAD_GT:

				glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
					-vertices[p->v[0]].z);
				glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
					-vertices[p->v[1]].z );
				
				glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
					-vertices[p->v[1]].z );
				glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
					-vertices[p->v[3]].z );

				glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
					-vertices[p->v[3]].z );
				glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
					-vertices[p->v[2]].z );
				
				glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
					-vertices[p->v[2]].z );
				glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
					-vertices[p->v[0]].z);
				
				break;
		}
    }
	
    glEnd();
}

void ModelClass::RenderTextured(int culling) {
	
	if ( vertices.empty() )
	{
		return;
	}
	
	
	float texcol_div = 127.f;
	float alpha;
	
	if ( ui->glviewport->shader.Initialized() ) {
		texcol_div = 255.f;
	}
	
	/*if ( shader.Initialized() )
	{
		texcol_div = 255.f;
		shader.ActivateShader();
		shader.TextureEnable( false );
	}*/
	
	// Parse untextured polygons (flat and gouraud)
	
	ui->glviewport->shader.ActivateShader();
	
    for ( int i=primitives.size()-1; i>=0; i-- )
    {
		SmxClass::PRIMITIVE *p = &primitives[i];
		
		switch( p->blendmode ) {
			case 0:	// No blending
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				alpha = 1.0f;
				break;
			case 1:	// 50% + 50%
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				alpha = 0.5f;
				break;
			case 2: // 100% + 100%
				glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				alpha = 1.0f;
				break;
			case 3: // 100% + 50%
				glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				alpha = 0.5f;
				break;
			case 4: // 100% - 100%
				glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );
				alpha = 1.f;
				break;
		}
		
		if ( culling ) {
			if ( primitives[i].doublesided ) {
				glDisable( GL_CULL_FACE );
			} else {
				glEnable( GL_CULL_FACE );
			}
		} else {
			glDisable( GL_CULL_FACE );
		}
		
		
		if ( ( primitives[i].primtype != SmxClass::PRIM_TRI_FT ) &&
			( primitives[i].primtype != SmxClass::PRIM_TRI_GT ) &&
			( primitives[i].primtype != SmxClass::PRIM_QUAD_FT ) &&
			( primitives[i].primtype != SmxClass::PRIM_QUAD_GT ) ) {
			
			glBindTexture( GL_TEXTURE_2D, 0 );
			ui->glviewport->shader.TextureEnable( false );
			
			glBegin( GL_TRIANGLES );

			switch(primitives[i].primtype)
			{
				case SmxClass::PRIM_TRI_F:
				case SmxClass::PRIM_TRI_G:

					glColor4f( p->c[0].r/255.f, p->c[0].g/255.f, p->c[0].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z );

					glColor4f( p->c[1].r/255.f, p->c[1].g/255.f, p->c[1].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[1]].x, -normals[p->n[1]].y, 
						-normals[p->n[1]].z );*/
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z);

					glColor4f( p->c[2].r/255.f, p->c[2].g/255.f, p->c[2].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[2]].x, -normals[p->n[2]].y, 
						-normals[p->n[2]].z );*/
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );

					break;

				case SmxClass::PRIM_QUAD_F:
				case SmxClass::PRIM_QUAD_G:

					glColor4f( p->c[0].r/255.f, p->c[0].g/255.f, p->c[0].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);
					glColor4f( p->c[1].r/255.f, p->c[1].g/255.f, p->c[1].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[1]].x, -normals[p->n[1]].y, 
						-normals[p->n[1]].z );*/
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z );
					glColor4f( p->c[3].r/255.f, p->c[3].g/255.f, p->c[3].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[3]].x, -normals[p->n[3]].y, 
						-normals[p->n[3]].z );*/
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );

					glColor4f( p->c[3].r/255.f, p->c[3].g/255.f, p->c[3].b/255.f, 
						alpha );
					/*glNormal3f( normals[p->n[3]].x, -normals[p->n[3]].y, 
						-normals[p->n[3]].z );*/
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );
					glColor4f( p->c[2].r/255.f, p->c[2].g/255.f, p->c[2].b/255.f,
						alpha );
					/*glNormal3f( normals[p->n[2]].x, -normals[p->n[2]].y, 
						-normals[p->n[2]].z );*/
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );
					glColor4f( p->c[0].r/255.f, p->c[0].g/255.f, p->c[0].b/255.f,
						alpha );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);

					break;
			}

			glEnd();
			
		} else {
			
			if ( textures[p->texnum]->userdata2 == nullptr ) {
				continue;
			}
			
			TexImageClass* tex = (TexImageClass*)textures[p->texnum]->userdata2;
			glBindTexture( GL_TEXTURE_2D, tex->gl_texture );
			ui->glviewport->shader.TextureEnable( true );
			
			glBegin( GL_TRIANGLES );
			
			switch ( primitives[i].primtype )
			{
				case SmxClass::PRIM_TRI_FT:
				case SmxClass::PRIM_TRI_GT:
					
					glColor4f( p->c[0].r/texcol_div, p->c[0].g/texcol_div, 
						p->c[0].b/texcol_div, alpha );
					glTexCoord2f( (float)p->t[0].u/(tex->w-1), 
						(float)p->t[0].v/(tex->h-1) );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z );

					glColor4f( p->c[1].r/texcol_div, p->c[1].g/texcol_div, 
						p->c[1].b/texcol_div, alpha );
					glTexCoord2f( (float)p->t[1].u/(tex->w-1), 
						(float)p->t[1].v/(tex->h-1) );
					/*glNormal3f( normals[p->n[1]].x, -normals[p->n[1]].y, 
						-normals[p->n[1]].z );*/
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z);

					glColor4f( p->c[2].r/texcol_div, p->c[2].g/texcol_div, 
						p->c[2].b/texcol_div, alpha );
					glTexCoord2f( (float)p->t[2].u/(tex->w-1), 
						(float)p->t[2].v/(tex->h-1) );
					/*glNormal3f( normals[p->n[2]].x, -normals[p->n[2]].y, 
						-normals[p->n[2]].z );*/
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );

					break;

				case SmxClass::PRIM_QUAD_FT:
				case SmxClass::PRIM_QUAD_GT:

					// 1st
					glTexCoord2f( (float)p->t[0].u/(tex->w-1), 
						(float)p->t[0].v/(tex->h-1) );
					glColor4f( p->c[0].r/texcol_div, p->c[0].g/texcol_div, 
						p->c[0].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);
					
					glTexCoord2f( (float)p->t[1].u/(tex->w-1), 
						(float)p->t[1].v/(tex->h-1) );
					glColor4f( p->c[1].r/texcol_div, p->c[1].g/texcol_div, 
						p->c[1].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[1]].x, -normals[p->n[1]].y, 
						-normals[p->n[1]].z );*/
					glVertex3f( vertices[p->v[1]].x, -vertices[p->v[1]].y, 
						-vertices[p->v[1]].z );
					
					glTexCoord2f( (float)p->t[3].u/(tex->w-1),
						(float)p->t[3].v/(tex->h-1) );
					glColor4f( p->c[3].r/texcol_div, p->c[3].g/texcol_div, 
						p->c[3].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[3]].x, -normals[p->n[3]].y, 
						-normals[p->n[3]].z );*/
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );

					// 2nd
					glTexCoord2f( (float)p->t[3].u/(tex->w-1), 
						(float)p->t[3].v/(tex->h-1) );
					glColor4f( p->c[3].r/texcol_div, p->c[3].g/texcol_div, 
						p->c[3].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[3]].x, -normals[p->n[3]].y, 
						-normals[p->n[3]].z );*/
					glVertex3f( vertices[p->v[3]].x, -vertices[p->v[3]].y, 
						-vertices[p->v[3]].z );
					
					glTexCoord2f( (float)p->t[2].u/(tex->w-1), 
						(float)p->t[2].v/(tex->h-1) );
					glColor4f( p->c[2].r/texcol_div, p->c[2].g/texcol_div, 
						p->c[2].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[2]].x, -normals[p->n[2]].y, 
						-normals[p->n[2]].z );*/
					glVertex3f( vertices[p->v[2]].x, -vertices[p->v[2]].y, 
						-vertices[p->v[2]].z );
					
					glTexCoord2f( (float)p->t[0].u/(tex->w-1), 
						(float)p->t[0].v/(tex->h-1) );
					glColor4f( p->c[0].r/texcol_div, p->c[0].g/texcol_div, 
						p->c[0].b/texcol_div, alpha );
					/*glNormal3f( normals[p->n[0]].x, -normals[p->n[0]].y, 
						-normals[p->n[0]].z );*/
					glVertex3f( vertices[p->v[0]].x, -vertices[p->v[0]].y, 
						-vertices[p->v[0]].z);

					break;
			}
			
			glEnd();
			
		}
    }
	
	ui->glviewport->shader.DeactivateShader();
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	
}

void ModelClass::SetShading(SmxClass::PRIMITIVE* prim, int shaded) {
	
	if ( shaded ) {	// Make gouraud
	
		if ( prim->primtype & SMX_PRIM_GOURAUD ) {
			return;
		}
		
		prim->primtype |= SMX_PRIM_GOURAUD;
		
		prim->c[1] = prim->c[0];
		prim->c[2] = prim->c[0];
		prim->c[3] = prim->c[0];
		
	} else {		// Make flat
		
		if ( !( prim->primtype & SMX_PRIM_GOURAUD ) ) {
			return;
		}
		
		prim->primtype &= ~SMX_PRIM_GOURAUD;
		
		prim->c[1] = prim->c[0];
		prim->c[2] = prim->c[0];
		prim->c[3] = prim->c[0];

	}
}

void ModelClass::SetTexturing(SmxClass::PRIMITIVE* prim, int textured, int texnum) {
	
	if ( textured ) {
		
		if ( prim->primtype & SMX_PRIM_TEXTURED ) {
			return;
		}
		
		prim->primtype |= SMX_PRIM_TEXTURED;
		prim->texnum = 0;
		
	} else {
		
		if ( !( prim->primtype & SMX_PRIM_TEXTURED ) ) {
			return;
		}
		
		prim->primtype &= ~SMX_PRIM_TEXTURED;
					
	}
	
}

int ModelClass::ScanFace( SmxClass::VECTOR3D pos, SmxClass::VECTOR3D ray ) {
	
	typedef struct {
		int index;
		float dist;
	} FACE_ENTRY;
	
	int num_valid_faces = 0;
	FACE_ENTRY face_dist[primitives.size()];
	SmxClass::VECTOR3D verts[4];
	
	Normalize( (float*)&ray, 3 );

	for ( int i=0; i<primitives.size(); i++ ) {
		
		if ( !( primitives[i].primtype & SMX_PRIM_QUAD ) ) {

			verts[0] = vertices[primitives[i].v[0]];
			r_translate( &verts[0], pos );
			verts[1] = vertices[primitives[i].v[1]];
			r_translate( &verts[1], pos );
			verts[2] = vertices[primitives[i].v[2]];
			r_translate( &verts[2], pos );

			face_dist[num_valid_faces].index = i;
			face_dist[num_valid_faces].dist = RayIntersectsTriangle( 
				&ray, &verts[0], &verts[1], &verts[2] );
			
			num_valid_faces++;
			
		} else if ( primitives[i].primtype & SMX_PRIM_QUAD ) {

			verts[0] = vertices[primitives[i].v[0]];
			r_translate( &verts[0], pos );
			verts[1] = vertices[primitives[i].v[1]];
			r_translate( &verts[1], pos );
			verts[2] = vertices[primitives[i].v[2]];
			r_translate( &verts[2], pos );
			verts[3] = vertices[primitives[i].v[3]];
			r_translate( &verts[3], pos );
			
			face_dist[num_valid_faces].index = i;
			face_dist[num_valid_faces].dist = RayIntersectsQuad( 
				&ray, &verts[0], &verts[1], &verts[2], &verts[3] );
			
			num_valid_faces++;
		}
			
	}

	float closestval = 0xffffffff;
	int closestindex = -1;

	for ( int i=0; i<num_valid_faces; i++ )
	{
		if ( face_dist[i].dist < closestval )
		{
			closestval = face_dist[i].dist;
			closestindex = face_dist[i].index;
		}
	}

	return closestindex;
}

void ModelClass::ClearTextures() {
	if ( texture_images.size() > 0 ) {
		for ( int i=0; i<texture_images.size(); i++ ) {
			delete texture_images[i];
		}
		texture_images.clear();
	}
#ifdef DEBUG
	std::cout << "Textures cleared." << std::endl;
#endif
}

void ModelClass::ClearSelection() {
	for ( int i=0; i<primitives.size(); i++ ) {
		primitives[i].selected = 0;
	}
}