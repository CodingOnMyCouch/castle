#include "CardBits.h"
#include <iostream>

#define TOLERANCE 10

BitPattern CardBits::getID()const
{
	return BitPattern_;
}

void CardBits::setID(BitPattern Bit_Pattern)
{
	BitPattern_ = Bit_Pattern;
}

//  overloading the setPoaition function
void CardBits::setPosition(Coords axis)
{
	x_axis = axis.x_axis;
	y_axis = axis.y_axis;

	Position.x = axis.x_axis;
	Position.y = axis.y_axis;
}

void CardBits::setPosition(float xCoord, float yCoord)
{
	// updating the x and y co-ordinates
	x_axis = xCoord;
	y_axis = yCoord;

	// updating the Position co-ordinates
	Position.x = xCoord;
	Position.y = yCoord;

	// SETTING THE COORDS AXIS 
	axis.x_axis = x_axis;
	axis.y_axis = y_axis;
}

void CardBits::setPosition(glm::vec2 Position)
{
	x_axis = Position.x;
	y_axis = Position.y;

	// SETTING THE COORDS AXIS 
	axis.x_axis = x_axis;
	axis.y_axis = y_axis;
}

float CardBits::getLocationX()
{
	return x_axis;
}

float CardBits::getLocationY()
{
	return y_axis;
}

//glm::vec2 CardBits::Move(GLfloat dt, int positionX, int positionY)
glm::vec2 CardBits::Move(float dt, int positionX, int positionY)
{
	//std::cout << "positionX: " << positionX << std::endl;
	//std::cout << "positionY: " << positionY << std::endl;
	//std::cout << "deltaSeconds: " << std::dec << dt << std::endl;
	//std::cout << "Position.x: " << Position.x << std::endl;
	//std::cout << "Position.y: " << Position.y << std::endl;
	//std::cout << "Position.x: " << this->Position.x << std::endl;
	//std::cout << "Position.y: " << this->Position.y << std::endl;
	//std::cout << "Velocity.x: " << this->Velocity.x << std::endl;
	//std::cout << "Velocity.y: " << this->Velocity.y << std::endl;
	//dt = 0.035f;
	dt /= 9999;
	//dt = 0.045f;
	//std::cin.get();
	// check to see if the card has reached it final destination
	// the quadrants go in here
	if (this->Position.x >= positionX && this->Position.y >= positionY)
	{
		std::cout << "North West Direction" << std::endl;
		// move the ball in the right direction
		if (this->Position.x != positionX)
		{
			// the coordinates along the hypotenuse should be calculated along here
			this->Position.x -= Velocity.x * dt;
		}
		if (this->Position.y != positionY)
		{
			this->Position.y -= Velocity.y * dt;
		}
		//std::cout << "x: " << this->Position.x << std::endl;
		//std::cout << "y: " << this->Position.y << std::endl;
		if (Position.x <= positionX + TOLERANCE && Position.x >= positionX - TOLERANCE
			&& Position.y <= positionY + TOLERANCE && Position.y >= positionY - TOLERANCE)
		{
			// the Game state is set to GAME_PLACEINNEWPILE
			this->Position.x = positionX;
			this->Position.y = positionY;
		}
	}
	else if (this->Position.x <= positionX && this->Position.y >= positionY)
	{
		std::cout << "North East Direction" << std::endl;
		// move the ball in the right direction
		if (this->Position.x != positionX)
		{
			this->Position.x += Velocity.x * dt;
		}
		if (this->Position.y != positionY)
		{
			this->Position.y -= Velocity.y * dt;
		}
		//std::cout << "x: " << this->Position.x << std::endl;
		//std::cout << "y: " << this->Position.y << std::endl;
		if (Position.x <= positionX + TOLERANCE && Position.x >= positionX - TOLERANCE
			&& Position.y <= positionY + TOLERANCE && Position.y >= positionY - TOLERANCE)
		{
			// the Game state is set to GAME_PLACEINNEWPILE
			//this->setState();
			this->Position.x = positionX;
			this->Position.y = positionY;
		}
	}
	else if (this->Position.x <= positionX && this->Position.y <= positionY)
	{
		std::cout << "South East Direction" << std::endl;
		// move the ball in the right direction
		if (this->Position.x != positionX)
		{
			this->Position.x += Velocity.x * dt;
		}
		if (this->Position.y != positionY)
		{
			this->Position.y += Velocity.y * dt;
		}
		//std::cout << "x: " << this->Position.x << std::endl;
		//std::cout << "y: " << this->Position.y << std::endl;
		if (Position.x <= positionX + TOLERANCE && Position.x >= positionX - TOLERANCE
			&& Position.y <= positionY + TOLERANCE && Position.y >= positionY - TOLERANCE)
		{
			// the Game state is set to GAME_PLACEINNEWPILE
			//this->setState();
			this->Position.x = positionX;
			this->Position.y = positionY;
		}
	}
	else if (this->Position.x >= positionX && this->Position.y <= positionY)
	{
		std::cout << "South West Direction" << std::endl;
		// move the ball in the right direction
		if (this->Position.x != positionX)
		{
			this->Position.x -= Velocity.x * dt;
		}
		if (this->Position.y != positionY)
		{
			this->Position.y += Velocity.y * dt;
		}
		//std::cout << "x: " << this->Position.x << std::endl;
		//std::cout << "y: " << this->Position.y << std::endl;

		if (Position.x <= positionX + TOLERANCE && Position.x >= positionX - TOLERANCE
			&& Position.y <= positionY + TOLERANCE && Position.y >= positionY - TOLERANCE)
		{
			// the Game state is set to GAME_PLACEINNEWPILE
			//this->setState();
			this->Position.x = positionX;
			this->Position.y = positionY;
		}
	}

	//this->setPosition(positionX, positionY);
	return this->Position;
}