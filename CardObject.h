#ifndef CARDOBJECT_H
#define CARDOBJECT_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

//#include <memory>                    //std::unique_ptr
#include <string>
#include <glm/glm.hpp>

#include "Texture.h"
#include "CardRenderer.h"
#include "CardBits.h"
#include "SharedData.h"




//Container object for holding all state relevant for a single
//game object entity.
class CardObject
{
private:
	
	Coords axis;
	float x_axis, y_axis;
	glm::vec2 Size;
	Texture Sprite;
	BitPattern BitPattern_;
	glm::vec2 Position, Velocity;
	glm::vec3 Colour;
	GLfloat Rotation;

	// dont forget to reset these variables afterwards
	int Xdestination;   // holds the x-coordinate of the candidate pile
	int Ydestination;   // holds the y-coordinate of the candidate pile

public: //public access specifier

	
	
	// Constructor(s)
	//CardObject(); // default constructor

	//constructor with initialiser list
	CardObject(float x_axis, float y_axis, glm::vec2 size, Texture sprite, unsigned int Bit_Pattern,
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f), glm::vec3 colour = glm::vec3(1.0f)
	);
	
	//Destructor
	~CardObject() {};


	// setting the BitPattern
	void Set(const BitPattern &BitPattern_) { this->BitPattern_ = BitPattern_; }
	const BitPattern &Get(void) { return BitPattern_; }

	// setting the x and y coordinates
	void Set(const Coords &axis)
	{
		this->axis = axis;
		x_axis = axis.x_axis;
		y_axis = axis.y_axis;
	}


	void DisplayPosition()const
	{
		std::cout << axis.x_axis << "," << axis.y_axis << std::endl;
	}

	const Coords &GetCoords(void) { return axis; }

	BitPattern getID()const;
	void setID(BitPattern Bit_Pattern);

	//Position functions
	glm::vec2 getPosition() const
	{
		return  Position;
	}

	//Position functions
	float getX()
	{
		//return this->Position.x;
		return this->x_axis;
	}

	float getY()
	{
		//return this->Position.y;
		return this->y_axis;
	}

	void Print()
	{
		std::cout << this->getX() << "," << this->getY() << std::endl;
	}


	//glm::vec2 Move(GLfloat dt, int positionX, int positionY);


	void setPosition(glm::vec2 NewPosition) 
	{
		Position = NewPosition;
		x_axis = Position.x;
		y_axis = Position.y;
	}

	void setXdest(int X);

	void setYdest(int Y);
	
	int setState()
	{
		int game_movetopile = 4;
		return game_movetopile;
	}

	// Draw sprite
	void Draw(CardRenderer &renderer);
};



#endif //CARDOBJECT_H












