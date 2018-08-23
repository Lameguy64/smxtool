/* 
 * File:   ShaderClass.cpp
 * Author: lameguy64
 * 
 * Created on November 15, 2017, 12:29 PM
 */

#include <malloc.h>
#include <iostream>
#include <GL/glew.h>
#include <FL/gl.h>
#include "ShaderClass.h"


// Shader program code
static const GLchar* gl_v_shader =
{
	"uniform mat4 gl_ModelViewMatrix;\n"
	"uniform mat4 gl_ProjectionMatrix;\n"
	"attribute vec4 gl_Vertex;\n"
	"attribute vec4 gl_Color;\n"
	"varying vec4 vColor;\n"
	"void main() {\n"
	"   gl_TexCoord[0]=gl_MultiTexCoord0;\n"
	"   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n"
	"   vColor = gl_Color;\n"
	"}\n"
};

static const GLchar* gl_f_shader =
{
	"uniform sampler2D texture;\n"
	"uniform bool tex_enable;\n"
	"varying vec4 vColor;\n"
	"void main() {\n"
	"   if (tex_enable) {\n"
	"      vec4 fColor;\n"
	"      float alpha;\n"
	"      fColor = texture2D(texture, gl_TexCoord[0].st);\n"
	"      alpha = fColor.w*vColor.w;\n"
	"      fColor *= (vColor*2.0);\n"
	"      fColor.w = alpha;\n"
	"      gl_FragColor = fColor;\n"
	"   } else {\n"
	"      gl_FragColor = vColor;\n"
	"   }\n"
	"}\n"
};


int ShaderClass::CompileShader(const GLchar* shader_code, GLuint shader_type)
{
	GLuint shader = glCreateShaderObjectARB( shader_type );
	
	glShaderSourceARB( shader, 1, &shader_code, nullptr );
	
	GLint compiled;
	
	glCompileShaderARB( shader );
	glGetObjectParameterivARB( shader, GL_COMPILE_STATUS, &compiled );
	
	GLint blen = 0;	
	GLsizei slen = 0;
	glGetShaderiv( shader, GL_INFO_LOG_LENGTH , &blen);       
	
	if (blen > 1)
	{
		GLchar* compiler_log = (GLchar*)malloc( blen );
		glGetInfoLogARB( shader, blen, &slen, compiler_log );
		
		std::cout << "ERROR: Failed to compile shader program." << std::endl;
		std::cout << compiler_log << std::endl;
		free(compiler_log);
		
		glDeleteObjectARB( shader );
		
		return 0;
	}
	
	return shader;
}

ShaderClass::ShaderClass()
{
	vertex_shader = 0;
	fragment_shader = 0;
	shader_program = 0;
	is_initialized = false;
}

ShaderClass::~ShaderClass()
{
	if ( vertex_shader )
	{
		glDeleteObjectARB( vertex_shader );
	}
	if ( fragment_shader )
	{
		glDeleteObjectARB( fragment_shader );
	}
	if ( shader_program )
	{
		glDeleteObjectARB( shader_program );
	}
}

int ShaderClass::Initialize()
{
	vertex_shader = CompileShader( gl_v_shader, GL_VERTEX_SHADER );
	
	if ( !vertex_shader )
	{
		return 1;
	}
	
	fragment_shader = CompileShader( gl_f_shader, GL_FRAGMENT_SHADER );
	
	if ( !fragment_shader )
	{
		return 1;
	}
	
	shader_program = glCreateProgram();

	glAttachShader( shader_program, vertex_shader );
	glAttachShader( shader_program, fragment_shader );

	glLinkProgram( shader_program );
	
	glUseProgramObjectARB( shader_program );
	
	uniform_texture_enable = glGetUniformLocationARB( shader_program, 
		"tex_enable" );
	
	//std::cout << "tex_enable = " << uniform_texture_enable << std::endl;
	
	is_initialized = true;
	return 0;
}

void ShaderClass::TextureEnable(bool enable)
{
	if ( !is_initialized )
	{
		return;
	}
	glUniform1iARB( uniform_texture_enable, enable );
}

void ShaderClass::ActivateShader()
{
	if ( !is_initialized )
	{
		return;
	}
	glUseProgramObjectARB( shader_program );
}

void ShaderClass::DeactivateShader()
{
	if ( !is_initialized )
	{
		return;
	}
	glUseProgramObjectARB( 0 );
}