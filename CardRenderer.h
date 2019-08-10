//CardRenderer.h

#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "GLFunctionLoader.h"


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
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class CardRenderer
{
public:
	//constructor (inits shaders/shapes)
	CardRenderer(Shader shader);

	//destructor
	~CardRenderer();

	//renders a defined quad textured with a given card
	//void DrawCard(Texture texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 colour = glm::vec3(1.0f));
	void DrawCard(Texture texture, float x_axis, float y_axis, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 colour = glm::vec3(1.0f));
private:
	//render state
	Shader shader;

	GLuint quadVAO;

	//initialise and configures the quad's buffer and vertex attributes
	void initRenderData();
};


#endif //CARDRENDERER_H

