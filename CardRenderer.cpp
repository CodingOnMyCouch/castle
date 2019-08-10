//CardRenderer.cpp

#include "CardRenderer.h"

//defining the constructor
// the gcc compiler complains about a non-const lvalue binding to a reference
CardRenderer::CardRenderer(Shader shader)
{
	this->shader = shader;
	this->initRenderData();
}


//defining the destructor
CardRenderer::~CardRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void CardRenderer::DrawCard(Texture texture, float x_axis, float y_axis, glm::vec2 size, GLfloat rotate, glm::vec3 colour)
{
	glm::vec2 position;
	position.x = x_axis;
	position.y = y_axis;
	//prepare transformations
	this->shader.Use();
	glm::mat4 model;
	//first translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//move the origins of rotation to the center of the quad
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	//then rotate
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	//move origin back
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5f * size.y, 0.0f));

	//last scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);

	//render textured quad 
	this->shader.SetVector3f("cardColour", colour);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void CardRenderer::initRenderData()
{
	//configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] =
	{
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}