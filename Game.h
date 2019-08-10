// using the pimpl idiom
#pragma once	//this provides compilation speed optimisation that some compilers can execute
#ifndef GAME_H
#define GAME_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif
#include<SDL2\SDL.h>
#include<SDL2\SDL_mixer.h>

#include <memory>                 // std::shared_ptr and std::weak_ptr
#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "CardObject.h"
//#include "StopWatch.h"
#include "SharedData.h"
#include "CardBits.h"


//extern const GLuint HEIGHT;
//extern const GLuint WIDTH;
extern GLuint HEIGHT;
extern GLuint WIDTH;

#if defined(_ANDROID_)
//extern const GLuint HEIGHT;
//extern const GLuint WIDTH;
extern GLuint HEIGHT;
extern GLuint WIDTH;
#endif

// including the enums
enum GameState
{
	GAME_INIT,
	GAME_INTRO,
	GAME_ACTIVE,
	GAME_MENU,
	GAME_PLACEINNEWPILE,
	GAME_SMOOTHMOVER,
	GAME_UPDATE,
	GAME_WIN,
	GAME_INSTRUCTIONS,
	GAME_SCORESYSTEMS,
	GAME_RESUME,
	GAME_RESTART,
};

enum TimerState
{
	OFF,
	ON,
};

enum MoveToPile
{
	FINDPILE,
	RELOCATEPILE,
	SMOOTHMOVER,
	CLEANUP,
};


// the Game interface
class Game
{
public: // public access specifier
    // public data fields
	///<description> .... enum objects .... <description/>///
	TimerState ClockState;
	GameState State;
	MoveToPile mover;
	//Scalene scalene;
	
	///<description/> .... data fields .... <description/>///
	GLuint Width, Height;
	GLboolean ClickA;
	GLuint Moves;
	GLuint Score;
	GLuint tempScore;
	GLuint Time;
	GLuint Caption;
	float Xdest;
	float Ydest;
	int xpos = 0;
	int ypos = 0;
	
	///<description> .... containers .... <description/>///
	// a pair container to store the coordinates
	std::pair <int, int> Coords;
	std::pair <int, int> moveTexture;

	// a vector for the iterators for each card region
	std::vector<CardObject*>::iterator iter1;

	std::vector<CardObject*> iterFoundation1;
	std::vector<CardObject*> iterFoundation2;
	std::vector<CardObject*> iterFoundation3;
	std::vector<CardObject*> iterFoundation4;

	std::vector<CardObject*> iterPileL1;
	std::vector<CardObject*> iterPileL2;
	std::vector<CardObject*> iterPileL3;
	std::vector<CardObject*> iterPileL4;
	std::vector<CardObject*> iterPileR1;
	std::vector<CardObject*> iterPileR2;
	std::vector<CardObject*> iterPileR3;
	std::vector<CardObject*> iterPileR4;

	// the container for the CardObjects
	std::vector<CardObject*> Deck;

	// the container for the CardBits
	std::vector<CardBits*> BitPack;

	// the Bit Foundations
	std::vector<CardBits*> BitFoundation1;
	std::vector<CardBits*> BitFoundation2;
	std::vector<CardBits*> BitFoundation3;
	std::vector<CardBits*> BitFoundation4;

	// the Bit piles
	std::vector<CardBits*> BitPileL1;
	std::vector<CardBits*> BitPileL2;
	std::vector<CardBits*> BitPileL3;
	std::vector<CardBits*> BitPileL4;
	std::vector<CardBits*> BitPileR1;
	std::vector<CardBits*> BitPileR2;
	std::vector<CardBits*> BitPileR3;
	std::vector<CardBits*> BitPileR4;

	// temporary iterPile container
	std::vector<CardObject*> It_temp;

	// temporary BitPile container
	std::vector<CardBits*> Bit_temp;

	// temporary BitPattern holder
	BitPattern tempBit = { 0x000000 };
	bool tempBitAssigned = false;
	

	// a container to hold the candidate BitPiles
	typedef unsigned int candidate;
	std::vector<candidate> Candidate;

	///<description> .... member functions .... <descriptions/>///
	//setter functions
	void setGameState(GameState _state);
    void setTimerState(TimerState _state);
	void setMoveToPile(MoveToPile _state);

	///<description> .... static variables that control the flow of the program ... <description/>///
	// the token to identify the Key
	static std::string token;
	static int logicGuard;
	static std::string nameOfPile;
	
	//constructor/destructor
	Game(GLuint width, GLuint height);
	~Game();

	//Initialise game state(load all shaders and textures - there is only one level)
	void Init();

	void InitBits();

	void InitCards();

	void PopulateDeck();

	void ResetGame();

	void shuffleBits_();

	void distributeBits_();

	void Assign_CardObject_Pos(std::vector<CardBits*> &, int num);

	void Update_CardObject_Pos(std::vector<CardBits*> &, int num);

	void Print_Object_Pos(std::vector<CardObject*> &);

	void Clear_Iter_Vectors();

	void Create_Iter_Vector(std::vector<CardObject*> &);

	void Init_Iter_Vec();

	void Sort_Iter_Vec();

	void Print_Iter_Vec_Pos(std::vector<CardObject*> &);

	


	// game loop
	void processInput(SDL_Event *ev, GLfloat dt);
	
	void Quit(SDL_Event *ev);

	void Update(GLfloat dt);

	void Render();

	//screen co-ordinate capture
	//void captureCoords(int **xp, int **yp);

	// this function takes a reference of an pair obj as its argument
	void moveCard(int mxp, int myp);
	//void setClickA(const bool **selectA);
	//bool getClickA(void);
	void moveToPile();
	void moveToPileAuxillairy(candidate pile, std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1);


	void RecursiveRenderer();
	void RecursiveRendererAuxillary(int num, candidate &pile, std::vector<CardBits*> &cntr, float Xdist, float Ydist,
		                            float distance, float distance2, bool calculateD2, float pos_x, float pos_y, float Xpos,
		                            float Ypos, glm::vec2 location, candidate pileReceive, candidate pileReceive2);

	void RecursiveRendererAuxillary2(float Xdist, float Ydist, glm::vec2 location, float distance, float distance2,
		                             bool calculateD2, candidate pileReceive, candidate pileReceive2);

	void RecursiveRendererAuxillary3(candidate pileReceive, candidate &pile, std::vector<CardBits*> &cntr,
		float X, float Y, float Xdest, float Ydest, float left_offset, float right_offset);

	void bruteForceSearch();
	// overloading the bruteforce auxillary helper functions
	void bruteForceAuxillary(int num, candidate pile, std::vector<CardBits*> &cntr);
	void bruteForceAuxillary2(int num, candidate &pile, std::vector<CardBits*> &cntr);
	void bruteForcePileAuxillary3(int num, candidate& pile, std::vector<CardBits*> &cntr);
	void bruteForceKings( candidate& pile);

	// updating the score function for the CardBit foundations
	void score();
	
	

	// Functions to handle the clock
	void TurnClockOn();
	void clockStatus();
	void ActivatedClock();
	void setClockToFalse();

	//Game Loop
	
	void MenuSelection(float x, float y);
	void MenuSelectionTouch(float x, float y);
	void InstructionExit(int x, int y);
	void ScoringSystemExit(int x, int y);
	void MouseDown(float x, float y);
	
	// Game Mechanics Functions
	// this how intial co-ordinates are passed to the card selected
	void AssignCoords(float x, float y);
	void AssignCoords2(float x, float y);
	void MovingCardFromPileCntrToTempCntr(std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1, float x, float y);
	void mustExecute(std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1);

	
    
    // Handling input via the mouse
	void HandleMouseDownEvent(SDL_Event *event);
	int HandleMouseMoveEvent(SDL_Event *event);
	void HandleMouseUpEvent(SDL_Event *event);


	// Handling Input via the Touch Screen
	void HandleFingerTouchDownEvent(SDL_Event *event);
	int HandleFingerMoveEvent(SDL_Event *event);
	void HandleFingerUpEvent(SDL_Event *event);


	// Handling the card drag
	int InitDrag(float x, float y);
	int DoDrag(float mx, float my);
	void ReleaseCard(float x, float y);

};


#endif // GAME_H
