#include "CardObject.h"
#include <iostream>

#define TOLERANCE 10

/*
//defining the default constructor with the initialisation list
CardObject::CardObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Colour(1.0f), Rotation(0.0f), Sprite(), 
{
	std::cout << "Default CardObject constructor" << std::endl;
	//std::cin.get();
}*/

//defining the default constructor with the initialisation list
/*CardObject::CardObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Colour(1.0f), Rotation(0.0f), Sprite()
	{
			std::cout << "Default CardObject constructor" << std::endl;
	}*/

//defining the constructor with the initialisation list
CardObject::CardObject(float x_axis, float y_axis, glm::vec2 size, Texture sprite, unsigned int BitPattern_, glm::vec2 velocity, glm::vec3 colour)
	: axis{ x_axis, y_axis }, Size(size), Sprite(sprite), BitPattern_{ BitPattern_ }, Velocity(velocity), Colour(colour), Rotation(0.0f)
{
	//initialising the ditsnace variables within the brace
	// Xdestination = 0;  
	// Ydestination = 0;  
	 std::cout << "Initialised constructor" << std::endl;
	 //std::cin.get();
}

BitPattern CardObject::getID()const
{
	return BitPattern_;
}

void CardObject::setID(BitPattern Bit_Pattern)
{
	BitPattern_ = Bit_Pattern;
}

void CardObject::Draw(CardRenderer &renderer)
{
	CardRenderer *Renderer = &renderer;
	//std::unique_ptr<CardRenderer, renderer&> Renderer(new CardRenderer, renderer);
	Renderer->DrawCard(this->Sprite, this->x_axis, this->y_axis, this->Size, this->Rotation, this->Colour);
}

/*glm::vec2 CardObject::Move(GLfloat dt, int positionX, int positionY)
{	
	std::cout << "positionX: " << positionX << std::endl;
	std::cout << "positionY: " << positionY << std::endl;
	std::cout << "deltaSeconds: " << dt << std::endl;
	std::cout << "Position.x: " << Position.x << std::endl;
	std::cout << "Position.y: " << Position.y << std::endl;
	std::cout << "Position.x: " << this->Position.x << std::endl;
	std::cout << "Position.y: " << this->Position.y << std::endl;
	//dt = 0.035f;
	dt = 0.045f;
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
		std::cout << "x: " << this->Position.x << std::endl;
		std::cout << "y: " << this->Position.y << std::endl;
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
		std::cout << "x: " << this->Position.x << std::endl;
		std::cout << "y: " << this->Position.y << std::endl;
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
		std::cout << "x: " << this->Position.x << std::endl;
		std::cout << "y: " << this->Position.y << std::endl;
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
	   std::cout << "x: " << this->Position.x << std::endl;
	   std::cout << "y: " << this->Position.y << std::endl;

	   if (Position.x <= positionX + TOLERANCE && Position.x >= positionX - TOLERANCE
		   && Position.y <= positionY + TOLERANCE && Position.y >= positionY - TOLERANCE)
	   {
		   // the Game state is set to GAME_PLACEINNEWPILE
		   //this->setState();
		   this->Position.x = positionX;
		   this->Position.y = positionY;
	   }
	}
	return this->Position;
}*/

void CardObject::setXdest(int X)
{
	 Xdestination = X;
};

void CardObject::setYdest(int Y)
{
	 Ydestination = Y;
};

