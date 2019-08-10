// the Text Renderer

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H


#include <map>


#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLFunctionLoader.h"


#ifndef glRenderbufferStorageMultisample
#define glRenderbufferStorageMultisample glRenderbufferStorageMultisampleIMG
#endif

#ifndef glBindVertexArray
#define glBindVertexArray GLFunctionLoader::instance().bindVertexArrayOES
#endif

#ifndef glGenVertexArrays
#define glGenVertexArrays GLFunctionLoader::instance().genVertexArraysOES
#endif

#ifndef glDeleteVertexArrays
#define glDeleteVertexArrays GLFunctionLoader::instance().deleteVertexArraysOES
#endif


#endif
#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to the next glyph
};

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library.
// A single font is loaded, processed into a list of Character items for later rendering
class TextRenderer
{
public:
	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;

	//Shader used for text rendering
	Shader TextShader;

	// Constructor
	TextRenderer(GLuint width, GLuint height);

	// Pre-compiles a list of characters from the given font
	void Load(std::string font, GLuint fontSize);

	// Renders a string of text using the precompiled list of characters
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour = glm::vec3(1.0f));

private:

	// Render state
	GLuint VAO, VBO;
};


#endif // TEXTRENDERER_H
