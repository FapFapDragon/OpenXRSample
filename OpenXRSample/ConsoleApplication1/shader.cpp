#include "shader.hpp"
#include <fstream>
#include <iostream>
#include "liststring.h"
#include <vector>

Shader* Shader::default_shader = 0;

char* Shader::loadShaderText(const char* file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open()) 
	{
		printf("Unable to open File %s\n", file_path);
		return NULL;
	}

	String str = createString();

	char ch;
	while (file.get(ch)) {
		// Output the text from the file
		addChar(str, ch);
	}
	char* result = putString(str);
	freeString(str);
	return result;
}

bool Shader::compileShader(GLuint shader_id, const char* shader_text)
{
	GLint result = GL_FALSE;
	int InfoLogLength;

	glShaderSource(shader_id, 1, &shader_text, NULL);
	glCompileShader(shader_id);

	// Check Vertex Shader
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result == GL_FALSE) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
		return false;
	}

	return true;
}

void Shader::buildProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
	GLuint program = glCreateProgram();
	GLint result = GL_FALSE;

	glAttachShader(program, vertex_shader_id);
	glAttachShader(program, fragment_shader_id);
	glLinkProgram(program);
	int InfoLogLength;

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result == GL_FALSE) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		return;
	}

	glDetachShader(program, vertex_shader_id);
	glDetachShader(program, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	this->program_id = program;
	

}

GLuint Shader::getProgram()
{
	return this->program_id;
}

Shader::Shader(const char* vert_path, const char* frag_path, bool default_shader)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	char* vert_text = loadShaderText(vert_path);
	char* frag_text = loadShaderText(frag_path);

	if (!compileShader(vertex_shader_id ,vert_text)) 
	{
		throw::std::runtime_error("Unable to generate Vertex Shader");
	}

	if (!compileShader(fragment_shader_id, frag_text))
	{
		throw::std::runtime_error("Unable to generate Vertex Shader");
	}

	buildProgram(vertex_shader_id, fragment_shader_id);

	if (default_shader) 
	{
		Shader::default_shader = this;
	}
	free(vert_text);
	free(frag_text);
}

Shader::Shader(const char* vert_path, const char* frag_path)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	char* vert_text = loadShaderText(vert_path);
	char* frag_text = loadShaderText(frag_path);

	if (!compileShader(vertex_shader_id, vert_text))
	{
		throw::std::runtime_error("Unable to generate Vertex Shader");
	}

	if (!compileShader(fragment_shader_id, frag_text))
	{
		throw::std::runtime_error("Unable to generate Vertex Shader");
	}

	buildProgram(vertex_shader_id, fragment_shader_id);

	free(vert_text);
	free(frag_text);
}
