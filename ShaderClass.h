/* 
 * File:   ShaderClass.h
 * Author: lameguy64
 *
 * Created on November 15, 2017, 12:29 PM
 */

#ifndef SHADERCLASS_H
#define SHADERCLASS_H

class ShaderClass
{
	
	GLuint	vertex_shader;
	GLuint	fragment_shader;
	GLuint	shader_program;
	GLint	uniform_texture_enable;
	bool	is_initialized;
	
	int CompileShader(const char* shader_code, GLuint shader_type);
	
public:
	
	ShaderClass();
	virtual ~ShaderClass();

	int Initialize();
	
	void ActivateShader();
	void DeactivateShader();
	
	void TextureEnable(bool enable);
	
	bool Initialized()
	{
		return is_initialized;
	}
};

#endif /* SHADERCLASS_H */

