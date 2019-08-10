///<description> this hold the data for each cards binary identification </description> ///

#ifndef CARDBITS_H
#define CARDBITS_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#endif

#include <glm/glm.hpp>

#include <iostream>
#include "SharedData.h"



class CardBits
{
private: 
	BitPattern BitPattern_;

	Coords axis;
	//glm::vec2 position;

	// public data fields
	float x_axis, y_axis;

	// captures the index of its relative CardObject
	int Index;


	// dont forget to reset these variables afterwards
	int Xdestination;   // holds the x-coordinate of the candidate pile
	int Ydestination;   // holds the y-coordinate of the candidate pile

	glm::vec2 Position, Velocity;

public:
	//default ctor
	CardBits()
	{
		Velocity = glm::vec2(200, 200);
		Xdestination = 0;
	    Ydestination = 0;
		std::cout << "Initialise the CardBits" << std::endl;
	};

	//dtor
	~CardBits() {};

	// get ID functions
	BitPattern getID()const;
	void setID(BitPattern Bit_Pattern);

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

	const Coords &GetCoords(void) { return axis; }

	glm::vec2 Move(float dt, int positionX, int positionY);
	//void Move(GLfloat dt, int positionX, int positionY);
	

	// position functions
	// overloading the setPosition function
	void setPosition(Coords axis);
	void setPosition(float xCoord, float yCoord);
	void setPosition(glm::vec2 Position);

	void DisplayPosition()const
	{
		std::cout << axis.x_axis << "," << axis.y_axis << std::endl;
	}

	float getLocationX();
	float getLocationY();	

	// the Index getter/setter functions
	void setIndex(int _count)
	{
		Index = _count;
	}

	int getIndex()
	{
		return Index;
	}
};




#endif //CARDBITS_H












