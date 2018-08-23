#ifndef _SMX_H
#define _SMX_H

#include <string>
#include <vector>

#ifndef nullptr
#define nullptr 0
#endif

/* bit 0 - line/polygon
 * bit 1 - triangle/quad
 * bit 2 - flat/shaded
 * bit 3 - untextured/textured
 */

#define SMX_PRIM_LINE		0x0
#define SMX_PRIM_POLY		0x1
#define SMX_PRIM_TRI		0x0
#define SMX_PRIM_QUAD		0x2
#define SMX_PRIM_FLAT		0x0
#define SMX_PRIM_GOURAUD	0x4
#define SMX_PRIM_UNTEXTURED	0x0
#define SMX_PRIM_TEXTURED	0x8

class SmxClass
{
public:

	enum SMX_ERROR
	{
		ERR_NONE = 0,
		ERR_NOTFOUND,
		ERR_INVALID,
		ERR_UNKNOWN,
		ERR_NOEXT,
		ERR_NO_MODEL,
		ERR_NO_VERTS,
		ERR_NO_NORMS,
		ERR_NO_PRIMS,
	};

	enum SMX_PRIMTYPE
	{
		PRIM_LINE_F		= SMX_PRIM_LINE|SMX_PRIM_FLAT,
		PRIM_LINE_G		= SMX_PRIM_LINE|SMX_PRIM_GOURAUD,
		
		PRIM_TRI_F		= SMX_PRIM_POLY|SMX_PRIM_TRI|SMX_PRIM_FLAT,
		PRIM_TRI_FT		= SMX_PRIM_POLY|SMX_PRIM_TRI|SMX_PRIM_FLAT|SMX_PRIM_TEXTURED,
		PRIM_TRI_G		= SMX_PRIM_POLY|SMX_PRIM_TRI|SMX_PRIM_GOURAUD,
		PRIM_TRI_GT		= SMX_PRIM_POLY|SMX_PRIM_TRI|SMX_PRIM_GOURAUD|SMX_PRIM_TEXTURED,
		
		PRIM_QUAD_F		= SMX_PRIM_POLY|SMX_PRIM_QUAD|SMX_PRIM_FLAT,
		PRIM_QUAD_FT	= SMX_PRIM_POLY|SMX_PRIM_QUAD|SMX_PRIM_FLAT|SMX_PRIM_TEXTURED,
		
		PRIM_QUAD_G		= SMX_PRIM_POLY|SMX_PRIM_QUAD|SMX_PRIM_GOURAUD,
		PRIM_QUAD_GT	= SMX_PRIM_POLY|SMX_PRIM_QUAD|SMX_PRIM_GOURAUD|SMX_PRIM_TEXTURED,
	};

	enum SMX_SHADETYPE
	{
		SHADE_NONE,
		SHADE_FLAT,
		SHADE_SMOOTH,
	};

	typedef struct {
		float x,y,z;
	} VECTOR3D;

	typedef struct {
		unsigned char u,v;
	} UV;

	typedef struct {
		unsigned char r,g,b;
	} RGB;

	typedef struct {
		int				primtype;	// Primitive type
		SMX_SHADETYPE	shadetype;	// Shading type
		int				texnum;		// Texture number of primitive (if textured)
		int				blendmode;
		int				doublesided;
		int				selected;
		int				v[4];		// Vertex indices
		int				n[4];		// Normal indices
		RGB				c[4];		// Vertex colors
		UV				t[4];		// Texture coords
	} PRIMITIVE;
	
	class TextureClass
	{
	public:
		std::string filename;
		int userdata1;
		void* userdata2;
	};

	SmxClass();
	virtual ~SmxClass();

	void Clear();
	
	SMX_ERROR LoadFile(const char* filename);
	SMX_ERROR SaveFile(const char *filename);
		
	int FindVertexIndex(VECTOR3D *vertex);
	int FindNormalIndex(VECTOR3D *vertex);
	
	int IsTextured(PRIMITIVE* pri);

	std::vector<VECTOR3D>		vertices;
	std::vector<VECTOR3D>		normals;
	std::vector<TextureClass*>	textures;
	std::vector<PRIMITIVE>		primitives;
	
};

#endif
