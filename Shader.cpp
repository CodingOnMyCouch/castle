//Shader.cpp
#include "Shader.h"

#include<iostream>
#ifdef _ANDROID_
#include <android/log.h>
#endif


namespace
{
	#define LOG_TAG "Shader"
}
//defining the Shader &Use function
Shader &Shader::Use()
{
	glUseProgram(this->ID); //use our shader program when we want to render an OpenGL object
	return *this;
};


void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource)
{
	GLuint sVertex, sFragment, gShader = 0;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);

	const GLchar * sources[2];
#if defined(_ANDROID_)
	sources[0] = "#version 300 es\nprecision mediump float;\n";
#else
	sources[0] = "#version 330\n";
#endif
	sources[1] = vertexSource;
	glShaderSource(sVertex, 2, sources, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");

	//Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	sources[1] = fragmentSource;
	glShaderSource(sFragment, 2, sources, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");

#if !defined(_ANDROID_)
	//if the geomtrySource code is given, also compile the geometry shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
#endif

	//Shader Program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);

	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);

	glLinkProgram(this->ID);

	//checkCompileErrors(this->ID, "PROGRAM");

	//Delete the shaders as they're linked into our program now and are no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);

	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar* name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
};

void Shader::SetInteger(const GLchar* name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
};

void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
};

void Shader::SetVector2f(const GLchar* name, const glm::vec2 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
};

void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
};

void Shader::SetVector3f(const GLchar* name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
};

void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
};

void Shader::SetVector4f(const GLchar* name, const glm::vec4 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
};


void Shader::SetMatrix4(const GLchar* name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
};

void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
#if defined(_ANDROID_)
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Failed to compile shader. InfoLog= %s\n", infoLog);
#endif
			std::cout << "| ERROR::SHADER: COMPILE-TIME ERROR: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
#if defined(_ANDROID_)
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Failed to link shader. InfoLog= %s\n", infoLog);
#endif
			std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- -------------------------------------------------- -- "
				<< std::endl;
		}
	}
}