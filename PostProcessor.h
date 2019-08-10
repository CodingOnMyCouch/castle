//the PostProceesor class header file
#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>

#include <KHR/khrplatform.h>
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
#include <SDL2/SDL_opengles2.h>
#include <SDL2/SDL_opengles2_gl2.h>
#include <SDL2/SDL_opengles2_gl2ext.h>
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
#include "CardRenderer.h"
#include "Shader.h"

// The PostProcessor renders the game on a textured quad
// It is required to call BeginRnder() before RENDERING THE GAME
// and EndRender() after rendering the game for the class to work

class PostProcessor
{
public: 
	// state
	Shader PostProcessingShader;
	Texture Texture;
	GLuint Width, Height;

	// constructor
	PostProcessor(Shader shader, GLuint width, GLuint height);

	// prepares the postprocessor's framebuffer operations before rendering the game
	void BeginRender();

	// should be called after rendering the game, so it stores all the rendered data into a texture object
	void EndRender();

	// Renders the PostProcessor texture quad(as a screen-encompassing large sprite)
	void Render();

private:
	// Render state
	GLuint MSFBO, FBO;  //MSFBO = Multisampled FBO. FBO is regular, used for blitting MS colour-buffer to texture
	GLuint RBO;  // RBO is used for multisampled colour buffer
	GLuint VAO;

	// Initialise quad for rendering postprocessing texture
	void initRenderData();

};





#endif //POSTPROCESSOR_H
