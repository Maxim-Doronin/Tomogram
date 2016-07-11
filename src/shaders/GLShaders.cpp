#include "shaders/GLShaders.h"
#include <fstream>
#include <iostream>

using std::ifstream;

GLShader::GLShader() : ShaderProgram(0)
{

}

GLShader::~GLShader()
{
	glUseProgram(0);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(ShaderProgram);
}

GLuint GLShader::loadFiles(const string& vertex_file_name, const string& fragment_file_name)
{
	vertex_shader = loadSourcefile(vertex_file_name, GL_VERTEX_SHADER);
	fragment_shader = loadSourcefile(fragment_file_name, GL_FRAGMENT_SHADER);

	linkProgram();

	return ShaderProgram;
}

GLuint GLShader::load(const string& vertex_source, const string& fragment_source)
{
	vertex_shader = compileSource(vertex_source.c_str(), GL_VERTEX_SHADER);
	fragment_shader = compileSource(fragment_source.c_str(), GL_FRAGMENT_SHADER);

	linkProgram();
	return ShaderProgram;
}

GLuint GLShader::load(const GLchar* vertex_source, const GLchar* fragment_source)
{
	vertex_shader = compileSource(vertex_source, GL_VERTEX_SHADER);
	fragment_shader = compileSource(fragment_source, GL_FRAGMENT_SHADER);

	linkProgram();
	return ShaderProgram;
}

void GLShader::linkProgram()
{
	GLint link_ok = GL_FALSE;
	if (!vertex_shader || !fragment_shader)
	{
		ShaderProgram = GL_FALSE;
		return;
	}

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertex_shader);
	glAttachShader(ShaderProgram, fragment_shader);

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		std::cout << "glLinkProgram:";
#ifdef _DEBUG
		printInfoLogProgram(ShaderProgram);
#endif
		ShaderProgram = GL_FALSE;
		return;
	}
}

void GLShader::use()
{
	glUseProgram(ShaderProgram);
}

void GLShader::release()
{
	glUseProgram(0);
}

GLuint GLShader::getIDProgram() 
{ 
	return ShaderProgram; 
}

bool GLShader::isLoad() 
{ 
	return ShaderProgram != 0; 
}

//---------------------------------------------------

//! Attribute get
GLint GLShader::getAttribLocation(const GLchar* name) const
{
	GLint location = -1;
	location = glGetAttribLocation(ShaderProgram, name);
	if (location == -1)
		std::cout << "Could not bind attribute " << name << "\n";
	return location;
}

GLint GLShader::getAttribLocation(const std::string& name) const
{
	return getAttribLocation(name.c_str());
}

//! Uniform get
GLint GLShader::getUniformLocation(const GLchar* name) const
{
	GLint location = -1;
	location = glGetUniformLocation(ShaderProgram, name);
	if (location == -1)
		std::cout << "Could not bind uniform " << name << "\n";
	return location;
}

GLint GLShader::getUniformLocation(const std::string& name) const
{
	return getUniformLocation(name.c_str());
}
//---------------------------------------------------

//! Set Uniform 

void GLShader::setUniform(GLint location, const glm::vec4& value)
{
	glUniform4fv(location, 1, &value[0]);
}
void GLShader::setUniform(GLint location, const glm::vec3& value)
{
	glUniform3fv(location, 1, &value[0]);
}
void GLShader::setUniform(GLint location, const glm::vec2& value)
{
	glUniform2fv(location, 1, &value[0]);
}

void GLShader::setUniform(GLint location, const glm::mat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}
void GLShader::setUniform(GLint location, const GLfloat value)
{
	glUniform1f(location, value);
}
void GLShader::setUniform(GLint location, const GLint value)
{
	glUniform1i(location, value);
}
//---------------------------------------------------

//! Print info log
void GLShader::printInfoLogShader(GLuint shader)
{
	int infologLen = 0;
	int charsWritten = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		GLchar *infoLog = new GLchar[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);

		std::cout << "InfoLog: " << infoLog << "\n\n\n";
		delete[] infoLog;
	}
}

//! Print info log
void GLShader::printInfoLogProgram(GLuint shader)
{
	int infologLen = 0;
	int charsWritten = 0;

	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		GLchar *infoLog = new GLchar[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetProgramInfoLog(shader, infologLen, &charsWritten, infoLog);

		std::cout << "InfoLog: " << infoLog << "\n\n\n";
		delete[]infoLog;
	}
}

GLuint GLShader::compileSource(const GLchar* source, GLuint shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

#ifdef _DEBUG
	printInfoLogShader(shader);
#endif
	return shader;
}

GLuint GLShader::loadSourcefile(const string& source_file_name, GLuint shader_type)
{
	ifstream  file(source_file_name.c_str());
	if (!file)
	{
		std::cout << source_file_name << "  file  not  found\n";
		return  GL_FALSE;
	}

	std::istreambuf_iterator<char>  begin(file), end;
	string  sourceStr(begin, end);
	file.close();
	return  compileSource(sourceStr.c_str(), shader_type);
}
