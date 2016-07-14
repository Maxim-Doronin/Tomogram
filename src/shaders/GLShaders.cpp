#include "shaders/GLShaders.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;

GLShader::GLShader() : ShaderProgram(0), vertex_shader(0), fragment_shader(0)
{
	remove("log.txt");
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		ofstream output("log.txt", std::ios::app);
		output << "Can't init glew\n";
		output.close();
	}
}

GLShader::~GLShader()
{
	glUseProgram(0);
	glDetachShader(ShaderProgram, vertex_shader);
	glDetachShader(ShaderProgram, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(ShaderProgram);
}

GLuint GLShader::loadFiles(const std::string& vertex_file_name, const std::string& fragment_file_name)
{
	const GLchar* vShader;
	const GLchar* fShader;
	vShader = loadSourcefile(vertex_file_name);
	fShader = loadSourcefile(fragment_file_name);
	
	versionControl();

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &vShader, NULL);
	glShaderSource(fragment_shader, 1, &fShader, NULL);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

#ifdef _DEBUG
	printInfoLogShader(vertex_shader, GL_VERTEX_SHADER);
	printInfoLogShader(fragment_shader, GL_FRAGMENT_SHADER);
#endif

	linkProgram();

	delete []vShader;
	delete []fShader;
	return ShaderProgram;
}

GLuint GLShader::load(const std::string& vertex_source, const std::string& fragment_source)
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
		ofstream output("log.txt", std::ios::app);
		output << "glLinkProgram: ";
		output.close();
#ifdef _DEBUG
		printInfoLogProgram(ShaderProgram);
#endif
		ShaderProgram = GL_FALSE;
		return;
	}
	else {
		ofstream output("log.txt", std::ios::app);
		output << "Successfully linked!\n\n";
		output.close();
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
	if (location == -1) {
		ofstream output("log.txt", std::ios::app);
		output << "Could not bind attribute " << name << "\n";
		output.close();
	}
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
	if (location == -1) {
		ofstream output("log.txt", std::ios::app);
		output << "Could not bind uniform " << name << "\n";
		output.close();
	}
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
void GLShader::setUniform(GLint location, const GLuint value)
{
	glUniform1i(location, value);
}
void GLShader::setUniform(const std::string& name, const glm::vec4& value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const glm::vec3& value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const glm::vec2& value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const glm::mat4& value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const GLint value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const GLuint value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}
void GLShader::setUniform(const std::string& name, const GLfloat value)
{
	GLint location = getUniformLocation(name);
	setUniform(location, value);
}

//---------------------------------------------------

//! Print info log
void GLShader::printInfoLogShader(GLuint shader, GLuint shader_type)
{
	int infologLen = 0;
	int charsWritten = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	ofstream output("log.txt", std::ios::app);
	if (infologLen > 1)
	{	
		GLchar *infoLog = new GLchar[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		if (shader_type == GL_VERTEX_SHADER) 
			output << "InfoLog vertex shader : "<< infoLog << "\n\n\n";
		else
			output << "InfoLog fragment shader : " << infoLog << "\n\n\n";
		delete[] infoLog;
	}
	else 
		if (shader_type == GL_VERTEX_SHADER)
			output << "Vertex shader is compiled" << "\n\n\n";
		else
			output << "Fragment shader is compiled"  << "\n\n\n";
	output.close();
}

//! Print info log
void GLShader::printInfoLogProgram(GLuint shader)
{
	int infologLen = 0;
	int charsWritten = 0;

	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		ofstream output("log.txt", std::ios::app);
		GLchar *infoLog = new GLchar[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetProgramInfoLog(shader, infologLen, &charsWritten, infoLog);

		output << "InfoLog: " << infoLog << "\n\n\n";
		delete[]infoLog;
		output.close();
	}
}

void GLShader::versionControl() {
	ofstream output("log.txt", std::ios::app);
	float version = (float)atof((const char *)glGetString(GL_VERSION));
	output << "OpenGL v" << version;
	if (version < 2) {
		output << " <-- ERROR: Need at least OpenGL v2.0. Does your graphics card support shaders?\n";
		exit(-1);
	}
	else {
		output << " <-- OKAY!\n\n\n";
	}
	output.close();
}

GLuint GLShader::compileSource(const GLchar* source, GLuint shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

#ifdef _DEBUG
	printInfoLogShader(shader, shader_type);
#endif
	return shader;
}

const GLchar* GLShader::loadSourcefile(const std::string& source_file_name)
{
	ifstream  file;
	file.open(source_file_name.c_str());
	if (!file.is_open()) {
		ofstream output("log.txt", std::ios::app);
		output << "Shader File Error" << std::endl;
		output.close();
		return 0;
	}
	long begin = file.tellg();
	file.seekg(0, std::ios::end);
	long end = file.tellg();
	int size = end - begin;
	
	char* tmp = new char[size + 1];
	file.seekg(0, std::ios::beg);

	int i = 0;
	while (file.good()) {
		tmp[i] = file.get();
		if (!file.eof()) i++;
	}
	tmp[i] = '\0';
	file.close();
	return (const GLchar*)tmp;
}
