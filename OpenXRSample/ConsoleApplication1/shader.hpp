#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>

class Shader
{
private:
	static char* loadShaderText(const char* file_path);

	GLuint program_id;

public: 
	static Shader* default_shader;

	static bool compileShader(GLuint shader_id, const char* shader_text);
	
	void buildProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

	GLuint getProgram();

	Shader(const char* vert_path, const char* frag_path, bool default_shader);

	Shader(const char* vert_path, const char* frag_path);

};

#endif 