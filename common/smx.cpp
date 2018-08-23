#include <ctype.h>
#include <tinyxml2.h>
#include <iostream>
#include "smx.h"

static int casecmp( const char *a, const char *b )
{

	int ca,cb;

	do
	{

		ca = (unsigned char) *a++;
		cb = (unsigned char) *b++;
		ca = tolower(toupper(ca));
		cb = tolower(toupper(cb));

	}
	while (ca == cb && ca != '\0');

	return ca-cb;

}


SmxClass::SmxClass()
{
}

SmxClass::~SmxClass()
{
	Clear();
	
	for ( int i=0; i<textures.size(); i++ )
	{
		delete textures[i];
	}
}

void SmxClass::Clear()
{
	SmxClass::vertices.clear();
	SmxClass::normals.clear();
	SmxClass::primitives.clear();
	
	for ( int i=0; i<SmxClass::textures.size(); i++ )
	{
		delete SmxClass::textures[i];
	}
	
	SmxClass::textures.clear();
}

SmxClass::SMX_ERROR SmxClass::LoadFile( const char *filename )
{

    tinyxml2::XMLDocument	smxFile;
    tinyxml2::XMLElement	*modelElement;
	int ret;


    if ( ( ret = smxFile.LoadFile( filename ) ) != tinyxml2::XML_SUCCESS )
	{

		if ( ret == tinyxml2::XML_ERROR_FILE_NOT_FOUND )
		{
			return SmxClass::ERR_NOTFOUND;
		}
		else
		{
			return ERR_INVALID;
		}

    }

	modelElement = smxFile.FirstChildElement("model");

	if (modelElement == nullptr)
	{
		return SmxClass::ERR_NO_MODEL;
	}

	// Clears everything
	Clear();

	// Read vertices
	if ( modelElement->FirstChildElement( "vertices" ) != nullptr )
	{

		tinyxml2::XMLElement* vElement = modelElement->FirstChildElement( 
			"vertices" );

		vElement = vElement->FirstChildElement("v");
        while ( vElement != nullptr )
		{

			SmxClass::VECTOR3D vertex;
			
			vertex.x = vElement->FloatAttribute( "x" );
			vertex.y = vElement->FloatAttribute( "y" );
			vertex.z = vElement->FloatAttribute( "z" );
			
			SmxClass::vertices.push_back( vertex );

			vElement = vElement->NextSiblingElement( "v" );

        }

	}
	else
	{

		return SmxClass::ERR_NO_VERTS;

	}

	// Read normals
	if ( modelElement->FirstChildElement( "normals" ) != nullptr )
	{

		tinyxml2::XMLElement* nElement = modelElement->FirstChildElement("normals");

		nElement = nElement->FirstChildElement("v");
        while ( nElement != nullptr )
		{

			SmxClass::VECTOR3D normal;
			
			normal.x = nElement->FloatAttribute( "x" );
			normal.y = nElement->FloatAttribute( "y" );
			normal.z = nElement->FloatAttribute( "z" );
			SmxClass::normals.push_back( normal );

			nElement = nElement->NextSiblingElement( "v" );

        }

	}
	/*
	else
	{

		return SmxClass::ERR_NO_NORMS;

	}*/

	// Read texture files
	if (modelElement->FirstChildElement("textures") != nullptr) {

		tinyxml2::XMLElement* tElement = modelElement->FirstChildElement("textures");
		
        tElement = tElement->FirstChildElement("texture");
		
        while ( tElement != nullptr )
		{

			if ( tElement->Attribute( "file" ) != NULL )
			{
				SmxClass::TextureClass *texture = new TextureClass;
				
				texture->filename = tElement->Attribute( "file" );
				texture->userdata1 = 0;
				texture->userdata2 = nullptr;
				
				textures.push_back( texture );
			}

			tElement = tElement->NextSiblingElement( "texture" );

        }

	}

	// Read primitives
	if ( modelElement->FirstChildElement( "primitives" ) != nullptr )
	{

		tinyxml2::XMLElement* pElement = modelElement->FirstChildElement( 
			"primitives" );

		if ( pElement == nullptr )
		{
			return SmxClass::ERR_NO_PRIMS;
		}
		
		pElement = pElement->FirstChildElement("poly");

		while ( pElement != NULL )
		{
			const char* primType = pElement->Attribute("type");
			SMX_PRIMTYPE primCode = SmxClass::PRIM_TRI_F;

			SmxClass::PRIMITIVE prim;
			
			prim.texnum = -1;

            if ( pElement->Attribute( "shading" ) != nullptr )
			{

				if ( casecmp( "F", pElement->Attribute("shading") ) == 0 )
				{
					prim.shadetype = SmxClass::SHADE_FLAT;
				}
				else if ( casecmp( "S", pElement->Attribute("shading") ) == 0 )
				{
					prim.shadetype = SmxClass::SHADE_SMOOTH;
				}
				else
				{
					prim.shadetype = SmxClass::SHADE_FLAT;
				}

            } else {

            	prim.shadetype = SmxClass::SHADE_NONE;

            }
			
			if ( pElement->Attribute( "blend" ) != nullptr )
			{
				
				prim.blendmode = pElement->IntAttribute( "blend" );
				
			} else {
				
				prim.blendmode = 0;
				
			}
			
			if ( pElement->Attribute( "double" ) != nullptr )
			{
			
				prim.doublesided = pElement->IntAttribute( "double" );
			
			} else {
				
				prim.doublesided = 0;
				
			}

            if ( casecmp( "F3", primType ) == 0 )
			{
				primCode = SmxClass::PRIM_TRI_F;
            }
			else if ( casecmp( "FT3", primType ) == 0 )
			{
            	primCode = SmxClass::PRIM_TRI_FT;
            }
			else if ( casecmp( "G3", primType ) == 0 )
			{
            	primCode = SmxClass::PRIM_TRI_G;
            }
			else if ( casecmp( "GT3", primType ) == 0)
			{
            	primCode = SmxClass::PRIM_TRI_GT;
            }
			else if ( casecmp( "F4", primType ) == 0 )
			{
				primCode = SmxClass::PRIM_QUAD_F;
            }
			else if ( casecmp( "FT4", primType ) == 0 )
			{
            	primCode = SmxClass::PRIM_QUAD_FT;
            }
			else if ( casecmp( "G4", primType ) == 0 )
			{
            	primCode = SmxClass::PRIM_QUAD_G;
            }
			else if ( casecmp( "GT4", primType ) == 0 )
			{
            	primCode = SmxClass::PRIM_QUAD_GT;
            }

            prim.primtype = primCode;

            if  ( ( primCode == SmxClass::PRIM_TRI_F ) || 
				( primCode == SmxClass::PRIM_TRI_FT ) || 
				( primCode == SmxClass::PRIM_TRI_G ) || 
				( primCode == SmxClass::PRIM_TRI_GT ) )
			{

				prim.v[0] = pElement->IntAttribute( "v0" );
                prim.v[1] = pElement->IntAttribute( "v1" );
                prim.v[2] = pElement->IntAttribute( "v2" );

				if ( prim.shadetype == SmxClass::SHADE_FLAT )
				{
					prim.n[0] = pElement->IntAttribute( "n0" );
					prim.n[1] = pElement->IntAttribute( "n0" );
					prim.n[2] = pElement->IntAttribute( "n0" );
				}
				else
				{
					prim.n[0] = pElement->IntAttribute( "n0" );
					prim.n[1] = pElement->IntAttribute( "n1" );
					prim.n[2] = pElement->IntAttribute( "n2" );
				}

                if ( ( primCode == SmxClass::PRIM_TRI_F ) || 
					( primCode == SmxClass::PRIM_TRI_FT ) || 
					( primCode == SmxClass::PRIM_TRI_G ) || 
					( primCode == SmxClass::PRIM_TRI_GT ) )
				{

					prim.c[0].r = pElement->IntAttribute( "r0" );
					prim.c[0].g = pElement->IntAttribute( "g0" );
					prim.c[0].b = pElement->IntAttribute( "b0" );

					prim.c[1] = prim.c[0];
					prim.c[2] = prim.c[0];

					if ( ( primCode == SmxClass::PRIM_TRI_G ) || 
						( primCode == SmxClass::PRIM_TRI_GT ) )
					{

						prim.c[1].r = pElement->IntAttribute( "r1" );
						prim.c[1].g = pElement->IntAttribute( "g1" );
						prim.c[1].b = pElement->IntAttribute( "b1" );

						prim.c[2].r = pElement->IntAttribute( "r2" );
						prim.c[2].g = pElement->IntAttribute( "g2" );
						prim.c[2].b = pElement->IntAttribute( "b2" );

					}

					if ( ( primCode == SmxClass::PRIM_TRI_FT ) || 
						( primCode == SmxClass::PRIM_TRI_GT ) )
					{

						prim.texnum = pElement->IntAttribute( "texture" );

						prim.t[0].u = pElement->IntAttribute( "tu0" );
						prim.t[0].v = pElement->IntAttribute( "tv0" );

						prim.t[1].u = pElement->IntAttribute( "tu1" );
						prim.t[1].v = pElement->IntAttribute( "tv1" );

						prim.t[2].u = pElement->IntAttribute( "tu2" );
						prim.t[2].v = pElement->IntAttribute( "tv2" );

					}

				}

			}
			else if ( ( primCode == SmxClass::PRIM_QUAD_F ) || 
				( primCode == SmxClass::PRIM_QUAD_FT ) || 
				( primCode == SmxClass::PRIM_QUAD_G ) || 
				( primCode == SmxClass::PRIM_QUAD_GT ) )
			{

				prim.v[0] = pElement->IntAttribute( "v0" );
                prim.v[1] = pElement->IntAttribute( "v1" );
                prim.v[2] = pElement->IntAttribute( "v2" );
                prim.v[3] = pElement->IntAttribute( "v3" );

                if ( prim.shadetype == SmxClass::SHADE_FLAT )
				{

					prim.n[0] = pElement->IntAttribute( "n0" );
					prim.n[1] = prim.n[0];
					prim.n[2] = prim.n[0];
					prim.n[3] = prim.n[0];

                } else {

					prim.n[0] = pElement->IntAttribute( "n0" );
					prim.n[1] = pElement->IntAttribute( "n1" );
					prim.n[2] = pElement->IntAttribute( "n2" );
					prim.n[3] = pElement->IntAttribute( "n3" );

                }

                if ( ( primCode == SmxClass::PRIM_QUAD_F ) ||
					( primCode == SmxClass::PRIM_QUAD_FT ) || 
					( primCode == SmxClass::PRIM_QUAD_G ) ||
					( primCode == SmxClass::PRIM_QUAD_GT ) )
				{

					prim.c[0].r = pElement->IntAttribute( "r0" );
					prim.c[0].g = pElement->IntAttribute( "g0" );
					prim.c[0].b = pElement->IntAttribute( "b0" );

					prim.c[1] = prim.c[0];
					prim.c[2] = prim.c[0];
					prim.c[3] = prim.c[0];

					if ( ( primCode == SmxClass::PRIM_QUAD_G ) ||
						( primCode == SmxClass::PRIM_QUAD_GT ) )
					{

						prim.c[1].r = pElement->IntAttribute( "r1" );
						prim.c[1].g = pElement->IntAttribute( "g1" );
						prim.c[1].b = pElement->IntAttribute( "b1" );

						prim.c[2].r = pElement->IntAttribute( "r2" );
						prim.c[2].g = pElement->IntAttribute( "g2" );
						prim.c[2].b = pElement->IntAttribute( "b2" );

						prim.c[3].r = pElement->IntAttribute( "r3" );
						prim.c[3].g = pElement->IntAttribute( "g3" );
						prim.c[3].b = pElement->IntAttribute( "b3" );

					}

					if ( ( primCode == SmxClass::PRIM_QUAD_FT ) || 
						( primCode == SmxClass::PRIM_QUAD_GT ) )
					{

						prim.texnum = pElement->IntAttribute( "texture");

						prim.t[0].u = pElement->IntAttribute( "tu0" );
						prim.t[0].v = pElement->IntAttribute( "tv0" );
						
						prim.t[1].u = pElement->IntAttribute( "tu1" );
						prim.t[1].v = pElement->IntAttribute( "tv1" );
						
						prim.t[2].u = pElement->IntAttribute( "tu2" );
						prim.t[2].v = pElement->IntAttribute( "tv2" );
						
						prim.t[3].u = pElement->IntAttribute( "tu3" );
						prim.t[3].v = pElement->IntAttribute( "tv3" );

					}

				}

			}

			prim.selected = 0;
			
            SmxClass::primitives.push_back( prim );

            pElement = pElement->NextSiblingElement( "poly" );

		}

	}

#ifdef DEBUG
	printf( "SMX model stats:\n" );
	printf( " VERTS = %d\n", SmxClass::vertices.size() );
	printf( " NORMS = %d\n", SmxClass::normals.size() );
	printf( " PRIMS = %d\n", SmxClass::primitives.size() );
#endif

	return SmxClass::ERR_NONE;

}

SmxClass::SMX_ERROR SmxClass::SaveFile( const char *filename ) {
	
	tinyxml2::XMLDocument document;
	
	tinyxml2::XMLElement* base = document.NewElement( "model" );
	base->SetAttribute( "version", 1 );
	
	// Write vertices
	tinyxml2::XMLElement* o = document.NewElement( "vertices" );
	tinyxml2::XMLElement* oo;
	
	o->SetAttribute( "count", (int)vertices.size() );
	
	for ( size_t i=0; i<vertices.size(); i++ ) { 
		oo = document.NewElement( "v" );
		oo->SetAttribute( "x", vertices[i].x );
		oo->SetAttribute( "y", vertices[i].y );
		oo->SetAttribute( "z", vertices[i].z );
		o->InsertEndChild( oo );
	}
	
	base->InsertEndChild( o );
	
	// Write normals
	o = document.NewElement( "normals" );
	o->SetAttribute( "count", (int)normals.size() );
	
	for ( size_t i=0; i<normals.size(); i++ ) { 
		oo = document.NewElement( "v" );
		oo->SetAttribute( "x", normals[i].x );
		oo->SetAttribute( "y", normals[i].y );
		oo->SetAttribute( "z", normals[i].z );
		o->InsertEndChild( oo );
	}
	
	base->InsertEndChild( o );
	
	// Textures
	o = document.NewElement( "textures" );
	o->SetAttribute( "count", (int)textures.size() );
	
	for ( size_t i=0; i<textures.size(); i++ ) {
		oo = document.NewElement( "texture" );
		oo->SetAttribute( "file", textures[i]->filename.c_str() );
		o->InsertEndChild( oo );
	}
	
	base->InsertEndChild( o );
	
	// Primitives
	
	o = document.NewElement( "primitives" );
	o->SetAttribute( "count", (int)primitives.size() );
	
	for ( size_t i=0; i<primitives.size(); i++ ) {
		
		PRIMITIVE* p = &primitives[i];
		
		oo = document.NewElement( "poly" );
		
		// Set vertex indices
		oo->SetAttribute( "v0", p->v[0] );
		oo->SetAttribute( "v1", p->v[1] );
		oo->SetAttribute( "v2", p->v[2] );
		if ( ( p->primtype == PRIM_QUAD_F ) || ( p->primtype == PRIM_QUAD_FT ) ||
			( p->primtype == PRIM_QUAD_G ) || ( p->primtype == PRIM_QUAD_GT ) ) {
		
			oo->SetAttribute( "v3", p->v[3] );
		
		}
		
		// Set normal indices
		if ( p->shadetype == SHADE_SMOOTH ) {
			
			oo->SetAttribute( "n0", p->n[0] );
			oo->SetAttribute( "n1", p->n[1] );
			oo->SetAttribute( "n2", p->n[2] );
			
			if ( ( p->primtype == PRIM_QUAD_F ) || ( p->primtype == PRIM_QUAD_FT ) ||
				( p->primtype == PRIM_QUAD_G ) || ( p->primtype == PRIM_QUAD_GT ) ) {
				oo->SetAttribute( "n3", p->n[3] );
			}
			
			oo->SetAttribute( "shading", "S" );
		
		} else if ( p->shadetype == SHADE_FLAT ) {
		
			oo->SetAttribute( "n0", p->n[0] );
			oo->SetAttribute( "shading", "F" );
		
		}
		
		if ( p->blendmode ) {
			oo->SetAttribute( "blend", p->blendmode );
		}
		
		if ( p->doublesided ) {
			oo->SetAttribute( "double", p->doublesided );
		}
		
		// Set color values
		switch( p->primtype ) {
			case PRIM_TRI_F:
			case PRIM_QUAD_F:
			case PRIM_TRI_FT:
			case PRIM_QUAD_FT:
				oo->SetAttribute( "r0", p->c[0].r );
				oo->SetAttribute( "g0", p->c[0].g );
				oo->SetAttribute( "b0", p->c[0].b );
				break;
			case PRIM_TRI_G:
			case PRIM_TRI_GT:
				oo->SetAttribute( "r0", p->c[0].r );
				oo->SetAttribute( "g0", p->c[0].g );
				oo->SetAttribute( "b0", p->c[0].b );
				
				oo->SetAttribute( "r1", p->c[1].r );
				oo->SetAttribute( "g1", p->c[1].g );
				oo->SetAttribute( "b1", p->c[1].b );
				
				oo->SetAttribute( "r2", p->c[2].r );
				oo->SetAttribute( "g2", p->c[2].g );
				oo->SetAttribute( "b2", p->c[2].b );
				break;
			case PRIM_QUAD_G:
			case PRIM_QUAD_GT:
				oo->SetAttribute( "r0", p->c[0].r );
				oo->SetAttribute( "g0", p->c[0].g );
				oo->SetAttribute( "b0", p->c[0].b );
				
				oo->SetAttribute( "r1", p->c[1].r );
				oo->SetAttribute( "g1", p->c[1].g );
				oo->SetAttribute( "b1", p->c[1].b );
				
				oo->SetAttribute( "r2", p->c[2].r );
				oo->SetAttribute( "g2", p->c[2].g );
				oo->SetAttribute( "b2", p->c[2].b );
				
				oo->SetAttribute( "r3", p->c[3].r );
				oo->SetAttribute( "g3", p->c[3].g );
				oo->SetAttribute( "b3", p->c[3].b );
				break;
		}
		
		// Set texture coordinates
		if ( ( p->primtype == PRIM_TRI_FT ) || ( p->primtype == PRIM_TRI_GT ) ) {
			oo->SetAttribute( "texture", p->texnum );
			oo->SetAttribute( "tu0", p->t[0].u );
			oo->SetAttribute( "tv0", p->t[0].v );
			oo->SetAttribute( "tu1", p->t[1].u );
			oo->SetAttribute( "tv1", p->t[1].v );
			oo->SetAttribute( "tu2", p->t[2].u );
			oo->SetAttribute( "tv2", p->t[2].v );
		} else if ( ( p->primtype == PRIM_QUAD_FT ) || ( p->primtype == PRIM_QUAD_GT ) ) {
			oo->SetAttribute( "texture", p->texnum );
			oo->SetAttribute( "tu0", p->t[0].u );
			oo->SetAttribute( "tv0", p->t[0].v );
			oo->SetAttribute( "tu1", p->t[1].u );
			oo->SetAttribute( "tv1", p->t[1].v );
			oo->SetAttribute( "tu2", p->t[2].u );
			oo->SetAttribute( "tv2", p->t[2].v );
			oo->SetAttribute( "tu3", p->t[3].u );
			oo->SetAttribute( "tv3", p->t[3].v );
		}
		
		switch ( p->primtype ) {
			case PRIM_TRI_F:
				oo->SetAttribute( "type", "F3" );
				break;
			case PRIM_TRI_FT:
				oo->SetAttribute( "type", "FT3" );
				break;
			case PRIM_TRI_G:
				oo->SetAttribute( "type", "G3" );
				break;
			case PRIM_TRI_GT:
				oo->SetAttribute( "type", "GT3" );
				break;
			case PRIM_QUAD_F:
				oo->SetAttribute( "type", "F4" );
				break;
			case PRIM_QUAD_FT:
				oo->SetAttribute( "type", "FT4" );
				break;
			case PRIM_QUAD_G:
				oo->SetAttribute( "type", "G4" );
				break;
			case PRIM_QUAD_GT:
				oo->SetAttribute( "type", "GT4" );
				break;
		}
		
		o->InsertEndChild( oo );
	}
	
	base->InsertEndChild( o );
	
	document.InsertEndChild( base );
	
	if ( document.SaveFile( filename ) != tinyxml2::XML_SUCCESS )
	{
		return ERR_INVALID;
	}
	
	return ERR_NONE;
}

int SmxClass::FindVertexIndex( SmxClass::VECTOR3D *vertex )
{

	for ( int i=0; i<vertices.size(); i++ )
	{
        if ( memcmp( &vertices[i], vertex, sizeof( SmxClass::VECTOR3D ) ) == 0 )
		{
			return i;
        }
	}

    return -1;

}

int SmxClass::FindNormalIndex( SmxClass::VECTOR3D *vertex )
{
	std::cout << "Normal count: " << std::to_string( normals.size() ) << std::endl;
	
	for ( int i=0; i<normals.size(); i++ )
	{
        if ( memcmp( &normals[i], vertex, sizeof( SmxClass::VECTOR3D ) ) == 0 )
		{
			return i;
        }
	}

    return -1;

}

int SmxClass::IsTextured( PRIMITIVE* pri ) {
	
	if ( ( pri->primtype == SmxClass::PRIM_TRI_FT ) || 
		( pri->primtype == SmxClass::PRIM_TRI_GT ) ||
		( pri->primtype == SmxClass::PRIM_QUAD_FT ) || 
		( pri->primtype == SmxClass::PRIM_QUAD_GT ) ) {
		
		return 1;
		
	}
	
	return 0;
}