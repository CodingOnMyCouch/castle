// The Implementation .cpp
#include <iostream>
#include <algorithm>      // std::move(ranges)
#include <utility>        // std::move(objects)
#include <cmath>          // std::ads(positive values)
#include <cstdlib>        // std::malloc(allocate size bytes of uninitialised storage)
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <string>
 
#include "Game.h"
#include "ResourceManager.h"
//#include "CardObject.h"
#include "CardRenderer.h"
#include "StopWatch.h"
#include "TextRenderer.h"
#include "PostProcessor.h"


// forward declarations - prototypes
BitPattern operator|(const BitPattern &lhs, const BitPattern &rhs);
BitPattern &operator>>=(BitPattern &lhs, const BitPattern &rhs);
BitPattern &operator<<=(BitPattern &lhs, const BitPattern &rhs);
std::ostream &operator<<(std::ostream &os, const BitPattern &out);
BitPattern operator&(const BitPattern &lhs, const BitPattern &rhs); // Bitwise and
bool operator==(const BitPattern &lhs, const BitPattern &rhs); // Equivalence operator
bool operator!=(const BitPattern &lhs, const BitPattern &rhs);
GLuint &operator+=(GLuint &lhs, const BitPattern &rhs); // Assign sum


extern bool shouldQuit;
extern bool startDrag;

// a static variable to make sure that the
// card in the Bit_temp is rendered at the end
BitPattern renderAtTheEnd;

namespace
{
/*	// preprocessing directives - card positions
	// android can only process floating points that are up to four decimal places!!
#define LEFT_OFFSET -(WIDTH * (0.0188))   //-15.0f
#define RIGHT_OFFSET (WIDTH * (0.0188))    //15.0f
#define CARD_WIDTH (WIDTH * 0.1)           //80
#define CARD_LENGTH (HEIGHT * 0.2)         //120
#define X1 (WIDTH * (0.325))               //260
#define X2 ((WIDTH/2)-(CARD_WIDTH/2))      //360
#define X3 (WIDTH * 0.575)                 //460
#define Y1 (HEIGHT * (0.0667))             //50 - revised value 40
#define Y2 (HEIGHT * 0.2833)               //180 - revised value 170
#define Y3 (HEIGHT * (0.5))                //310 - revised value 300
#define Y4 (HEIGHT * (0.7167))             //440 - revised value 430
#define XOrigin 0
#define YOrigin 0
#define RIGHT_HAND_GAP  (WIDTH * 0.025) // 20      //(X2 - (X1 + CARD_WIDTH))
#define LEFT_HAND_GAP   (WIDTH * 0.025) // 20        //(X3 - (X2 + CARD_WIDTH))
#define TRAVELSPEED 0.016       // this is the speed that the card will travel at
#define PI 3.14159265           // to convert radians int degrees*/

	
		// preprocessing directives - card positions
		// android can only process floating points that are up to four decimal places!!
#define LEFT_OFFSET  -15.0f        //-(WIDTH * (0.0188))   //-15.0f
#define RIGHT_OFFSET  15.0f        // (WIDTH * (0.0188))    //15.0f
#define CARD_WIDTH    80           //(WIDTH * 0.1)           //80
#define CARD_LENGTH   120          // (HEIGHT * 0.2)         //120
#define X1      260                // (WIDTH * (0.325))               //260
#define X2      360                // ((WIDTH/2)-(CARD_WIDTH/2))      //360
#define X3      460                // (WIDTH * 0.575)                 //460
#define Y1      40                 //(HEIGHT * (0.0667))             //50 - revised value 40
#define Y2      170                // (HEIGHT * 0.2833)               //180 - revised value 170
#define Y3      300                // (HEIGHT * (0.5))                //310 - revised value 300
#define Y4      430                // (HEIGHT * (0.7167))             //440 - revised value 430
#define XOrigin 0
#define YOrigin 0
#define RIGHT_HAND_GAP  (WIDTH * 0.025) // 20      //(X2 - (X1 + CARD_WIDTH))
#define LEFT_HAND_GAP   (WIDTH * 0.025) // 20        //(X3 - (X2 + CARD_WIDTH))
#define TRAVELSPEED 0.016       // this is the speed that the card will travel at
#define PI 3.14159265           // to convert radians int degrees

	//static variable to control coordinate updates
	//static int index = 0;


	// flags to control the flow of the game
	bool mouseClick = false;
	static unsigned int doOnlyOnce = 0;
	static bool CLOCK = false;

	// the movement coordinates that work between InitDrag() and DoDrag()
	//static int xpos = 0;
	//static int ypos = 0;

	// bit patterns for the BitPiles
	typedef unsigned int candidate;
	candidate _BitFoundation1_ = 0x800;
	candidate _BitFoundation2_ = 0x400;
	candidate _BitFoundation3_ = 0x200;
	candidate _BitFoundation4_ = 0x100;
	candidate _BitPileL1_ = 0x080;
	candidate _BitPileL2_ = 0x040;
	candidate _BitPileL3_ = 0x020;
	candidate _BitPileL4_ = 0x010;
	candidate _BitPileR1_ = 0x008;
	candidate _BitPileR2_ = 0x004;
	candidate _BitPileR3_ = 0x002;
	candidate _BitPileR4_ = 0x001;



	//bit patterns for the iterPiles
	typedef unsigned int iterCheck;
	//iterCheck _iterFoundation1_ = 0x800;
	//iterCheck _iterFoundation2_ = 0x400;
	//iterCheck _iterFoundation3_ = 0x200;
	//iterCheck _iterFoundation4_ = 0x100;
	iterCheck _iterPileL1_ = 0x080;
	iterCheck _iterPileL2_ = 0x040;
	iterCheck _iterPileL3_ = 0x020;
	iterCheck _iterPileL4_ = 0x010;
	iterCheck _iterPileR1_ = 0x008;
	iterCheck _iterPileR2_ = 0x004;
	iterCheck _iterPileR3_ = 0x002;
	iterCheck _iterPileR4_ = 0x001;



	//static iterpile holders
	static iterCheck pile1 = 0;
	//static iterCheck pile2 = 0;


	// this static BitHolder will hold the value of the Bit vector than a card is originally selected from
	static candidate BitHolder = 0;
	static candidate BitofPile = 0;
}

// initial velocity of the CardBit
const glm::vec2 INITIAL_BALL_VELOCITY(200, 200);

extern int STATE;

#if defined(_ANDROID_)
extern SDL_Point *touchLocationPtr;
extern SDL_Point touchLocation;

// a boolean flag that records when the temp vector contains a card obj
//static bool CardInTempCntr = false;

// a boolean flag that records when the temp vector's position attributes are being updated
//static bool UpdatingCardPosition = false;
#endif

namespace
{
	// Game-related state data
	CardRenderer *Renderer;
	CardBits *bits;
	TextRenderer *Text;
	
	PostProcessor* Effects;
	// initialising the card objects
	CardObject *AOD;    CardObject *C3; CardObject *C5; CardObject *C7; CardObject *C9;   CardObject *JOC; CardObject *KOC;
	CardObject *AOC;    CardObject *D3; CardObject *D5; CardObject *D7; CardObject *D9;   CardObject *JOD; CardObject *KOD;
	CardObject *AOH;    CardObject *H3; CardObject *H5; CardObject *H7; CardObject *H9;   CardObject *JOH; CardObject *KOH;
	CardObject *AOS;    CardObject *S3; CardObject *S5; CardObject *S7; CardObject *S9;   CardObject *JOS; CardObject *KOS;
	CardObject *C2;     CardObject *C4; CardObject *C6; CardObject *C8; CardObject *C10;  CardObject *QOC;
	CardObject *D2;     CardObject *D4; CardObject *D6; CardObject *D8; CardObject *D10;  CardObject *QOD;
	CardObject *H2;     CardObject *H4; CardObject *H6; CardObject *H8; CardObject *H10;  CardObject *QOH;
	CardObject *S2;     CardObject *S4; CardObject *S6; CardObject *S8; CardObject *S10;  CardObject *QOS;
}

Timer *Timer::s_instance = 0;

// defining the static string variable
//std::string Game::token = "EMPTY";

// defining the static string variable
//std::string Game::nameOfPile = "EMPTY";

//defining the static int logicGuard
int Game::logicGuard = 0;

//constructor
Game::Game(GLuint width, GLuint height) :State(GAME_INTRO), Width(width), Height(height), ClickA(false),
Moves(0), Score(0), Time(0), Xdest(0), Ydest(0), xpos(0), ypos(0)
{
	
}

//destructor
Game::~Game()
{
	delete bits;
	delete Renderer;
	delete Text;
	delete Effects;
	delete AOD; delete C3; delete C5; delete C7; delete C9;  delete JOC; delete KOC;
	delete AOC; delete D3; delete D5; delete D7; delete D9;  delete JOD; delete KOD;
	delete AOH; delete H3; delete H5; delete H7; delete H9;  delete JOH; delete KOH;
	delete AOS; delete S3; delete S5; delete S7; delete S9;  delete JOS; delete KOS;
	delete C2;  delete C4; delete C6; delete C8; delete C10; delete QOC;
	delete D2;  delete D4; delete D6; delete D8; delete D10; delete QOD;
	delete H2;  delete H4; delete H6; delete H8; delete H10; delete QOH;
	delete S2;  delete S4; delete S6; delete S8; delete S10; delete QOS;
}

///<description> .... the pimpl functions .... <description/>///
// defining the setter functions
void Game::setGameState(GameState _state)
{
	this->State = _state;
	//pimpl->State = _state;
}

void Game::setTimerState(TimerState _state)
{
	this->ClockState = _state;
}

void Game::setMoveToPile(MoveToPile _state)
{
	this->mover = _state;
}

//initialise the shaders and texture
void Game::Init()
{
	//load shaders
	ResourceManager::LoadShader("card.vert", "card.frag", nullptr, "card");
	ResourceManager::LoadShader("PP.vert", "PP.frag", nullptr, "postprocessing");
	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("card").Use().SetInteger("image", 0);
	ResourceManager::GetShader("card").SetMatrix4("projection", projection);

	// read the textures file into memory
	// then access the memory, should be faster?

	//load  background texture
	ResourceManager::LoadTexture("textures/Green_Felt.png", GL_TRUE, "background");

	//load start-game texture
	ResourceManager::LoadTexture("textures/StartGame.png", GL_TRUE, "splashpage");

	//load menu icon
	ResourceManager::LoadTexture("textures/menu_.png", GL_TRUE, "menu");

	//load menu2 page
	ResourceManager::LoadTexture("textures/menu2.png", GL_FALSE, "menu2");

	//load card textures
	ResourceManager::LoadTexture("textures/cards/ace_of_diamonds.png", GL_TRUE, "AOD");
	ResourceManager::LoadTexture("textures/cards/ace_of_clubs.png", GL_TRUE, "AOC");
	ResourceManager::LoadTexture("textures/cards/ace_of_hearts.png", GL_TRUE, "AOH");
	ResourceManager::LoadTexture("textures/cards/ace_of_spades.png", GL_TRUE, "AOS");
	ResourceManager::LoadTexture("textures/cards/2_of_clubs.png", GL_TRUE, "C2");
	ResourceManager::LoadTexture("textures/cards/2_of_diamonds.png", GL_TRUE, "D2");
	ResourceManager::LoadTexture("textures/cards/2_of_hearts.png", GL_TRUE, "H2");
	ResourceManager::LoadTexture("textures/cards/2_of_spades.png", GL_TRUE, "S2");
	ResourceManager::LoadTexture("textures/cards/3_of_clubs.png", GL_TRUE, "C3");
	ResourceManager::LoadTexture("textures/cards/3_of_diamonds.png", GL_TRUE, "D3");
	ResourceManager::LoadTexture("textures/cards/3_of_hearts.png", GL_TRUE, "H3");
	ResourceManager::LoadTexture("textures/cards/3_of_spades.png", GL_TRUE, "S3");
	ResourceManager::LoadTexture("textures/cards/4_of_clubs.png", GL_TRUE, "C4");
	ResourceManager::LoadTexture("textures/cards/4_of_diamonds.png", GL_TRUE, "D4");
	ResourceManager::LoadTexture("textures/cards/4_of_hearts.png", GL_TRUE, "H4");
	ResourceManager::LoadTexture("textures/cards/4_of_spades.png", GL_TRUE, "S4");
	ResourceManager::LoadTexture("textures/cards/5_of_clubs.png", GL_TRUE, "C5");
	ResourceManager::LoadTexture("textures/cards/5_of_diamonds.png", GL_TRUE, "D5");
	ResourceManager::LoadTexture("textures/cards/5_of_hearts.png", GL_TRUE, "H5");
	ResourceManager::LoadTexture("textures/cards/5_of_spades.png", GL_TRUE, "S5");
	ResourceManager::LoadTexture("textures/cards/6_of_clubs.png", GL_TRUE, "C6");
	ResourceManager::LoadTexture("textures/cards/6_of_diamonds.png", GL_TRUE, "D6");
	ResourceManager::LoadTexture("textures/cards/6_of_hearts.png", GL_TRUE, "H6");
	ResourceManager::LoadTexture("textures/cards/6_of_spades.png", GL_TRUE, "S6");
	ResourceManager::LoadTexture("textures/cards/7_of_clubs.png", GL_TRUE, "C7");
	ResourceManager::LoadTexture("textures/cards/7_of_diamonds.png", GL_TRUE, "D7");
	ResourceManager::LoadTexture("textures/cards/7_of_hearts.png", GL_TRUE, "H7");
	ResourceManager::LoadTexture("textures/cards/7_of_spades.png", GL_TRUE, "S7");
	ResourceManager::LoadTexture("textures/cards/8_of_clubs.png", GL_TRUE, "C8");
	ResourceManager::LoadTexture("textures/cards/8_of_diamonds.png", GL_TRUE, "D8");
	ResourceManager::LoadTexture("textures/cards/8_of_hearts.png", GL_TRUE, "H8");
	ResourceManager::LoadTexture("textures/cards/8_of_spades.png", GL_TRUE, "S8");
	ResourceManager::LoadTexture("textures/cards/9_of_clubs.png", GL_TRUE, "C9");
	ResourceManager::LoadTexture("textures/cards/9_of_diamonds.png", GL_TRUE, "D9");
	ResourceManager::LoadTexture("textures/cards/9_of_hearts.png", GL_TRUE, "H9");
	ResourceManager::LoadTexture("textures/cards/9_of_spades.png", GL_TRUE, "S9");
	ResourceManager::LoadTexture("textures/cards/10_of_clubs.png", GL_TRUE, "C10");
	ResourceManager::LoadTexture("textures/cards/10_of_diamonds.png", GL_TRUE, "D10");
	ResourceManager::LoadTexture("textures/cards/10_of_hearts.png", GL_TRUE, "H10");
	ResourceManager::LoadTexture("textures/cards/10_of_spades.png", GL_TRUE, "S10");
	ResourceManager::LoadTexture("textures/cards/jack_of_clubs2.png", GL_TRUE, "JOC");
	ResourceManager::LoadTexture("textures/cards/jack_of_diamonds2.png", GL_TRUE, "JOD");
	ResourceManager::LoadTexture("textures/cards/jack_of_hearts2.png", GL_TRUE, "JOH");
	ResourceManager::LoadTexture("textures/cards/jack_of_spades2.png", GL_TRUE, "JOS");
	ResourceManager::LoadTexture("textures/cards/queen_of_clubs2.png", GL_TRUE, "QOC");
	ResourceManager::LoadTexture("textures/cards/queen_of_diamonds2.png", GL_TRUE, "QOD");
	ResourceManager::LoadTexture("textures/cards/queen_of_hearts2.png", GL_TRUE, "QOH");
	ResourceManager::LoadTexture("textures/cards/queen_of_spades2.png", GL_TRUE, "QOS");
	ResourceManager::LoadTexture("textures/cards/king_of_clubs2.png", GL_TRUE, "KOC");
	ResourceManager::LoadTexture("textures/cards/king_of_diamonds2.png", GL_TRUE, "KOD");
	ResourceManager::LoadTexture("textures/cards/king_of_hearts2.png", GL_TRUE, "KOH");
	ResourceManager::LoadTexture("textures/cards/king_of_spades2.png", GL_TRUE, "KOS");

	//set render specific controls
	Renderer = new CardRenderer(ResourceManager::GetShader("card"));
	//Renderer = std::unique_ptr<CardRenderer>(new CardRenderer(ResourceManager::GetShader("card")));
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);


	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("fonts/OCRAEXT.TTF", 24);
}

// populate a vector with bit fields for each card in the deck
void Game::InitBits()
{
	BitPattern halfword_bitshift = { 1 };
	BitPattern upper_nybble_bitshift = { 1 };


	BitPattern halfword_mask = { 0x008000 };
	BitPattern nybble_mask = { 0x080000 };

	// the bit pattern starts at 0x088000
	// using the overloaded bitwise or operator
	BitPattern bit_pattern = (nybble_mask | halfword_mask); // (0x080000 | 0x008000) = 0x088000

	for (int i = 0; i != 52; i++)
	{
		bits = new CardBits;
		// popluating the CardBits vector
		BitPack.push_back(bits);

		if (i == 0)
		{
			BitPack[i]->setID(bit_pattern);

			std::cout << "First block " << std::endl;
			std::cout << std::hex << "bitshift: " << bit_pattern << std::endl;
			std::cout << std::dec << i << std::endl;
		}
		else if ((i % 13) == 0)
		{
			// reset the halfword to 0x008000
			halfword_mask = { 0x008000 };

			// and increment the upper byte nybble by 1
			(nybble_mask) >>= upper_nybble_bitshift;
			bit_pattern = (nybble_mask | halfword_mask);
			std::cout << std::hex << "bit_pattern: " << bit_pattern << std::endl;

			BitPack[i]->setID(bit_pattern);

			std::cout << "Second block " << std::endl;
			std::cout << std::dec << i << std::endl;
		}
		else   // - for every 13 cards shift one of the last 16 bits to the right - 16 bits are known as a halfword
		{
			//update the halfword_mask
			(halfword_mask) >>= halfword_bitshift; // 0x004000
			bit_pattern = (nybble_mask | halfword_mask);
			std::cout << std::hex << "halfword_mask: " << halfword_mask << std::endl;
			std::cout << std::hex << "bit_pattern: " << bit_pattern << std::endl;

			BitPack[i]->setID(bit_pattern);

			std::cout << "Third block " << std::endl;
			std::cout << std::dec << i << std::endl;
		}

	}
	for (int i = 0; i != 52; i++)
		std::cout << std::hex << BitPack[i]->getID() << std::endl;
}

void Game::InitCards()
{
	// initialise the card objects
	// let the pile container be responsible for positioning the card
	// the suitpile will contain the upper left corner position
	// the lefthand tablepile containers wil contain the upper left corner positions as well as -15 offset for every new element added
	// the righthand tablepile conatiners will conatin the uper left corner positions as well as +15 offset for every new element added

	// SOLUTION!!
	// Each object is assigned a default position of (0.0f, 0.0f) ___TICK____
	// These objects are then sent to populate a container called Deck  ___TICK____
	// Then these objects are shuffled within the Deck container __TICK____
	// They are then distributed to all the neccessary piles, Suit and Table. Use a sorting algorithm. Bucket sort algorithm.
	// Checks are conducted to check that the appropriate object is inserted in the correct container ACES only go to empty suit containers
	// The objects are sent to be rendered
	// The offset check is done at rendering using a define directive
	// So neiher the containers nor the renderer is aware of positioning
	// A check is done to differentiate between the SuitPile container
	// In the SuitPile container all the cards are stored in the same position. This is where they will be rendered
	// In the TablePile container only the first card is rendered at the base position
	// All the remaining objects are then rendered with an offset from the base position that is multiplied by their place in th
	AOC = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("AOC"), 0x088000, INITIAL_BALL_VELOCITY);
	AOD = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("AOD"), 0x048000, INITIAL_BALL_VELOCITY);
	AOH = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("AOH"), 0x028000, INITIAL_BALL_VELOCITY);
	AOS = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("AOS"), 0x018000, INITIAL_BALL_VELOCITY);
	C2 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C2"), 0x080008, INITIAL_BALL_VELOCITY);
	D2 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D2"), 0x040008, INITIAL_BALL_VELOCITY);
	H2 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H2"), 0x020008, INITIAL_BALL_VELOCITY);
	S2 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S2"), 0x010008, INITIAL_BALL_VELOCITY);
	C3 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C3"), 0x080010, INITIAL_BALL_VELOCITY);
	D3 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D3"), 0x040010, INITIAL_BALL_VELOCITY);
	H3 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H3"), 0x020010, INITIAL_BALL_VELOCITY);
	S3 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S3"), 0x010010, INITIAL_BALL_VELOCITY);
	C4 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C4"), 0x080020, INITIAL_BALL_VELOCITY);
	D4 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D4"), 0x040020, INITIAL_BALL_VELOCITY);
	H4 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H4"), 0x020020, INITIAL_BALL_VELOCITY);
	S4 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S4"), 0x010020, INITIAL_BALL_VELOCITY);
	C5 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C5"), 0x080040, INITIAL_BALL_VELOCITY);
	D5 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D5"), 0x040040, INITIAL_BALL_VELOCITY);
	H5 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H5"), 0x020040, INITIAL_BALL_VELOCITY);
	S5 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S5"), 0x010040, INITIAL_BALL_VELOCITY);
	C6 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C6"), 0x080080, INITIAL_BALL_VELOCITY);
	D6 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D6"), 0x040080, INITIAL_BALL_VELOCITY);
	H6 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H6"), 0x020080, INITIAL_BALL_VELOCITY);
	S6 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S6"), 0x010080, INITIAL_BALL_VELOCITY);
	C7 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C7"), 0x080100, INITIAL_BALL_VELOCITY);
	D7 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D7"), 0x040100, INITIAL_BALL_VELOCITY);
	H7 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H7"), 0x020100, INITIAL_BALL_VELOCITY);
	S7 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S7"), 0x010100, INITIAL_BALL_VELOCITY);
	C8 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C8"), 0x080200, INITIAL_BALL_VELOCITY);
	D8 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D8"), 0x040200, INITIAL_BALL_VELOCITY);
	H8 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H8"), 0x020200, INITIAL_BALL_VELOCITY);
	S8 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S8"), 0x010200, INITIAL_BALL_VELOCITY);
	C9 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C9"), 0x080400, INITIAL_BALL_VELOCITY);
	D9 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D9"), 0x040400, INITIAL_BALL_VELOCITY);
	H9 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H9"), 0x020400, INITIAL_BALL_VELOCITY);
	S9 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S9"), 0x010400, INITIAL_BALL_VELOCITY);
	C10 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("C10"), 0x080800, INITIAL_BALL_VELOCITY);
	D10 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("D10"), 0x040800, INITIAL_BALL_VELOCITY);
	H10 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("H10"), 0x020800, INITIAL_BALL_VELOCITY);
	S10 = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("S10"), 0x010800, INITIAL_BALL_VELOCITY);
	JOC = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("JOC"), 0x081000, INITIAL_BALL_VELOCITY);
	JOD = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("JOD"), 0x041000, INITIAL_BALL_VELOCITY);
	JOH = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("JOH"), 0x021000, INITIAL_BALL_VELOCITY);
	JOS = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("JOS"), 0x011000, INITIAL_BALL_VELOCITY);
	QOC = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("QOC"), 0x082000, INITIAL_BALL_VELOCITY);
	QOD = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("QOD"), 0x042000, INITIAL_BALL_VELOCITY);
	QOH = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("QOH"), 0x022000, INITIAL_BALL_VELOCITY);
	QOS = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("QOS"), 0x012000, INITIAL_BALL_VELOCITY);
	KOC = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("KOC"), 0x084000, INITIAL_BALL_VELOCITY);
	KOD = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("KOD"), 0x044000, INITIAL_BALL_VELOCITY);
	KOH = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("KOH"), 0x024000, INITIAL_BALL_VELOCITY);
	KOS = new CardObject(1, 1, glm::vec2(CARD_WIDTH, CARD_LENGTH), ResourceManager::GetTexture("KOS"), 0x014000, INITIAL_BALL_VELOCITY);

}

void Game::PopulateDeck()
{
	if (this->State == GAME_INIT || this->State == GAME_RESTART)
	{
		// populating the Deck in order of ace-to-king
		Deck.push_back(AOC);  Deck.push_back(C2);  Deck.push_back(C3);   Deck.push_back(C4);  Deck.push_back(C5);	Deck.push_back(C6);	Deck.push_back(C7);	Deck.push_back(C8); Deck.push_back(C9);	Deck.push_back(C10); Deck.push_back(JOC); Deck.push_back(QOC); Deck.push_back(KOC);
		Deck.push_back(AOD);  Deck.push_back(D2);  Deck.push_back(D3);   Deck.push_back(D4);  Deck.push_back(D5);	Deck.push_back(D6); Deck.push_back(D7);	Deck.push_back(D8);	Deck.push_back(D9);	Deck.push_back(D10); Deck.push_back(JOD); Deck.push_back(QOD); Deck.push_back(KOD);
		Deck.push_back(AOH);  Deck.push_back(H2);  Deck.push_back(H3);   Deck.push_back(H4);  Deck.push_back(H5);	Deck.push_back(H6);	Deck.push_back(H7);	Deck.push_back(H8);	Deck.push_back(H9);	Deck.push_back(H10); Deck.push_back(JOH); Deck.push_back(QOH); Deck.push_back(KOH);
		Deck.push_back(AOS);  Deck.push_back(S2);  Deck.push_back(S3);   Deck.push_back(S4);  Deck.push_back(S5);	Deck.push_back(S6);	Deck.push_back(S7);	Deck.push_back(S8);	Deck.push_back(S9);	Deck.push_back(S10); Deck.push_back(JOS); Deck.push_back(QOS); Deck.push_back(KOS);		
	}
}

void Game::shuffleBits_()
{
	if (this->State == GAME_INIT || this->State == GAME_RESTART)
	{
		// using the fisher yates index to shuffle the deck
		auto currentIndexCounter = BitPack.size();
		for (std::vector<CardBits*>::reverse_iterator iter = BitPack.rbegin(); iter != BitPack.rend(); iter++, --currentIndexCounter)
		{
			int randomIndex = std::rand() % currentIndexCounter; // random(currentIndexCounter);

			if (*iter != BitPack.at(randomIndex))
			{
				std::swap(BitPack.at(randomIndex), *iter);
			}
		}
	}
}

/*void Game::Assign_CardObject_Pos(std::vector<CardBits*> &container, int num)
{
	// pass the vector by reference to the function
	// pass the index by value
	// get the bit pattern of the CardBits object contained at that element
	// container[num]->getID();

	// search through the Deck vector to find a match
	for (std::vector<CardObject*>::iterator iter = Deck.begin(); iter != Deck.end(); ++iter)
	{
		BitPattern temp1, temp2;
		temp1 = (*iter)->getID();
		temp2 = container[num]->getID();
		//std::cout << temp1 << "," << temp2 << std::endl;

		// if the two IDs match then send the copy of the coordinates over
		if (temp1 == temp2)
		{
			// print coordinates in here

			std::cout << temp1 << "," << temp2 << std::endl;
			// dereferencing the pointer and the iterator
			(*iter)->Set(container[num]->GetCoords());
			std::cout << "Within Assign_CardObject_Pos" << std::endl;
			std::cout << (*iter)->getX() << std::endl;
			std::cout << (*iter)->getY() << std::endl;
			

			std::cout << "CardObject position: ";
			(*iter)->DisplayPosition();
			std::cout << "Leaving Assign_CardObject_Pos" << std::endl;
			//std::cin.get();
		}
	}
}*/

void Game::Assign_CardObject_Pos(std::vector<CardBits*> &container, int num)
{
	// pass the vector by reference to the function
	// pass the index by value
	// get the bit pattern of the CardBits object contained at that element
	// container[num]->getID();

	int count = 0;

	// search through the Deck vector to find a match
	for (std::vector<CardObject*>::iterator iter = Deck.begin(); iter != Deck.end(); ++iter, count++)
	{
		BitPattern temp1, temp2;
		temp1 = (*iter)->getID();
		temp2 = container[num]->getID();
		//std::cout << temp1 << "," << temp2 << std::endl;

		// if the two IDs match then send the copy of the coordinates over
		if (temp1 == temp2)
		{
			// updating the CardBit index with the position of the CardObject within the vector
			container[num]->setIndex(count);


			// print coordinates in here

			std::cout << temp1 << "," << temp2 << std::endl;
			// dereferencing the pointer and the iterator
			(*iter)->Set(container[num]->GetCoords());
			std::cout << "Within Assign_CardObject_Pos" << std::endl;
			std::cout << (*iter)->getX() << std::endl;
			std::cout << (*iter)->getY() << std::endl;


			std::cout << "CardObject position: ";
			(*iter)->DisplayPosition();
			std::cout << "Leaving Assign_CardObject_Pos" << std::endl;
			//std::cin.get();
		}
	}
}

// distribute the bits into other bit containers
void Game::distributeBits_()
{
	BitPattern ace = { 0x008000 };

	if (this->State == GAME_INIT || this->State == GAME_RESTART)
	{
		//std::cout << BitPack.size() << std::endl;
		for (std::vector<CardBits*>::iterator iter = BitPack.begin(); iter != BitPack.end(); iter++)
		{
			//if ((*iter)->getRank() == ACE)   // check whether the card is an ACE
			BitPattern temp_;
			temp_ = (*iter)->getID();
			std::cout << "temp: " << temp_ << std::endl;
			//std::cin.get();

			// a bitwise AND operator is used
			// to check whether the card ID and the bit pattern for ACE results in another bit pattern for ACE 
			//if (((*iter)->getID() & ace) == ace)
			if ((temp_ & ace) == ace)
			{
				std::cout << "Inside" << std::endl;

				if (BitFoundation1.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitFoundation1.push_back(std::move((*iter)));

					// assign its position
					BitFoundation1[num]->setPosition(X2, Y1);

					// debugging test
					std::cout << "CardBit position: ";
					BitFoundation1[num]->DisplayPosition();
					//std::cin.get();

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitFoundation1, num);
				}
				else if (BitFoundation2.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitFoundation2.push_back(std::move((*iter)));

					//assign its position
					BitFoundation2[0]->setPosition(X2, Y2);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitFoundation2, num);
				}
				else if (BitFoundation3.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitFoundation3.push_back(std::move((*iter)));

					//assign its position
					BitFoundation3[0]->setPosition(X2, Y3);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitFoundation3, num);
				}
				else if (BitFoundation4.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitFoundation4.push_back(std::move(*iter));

					//assign its position
					BitFoundation4[0]->setPosition(X2, Y4);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitFoundation4, num);
				}
			}
			else //if ((*iter)->getRank() != ACE)
			{
				if (BitPileL1.empty()) //check to see if the vector is empty
				{
					int num = 0;

					// move ace into the new container
					BitPileL1.push_back(std::move((*iter)));

					// assign its position
					BitPileL1[0]->setPosition(X1, Y1);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL1, num);
				}
				else if (BitPileL1.size() > 0 && BitPileL1.size() < 6)
				{
					int j = BitPileL1.size();
					int offset = j * LEFT_OFFSET;

					// move ace into the new container
					BitPileL1.push_back(std::move((*iter)));

					//assign its position
					BitPileL1[j]->setPosition(X1 + offset, Y1);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL1, j);
				}
				else if (BitPileL2.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileL2.push_back(std::move((*iter)));

					//assign its position
					BitPileL2[0]->setPosition(X1, Y2);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL2, num);
				}
				else if (BitPileL2.size() > 0 && BitPileL2.size() < 6)
				{
					int j = BitPileL2.size();
					int offset = j * LEFT_OFFSET;

					// move ace into the new container
					BitPileL2.push_back(std::move((*iter)));

					//assign its position
					BitPileL2[j]->setPosition(X1 + offset, Y2);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL2, j);
				}
				else if (BitPileL3.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileL3.push_back(std::move((*iter)));

					//assign its position
					BitPileL3[0]->setPosition(X1, Y3);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL3, num);
				}
				else if (BitPileL3.size() > 0 && BitPileL3.size() < 6)
				{
					int j = BitPileL3.size();
					int offset = j * LEFT_OFFSET;

					// move ace into the new container
					BitPileL3.push_back(std::move((*iter)));

					//assign its position
					BitPileL3[j]->setPosition(X1 + offset, Y3);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL3, j);
				}
				else if (BitPileL4.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileL4.push_back(std::move((*iter)));

					//assign its position
					BitPileL4[0]->setPosition(X1, Y4);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL4, num);
				}
				else if (BitPileL4.size() > 0 && BitPileL4.size() < 6)
				{
					int j = BitPileL4.size();
					int offset = j * LEFT_OFFSET;

					// move ace into the new container
					BitPileL4.push_back(std::move((*iter)));

					//assign its position
					BitPileL4[j]->setPosition(X1 + offset, Y4);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileL4, j);
				}
				else if (BitPileR1.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileR1.push_back(std::move((*iter)));

					//assign its position
					BitPileR1[0]->setPosition(X3, Y1);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR1, num);
				}
				else if (BitPileR1.size() > 0 && BitPileR1.size() < 6)
				{
					int j = BitPileR1.size();
					int offset = j * RIGHT_OFFSET;

					// move ace into the new container
					BitPileR1.push_back(std::move((*iter)));

					//assign its position
					BitPileR1[j]->setPosition(X3 + offset, Y1);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR1, j);
				}
				else if (BitPileR2.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileR2.push_back(std::move((*iter)));

					//assign its position
					BitPileR2[0]->setPosition(X3, Y2);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR2, num);
				}
				else if (BitPileR2.size() > 0 && BitPileR2.size() < 6)
				{
					int j = BitPileR2.size();
					int offset = j * RIGHT_OFFSET;

					// move ace into the new container
					BitPileR2.push_back(std::move((*iter)));

					//assign its position
					BitPileR2[j]->setPosition(X3 + offset, Y2);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR2, j);
				}
				else if (BitPileR3.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileR3.push_back(std::move((*iter)));

					//assign its position
					BitPileR3[0]->setPosition(X3, Y3);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR3, num);
				}
				else if (BitPileR3.size() > 0 && BitPileR3.size() < 6)
				{
					int j = BitPileR3.size();
					int offset = j * RIGHT_OFFSET;

					// move ace into the new container
					BitPileR3.push_back(std::move((*iter)));

					//assign its position
					BitPileR3[j]->setPosition(X3 + offset, Y3);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR3, j);
				}
				else if (BitPileR4.empty()) //check to see if the vector is empty
				{
					int num = 0;
					// move ace into the new container
					BitPileR4.push_back(std::move((*iter)));

					//assign its position
					BitPileR4[0]->setPosition(X3, Y4);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR4, num);
				}
				else if (BitPileR4.size() > 0 && BitPileR4.size() < 6)
				{
					int j = BitPileR4.size();
					int offset = j * RIGHT_OFFSET;

					// move ace into the new container
					BitPileR4.push_back(std::move((*iter)));

					//assign its position
					BitPileR4[j]->setPosition(X3 + offset, Y4);

					// the position of card objects can be set from within here
					this->Assign_CardObject_Pos(BitPileR4, j);
				}
			}
		}
	}
	this->Print_Object_Pos(Deck);
}

void Game::Print_Object_Pos(std::vector<CardObject*> &container)
{
	for (auto iter = container.begin(); iter != container.end(); ++iter)
		(*iter)->Print();
}

void Game::Clear_Iter_Vectors()
{
	iterFoundation1.clear();
	iterFoundation2.clear();
	iterFoundation3.clear();
	iterFoundation4.clear();
	iterPileL1.clear();
	iterPileL2.clear();
	iterPileL3.clear();
	iterPileL4.clear();
	iterPileR1.clear();
	iterPileR2.clear();
	iterPileR3.clear();
	iterPileR4.clear();
}

void Game::Create_Iter_Vector(std::vector<CardObject*> &Iter_vec)
{
	for (auto iter = Deck.begin(); iter != Deck.end(); ++iter)
	{
		// 1st duodecad
		if ((*iter)->getX() <= X1 && (*iter)->getY() == Y1)
		{
			//if ((*iter)->getY() == Y1)
			//{
			iterPileL1.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() == X2 && (*iter)->getY() == Y1) // 2nd doudecad
		{
			iterFoundation1.push_back(*iter);
		}
		else if ((*iter)->getX() >= X3 && (*iter)->getY() == Y1) // 3rd duodecad
		{
			//if ((*iter)->getY() == Y1)
			//{
			iterPileR1.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() <= X1 && (*iter)->getY() == Y2)// 4th duodecad
		{
			//if ((*iter)->getY() == Y2)
			//{
			iterPileL2.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() == X2 && (*iter)->getY() == Y2) // 5th duodecad
		{
			iterFoundation2.push_back(*iter);
		}
		else if ((*iter)->getX() >= X3 && (*iter)->getY() == Y2) // 6th duodecad
		{
			//if ((*iter)->getY() == Y2)
			//{
			iterPileR2.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() <= X1 && (*iter)->getY() == Y3) // 7th duodecad
		{
			//if ((*iter)->getY() == Y3)
			//{
			iterPileL3.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() == X2 && (*iter)->getY() == Y3) // 8th duodecad
		{
			iterFoundation3.push_back(*iter);
		}
		else if ((*iter)->getX() >= X3 && (*iter)->getY() == Y3) // 9th duodecad
		{
			//if ((*iter)->getY() == Y3)
			//{
			iterPileR3.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() <= X1 && (*iter)->getY() == Y4)    // 10th duodecad
		{
			//if ((*iter)->getY() == Y4)
			//{
			iterPileL4.push_back(*iter);
			//}
		}
		else if ((*iter)->getX() == X2 && (*iter)->getY() == Y4)// 11th duodecad
		{
			iterFoundation4.push_back(*iter);
		}
		else if ((*iter)->getX() >= X3 && (*iter)->getY() == Y4)  // 12th duodecad
		{
			//if ((*iter)->getY() == Y4)
			//{
			iterPileR4.push_back(*iter);
			//}
		}
	}
}

void Game::Init_Iter_Vec()
{
	this->Create_Iter_Vector(iterFoundation1);
	this->Create_Iter_Vector(iterFoundation2);
	this->Create_Iter_Vector(iterFoundation3);
	this->Create_Iter_Vector(iterFoundation4);
	this->Create_Iter_Vector(iterPileL1);
	this->Create_Iter_Vector(iterPileL2);
	this->Create_Iter_Vector(iterPileL3);
	this->Create_Iter_Vector(iterPileL4);
	this->Create_Iter_Vector(iterPileR1);
	this->Create_Iter_Vector(iterPileR2);
	this->Create_Iter_Vector(iterPileR3);
	this->Create_Iter_Vector(iterPileR4);
}

void Game::Sort_Iter_Vec()
{
	// sort the iterators using lambda functions
	// 1st duodecad right_to_left
	std::sort(iterPileL1.begin(), iterPileL1.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() > b->getX(); });

	// 3rd duodecad left_to_right
	std::sort(iterPileR1.begin(), iterPileR1.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() < b->getX(); });

	// 4th duodecad right_to_left
	std::sort(iterPileL2.begin(), iterPileL2.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() > b->getX(); });

	// 6th duodecad left_to_right
	std::sort(iterPileR2.begin(), iterPileR2.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() < b->getX(); });

	// 7th duodecad right_to_left
	std::sort(iterPileL3.begin(), iterPileL3.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() > b->getX(); });

	// 9th duodecad left_to_right
	std::sort(iterPileR3.begin(), iterPileR3.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() < b->getX(); });

	// 10 duodecad right_to_left
	std::sort(iterPileL4.begin(), iterPileL4.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() > b->getX(); });

	// 12 duodecad left_to_right
	std::sort(iterPileR4.begin(), iterPileR4.end(), [](CardObject* a, CardObject* b)-> float { return a->getX() < b->getX(); });
}

void Game::Print_Iter_Vec_Pos(std::vector<CardObject*> &vec)
{
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		std::cout << "X: " << (*iter)->getX() << ", Y: " << (*iter)->getY() << std::endl;
	}
}


void Game::ResetGame()
{
	if (this->State == GAME_RESTART)
	{
		// clear the BitPiles
		// the Bit Foundations
		BitFoundation1.clear();
		BitFoundation2.clear();
		BitFoundation3.clear();
		BitFoundation4.clear();

		// the Bit piles
		BitPileL1.clear();
		BitPileL2.clear();
		BitPileL3.clear();
		BitPileL4.clear();
		BitPileR1.clear();
		BitPileR2.clear();
		BitPileR3.clear();
		BitPileR4.clear();

		this->Clear_Iter_Vectors();

		this->shuffleBits_();

		this->distributeBits_();

		this->Init_Iter_Vec();

		this->Sort_Iter_Vec();

		this->clockStatus();

		Timer::instance()->ResetDisplay();
		this->Moves = 0;
		this->Score = 0;
	}
}

//render the textures 
void Game::Render()
{
	if (this->State == GAME_INTRO)
	{
		// Draw splashpage
		//Renderer->DrawCard(ResourceManager::GetTexture("splashpage"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("splashpage"), 0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));


#if defined(_ANDROID_)
		Text->RenderText("Touch the screen", (this->Width / 2), (this->Height / 2), 0.85f);
#else
		Text->RenderText("Click the mouse over the screen or tap the screen to start.", 12.0f, this->Height / 2, 0.85f);
#endif
		
	}
    else if (this->State == GAME_ACTIVE
		|| (this->State == GAME_SMOOTHMOVER && logicGuard == 100)
		|| (this->State == GAME_UPDATE && logicGuard == 100)
		|| (this->State == GAME_PLACEINNEWPILE && logicGuard == 100))
	{
#if defined(_ANDROID_)
#define SCALE 1.75f
#define ADJUSTMENT_X 0.9922f
#define ADJUSTMENT_HEIGHT 0.0714f
#else
#define SCALE 1.0f
#define ADJUSTMENT_X 0.9375f      // 750/800
#define ADJUSTMENT_HEIGHT 0.0583f
#endif
		// Begin rendering to postprocessing quad
		//Effects->BeginRender();

		// Draw background
		//Renderer->DrawCard(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("background"), 0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));

		// Draw menu
		//Renderer->DrawCard(ResourceManager::GetTexture("menu"), glm::vec2(750, 560), glm::vec2(45, 35), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));
		//Renderer->DrawCard(ResourceManager::GetTexture("menu"), glm::vec2((WIDTH * ADJUSTMENT_X),(HEIGHT * 0.9333)), glm::vec2((WIDTH * 0.0563), (HEIGHT * ADJUSTMENT_HEIGHT)), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));
		Renderer->DrawCard(ResourceManager::GetTexture("menu"),(WIDTH * ADJUSTMENT_X), (HEIGHT * 0.9333), glm::vec2((WIDTH * 0.0563), (HEIGHT * ADJUSTMENT_HEIGHT)), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));


		// 1st row
		int i = 0;
		for (auto it = iterPileL1.begin(); it != iterPileL1.end(); ++it, i++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileL1[i]->Draw(*Renderer);
				// debugging
				//std::cout << "L1: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "L1: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		int j = 0;
		for (auto it = iterFoundation1.begin(); it != iterFoundation1.end(); ++it, j++)
		{
			//(*it)->Draw(*Renderer);
			iterFoundation1[j]->Draw(*Renderer);
			// debugging
			//std::cout << "F1: " << std::dec << (*it)->getX() << std::endl;
			//std::cout << "F1: " << std::dec << (*it)->getY() << std::endl;
		}

		int k = 0;
		for (auto it = iterPileR1.begin(); it != iterPileR1.end(); ++it, k++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileR1[k]->Draw(*Renderer);
				// debugging
				//std::cout << "R1: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "R1: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		// 2nd row
		int l = 0;
		for (auto it = iterPileL2.begin(); it != iterPileL2.end(); ++it, l++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileL2[l]->Draw(*Renderer);
				// debugging
				//std::cout << "L2: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "L2: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		int m = 0;
		for (auto it = iterFoundation2.begin(); it != iterFoundation2.end(); ++it, m++)
		{
			//(*it)->Draw(*Renderer);
			iterFoundation2[m]->Draw(*Renderer);
			// debugging
			//std::cout << "F2: " << std::dec << (*it)->getX() << std::endl;
			//std::cout << "F2: " << std::dec << (*it)->getY() << std::endl;
		}

		int n = 0;
		for (auto it = iterPileR2.begin(); it != iterPileR2.end(); ++it, n++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileR2[n]->Draw(*Renderer);
				// debugging
				//std::cout << "R2: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "R2: " << std::dec << (*it)->getY() << std::endl;
			}
		}


		// 3rd row
		int o = 0;
		for (auto it = iterPileL3.begin(); it != iterPileL3.end(); ++it, o++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileL3[o]->Draw(*Renderer);
				// debugging
				//std::cout << "L3: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "L3: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		int p = 0;
		for (auto it = iterFoundation3.begin(); it != iterFoundation3.end(); ++it, p++)
		{
			//(*it)->Draw(*Renderer);
			iterFoundation3[p]->Draw(*Renderer);
			// debugging
			//std::cout << "F3: " << std::dec << (*it)->getX() << std::endl;
			//std::cout << "F3: " << std::dec << (*it)->getY() << std::endl;
		}

		int q = 0;
		for (auto it = iterPileR3.begin(); it != iterPileR3.end(); ++it, q++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileR3[q]->Draw(*Renderer);
				// debugging
				//std::cout << "R3: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "R3: " << std::dec << (*it)->getY() << std::endl;
			}
		}


		// 4th row
		int r = 0;
		for (auto it = iterPileL4.begin(); it != iterPileL4.end(); ++it, r++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileL4[r]->Draw(*Renderer);
				// debugging
				//std::cout << "L4: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "L4: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		int s = 0;
		for (auto it = iterFoundation4.begin(); it != iterFoundation4.end(); ++it, s++)
		{
			//(*it)->Draw(*Renderer);
			//if ((*it)->getID() != renderAtTheEnd)
				iterFoundation4[s]->Draw(*Renderer);
			
			// debugging
			//std::cout << "F4: " << std::dec << (*it)->getX() << std::endl;
			//std::cout << "F4: " << std::dec << (*it)->getY() << std::endl;
		}

		int t = 0;
		for (auto it = iterPileR4.begin(); it != iterPileR4.end(); ++it, t++)
		{
			if ((*it)->getID() != renderAtTheEnd)
			{
				//(*it)->Draw(*Renderer);
				iterPileR4[t]->Draw(*Renderer);
				// debugging
				//std::cout << "R4: " << std::dec << (*it)->getX() << std::endl;
				//std::cout << "R4: " << std::dec << (*it)->getY() << std::endl;
			}
		}

		//this->RenderCardBit();
		
		// rendering the CardObject in the Bit_temp
		for (auto iter = Deck.begin(); iter != Deck.end(); ++iter)
		{
			if ((*iter)->getID() == renderAtTheEnd)
			{
				(*iter)->Draw(*Renderer);
			}
		}

		// resorting the iter vectors
		//this->Sort_Iter_Vec();
		
/*
#if defined(_ANDROID_)
		//rendering the .png file stored in the temp2 vector
		// specifically to overcome android context problem
		if (CardInTempCntr == true)
		{
			temp2[0]->Draw(*Renderer);
		}

#endif
*/
		std::stringstream s_moves, s_score, s_hr, s_min, s_sec;
		s_moves << this->Moves;
		//Text->RenderText("Moves:" + s_moves.str(), 610.0f, 580.0f, 1.0f); // 580 - replaced by 570
		Text->RenderText("Moves: " + s_moves.str(), (float)(WIDTH * 0.7625), (float)(HEIGHT * 0.95), SCALE);

		s_score << this->Score;
		//Text->RenderText("Score: " + s_score.str(), 330.0f, 580.0f, 1.0f);
		Text->RenderText("Score: " + s_score.str(), (float)(WIDTH * 0.4125), (float)(HEIGHT * 0.95), SCALE);

		s_hr << Timer::instance()->getHr();
		s_min << Timer::instance()->getMin();
		s_sec << Timer::instance()->getSec();
		//Text->RenderText("Time: " + s_hr.str() + ":" + s_min.str() + ":" + s_sec.str(), 10.0f, 580.0f, 1.0f);
		Text->RenderText("Time: " + s_hr.str() + ":" + s_min.str() + ":" + s_sec.str(), (float)(WIDTH * 0.0125), (float)(HEIGHT * 0.95), SCALE);
		
		//Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2, 1.0f);
		//Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2 + 20.0f, 0.75f);
		// Text->RenderText("You WON!!!", 320.0f, this->Height / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));

		// End rendering to postprocessing quad
		//Effects->EndRender();

		// Render postprocessing quad
		//Effects->Render();
	}
	else if (this->State == GAME_MENU)
	{
#if !defined(_ANDROID_)
		#define	FONTSCALE 0.85f
#else
		#define FONTSCALE 2.0f
#endif
		// Draw splashpage
		//Renderer->DrawCard(ResourceManager::GetTexture("menu2"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("menu2"), 0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));


		//Text->RenderText("INSTRUCTIONS", 12.0f, 100.0f, 0.85f);
		Text->RenderText("INSTRUCTIONS", (WIDTH * 0.015), (HEIGHT * 0.1667), FONTSCALE);

		//Text->RenderText("SCORE SYSTEM", 12.0f, 200.0f, 0.85f);
		Text->RenderText("SCORE SYSTEM", (WIDTH * 0.015), (HEIGHT * 0.3333), FONTSCALE);

		//Text->RenderText("RESUME", 12.0f, 300.0f, 0.85f);
		Text->RenderText("RESUME", (WIDTH * 0.015), (HEIGHT * 0.5), FONTSCALE);

		//Text->RenderText("NEW GAME", 12.0f, 400.0f, 0.85f);
		Text->RenderText("NEW GAME", (WIDTH * 0.015), (HEIGHT * 0.6667), FONTSCALE);
	}
	else if (this->State == GAME_INSTRUCTIONS)
	{
#if !defined(_ANDROID_)
#define	FONTSCALE 0.85f
#define ADJUSTMENT_Y 0.9667f
#else
#define FONTSCALE 2.0f
#define ADJUSTMENT_Y 0.95f
#endif
		// Draw splashpage
		//Renderer->DrawCard(ResourceManager::GetTexture("menu2"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("menu2"),0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));

		//Text->RenderText("INSTRUCTIONS", 12.0f, 100.0f, 0.85f);
		Text->RenderText("INSTRUCTIONS", (WIDTH * 0.012), (HEIGHT * 0.1667), FONTSCALE);

		//Text->RenderText("EXIT", 700.0f, 580.0f, 0.85f);
		Text->RenderText("EXIT", (WIDTH * 0.875), (HEIGHT * ADJUSTMENT_Y), FONTSCALE);
	}
	else if (this->State == GAME_SCORESYSTEMS)
	{
#if !defined(_ANDROID_)
#define	FONTSCALE 0.85f
#define ADJUSTMENT_Y 0.9667f
#else
#define FONTSCALE 2.0f
#define ADJUSTMENT_Y 0.95f
#endif
		// Draw splashpage
		//Renderer->DrawCard(ResourceManager::GetTexture("menu2"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("menu2"), 0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));

		//Text->RenderText("SCORING SYSTEM", 12.0f, 100.0f, 0.85f);
		Text->RenderText("SCORING SYSTEM", (WIDTH * 0.012), (HEIGHT * 0.1667), FONTSCALE);

		//Text->RenderText("EXIT", 700.0f, 580.0f, 0.85f);
		Text->RenderText("EXIT", (WIDTH * 0.875), (HEIGHT * ADJUSTMENT_Y), FONTSCALE);
	}
	else if (this->State == GAME_RESTART)
	{
		// Draw background
		//Renderer->DrawCard(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));
		Renderer->DrawCard(ResourceManager::GetTexture("background"), 0.0f, 0.0f, glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(0.0f, 0.4f, 0.1f));

		// Draw menu
		//Renderer->DrawCard(ResourceManager::GetTexture("menu"), glm::vec2(750, 560), glm::vec2(45, 35), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));
		//Renderer->DrawCard(ResourceManager::GetTexture("menu"), glm::vec2((WIDTH * ADJUSTMENT_X), (HEIGHT * 0.9333)), glm::vec2((WIDTH * 0.0563), (HEIGHT * ADJUSTMENT_HEIGHT)), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));
		Renderer->DrawCard(ResourceManager::GetTexture("menu"),(WIDTH * ADJUSTMENT_X), (HEIGHT * 0.9333), glm::vec2((WIDTH * 0.0563), (HEIGHT * ADJUSTMENT_HEIGHT)), 0.0f, glm::vec3(0.0f, 0.4f, 0.0f));

	}

	//reseting the mouseClick to allow the selection loop to continue
	if (mouseClick)
	{
		doOnlyOnce = 0;
		mouseClick = false;
	}
}

/*void Game::RenderCardBit()
{
	// rendering the CardObject in the Bit_temp
	for (auto iter = Deck.begin(); iter != Deck.end(); ++iter)
	{
		if ((*iter)->getID() == renderAtTheEnd)
		{
			(*iter)->Draw(*Renderer);
		}
	}
}*/

void Game::Quit(SDL_Event *ev)
{
	if (ev->type == SDL_QUIT)
	{
		shouldQuit = true;
	}
	else if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_ESCAPE)
		{
			shouldQuit = true;
		}
	}
}

void Game::processInput(SDL_Event *ev, GLfloat dt)
{
	// all the input is taken from the SDL_PollEvents at this level
	if (this->State == GAME_INTRO)
	{
		this->Render();

		std::cout << "Render the splash page" << std::endl;

#if !defined(_ANDROID_)
		// a function operates with the quit and esc requests
		this->Quit(ev);

		if (ev->type == SDL_MOUSEBUTTONDOWN)
		{
			this->State = GAME_ACTIVE;
		}
		else if (ev->type == SDL_MOUSEBUTTONUP)
		{
			this->State = GAME_INTRO;
		}
#else
		if (ev->type == SDL_FINGERDOWN)
		{
			this->State = GAME_ACTIVE;
		}
		else if (ev->type == SDL_FINGERUP)
		{
			this->State = GAME_INTRO;
		}
#endif
	}
	else if (this->State == GAME_MENU)
	{
		this->Render();

		std::cout << "Render the menu page" << std::endl;
		//#if !defined(_ANDROID_)

		// a function operates with the quit and esc requests
		this->Quit(ev);

		if (ev->type == SDL_MOUSEBUTTONDOWN)
		{
			this->MenuSelection(ev->button.x, ev->button.y);
		}
		else if (ev->type == SDL_MOUSEBUTTONUP)
		{
			// do nothing
		}
		//#else
		//			if (ev->type == SDL_FINGERDOWN)
		//			{
		//				touchLocationPtr->x = ev->tfinger.x * WIDTH;
		//				touchLocationPtr->y = ev->tfinger.y * HEIGHT;
		//				//this->MenuSelectionTouch(ev->tfinger.x, ev->tfinger.y);
		//				this->MenuSelectionTouch(touchLocationPtr->x, touchLocationPtr->y);
		//			}
		//			else if (ev->type == SDL_FINGERUP)
		//			{
		//				//do nothing
		//			}
		//#endif
	}
	else if (this->State == GAME_ACTIVE)
	{

		this->Render();
		this->clockStatus();
		this->ActivatedClock();


		//#if defined(_ANDROID_)
		//rendering the .png file stored in the temp2 vector
		// specifically to overcome android context problem
		//		if (CardInTempCntr == true)
		//		{
		//			temp2[0]->Draw(*Renderer);
		//		}
		//		
		//#endif

		//this->DoDrag(xpos, ypos);
		

		{
			std::cout << "I'm in the GAME_ACTIVE while-loop" << std::endl;
		}
	}
	else if (this->State == GAME_INSTRUCTIONS)
	{
		this->Render();
		std::cout << "Game Instructions page" << std::endl;
		{
			
			// a function operates with the quit and esc requests
			this->Quit(ev);

			if (ev->type == SDL_MOUSEBUTTONDOWN)
			{
				//clickA = true;
				std::cout << "CLickA true " << std::endl;
				//this->InstructionsFSM(STATE_CLICK);
				this->InstructionExit(ev->button.x, ev->button.y);
			}
			else if (ev->type == SDL_MOUSEBUTTONUP)
			{
				//clickA = false;
				std::cout << "CLickA false " << std::endl;
				//this->InstructionsFSM(STATE_UNCLICK);
			}
		}
	}
	else if (this->State == GAME_SCORESYSTEMS)
	{
		std::cout << "In the scoresystems page" << std::endl;
		this->Render();
		{
			
			// a function operates with the quit and esc requests
			this->Quit(ev);

			if (ev->type == SDL_MOUSEBUTTONDOWN)
			{
				//clickA = true;
				//std::cout << "CLickA true " << std::endl;
				//this->ScoringSystem(STATE_CLICK);
				this->ScoringSystemExit(ev->button.x, ev->button.y);
			}
			else if (ev->type == SDL_MOUSEBUTTONUP)
			{
				//clickA = false;
				//std::cout << "CLickA false " << std::endl;
				//this->ScoringSystem(STATE_UNCLICK);
			}
		}
	}
	else if (this->State == GAME_RESTART)
	{
		
		// a function operates with the quit and esc requests
		this->Quit(ev);

		std::cout << "In the game restart page" << std::endl;
		this->Render();
		this->ResetGame();
		this->PopulateDeck();
		
		this->ClockState = OFF;
		//this->setClockToFalse();
		this->State = GAME_ACTIVE;
	}
	else if (this->State == GAME_WIN)
	{
		// a function operates with the quit and esc requests
		this->Quit(ev);
	}
	else if (this->State == GAME_SMOOTHMOVER)
	{
		// a function operates with the quit and esc requests
		this->Quit(ev);

		std::cout << " In the Game_SmoothMover " << std::endl;
		//std::cin.get();
		this->clockStatus();
		this->ActivatedClock();
		this->Render();

		this->RecursiveRenderer();

		std::cout << "GAME_SMOOTHMOVER" << std::endl;
	}
	else if (this->State == GAME_UPDATE)
	{
		// a function operates with the quit and esc requests
		this->Quit(ev);

		std::cout << "GAME_UPDATE" << std::endl;

		this->clockStatus();
		this->ActivatedClock();

		this->Render();
		this->Update(dt);


		//#if defined(_ANDROID_)
		// test to check that the conditions are correct  
		//		{
		//			temp2[0]->Draw(*Renderer);
		//		}
		//#endif


	}
	else if (this->State == GAME_PLACEINNEWPILE)
	{
		// a function operates with the quit and esc requests
		this->Quit(ev);

		this->Render();
	    this->moveToPile();
		this->Clear_Iter_Vectors();
		this->Init_Iter_Vec();
		this->Sort_Iter_Vec();

		std::cout << "GAME_PLACEINNEWPILE" << std::endl;
	}
}

void Game::moveCard(int mxp, int myp)
{
	//std::cout << "XPOS: " << **mxp << '\t' << " YPOS: " << **myp << std::endl;
	const int mx = mxp;
	const int my = myp;
	moveTexture = std::make_pair(mx, my);
	//std::cout << "makeTexture.first:  " << std::setprecision(3) << moveTexture.first << std::endl;
	//std::cout << "makeTexture.second: " << std::setprecision(3) << moveTexture.second << std::endl;
}

/*void Game::captureCoords(int **xp, int **yp)
{
	//std::cout << "XPOS: " << **xp << '\t' << " YPOS: " << **yp << std::endl;
	int x = **xp;
	int y = **yp;
	Coords = std::make_pair(x, y);
	//std::cout << "Coords.first:  " << std::setprecision(3) << Coords.first << std::endl;
	//std::cout << "Coords.second: " << std::setprecision(3) << Coords.second << std::endl;
}*/

/*********************************************************************************************************************/
/*********************************************************************************************************************/

///<summary> These functions deal with the handling of player input </summary>
void Game::HandleMouseDownEvent(SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_LEFT)
	{
		// this variable receives the return string value
		this->MouseDown(event->button.x, event->button.y);

		startDrag = true;
		this->bruteForceSearch();
		//SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

int Game::HandleMouseMoveEvent(SDL_Event *event)
{
	if (event->motion.state == SDL_BUTTON(1) && startDrag)
		return this->InitDrag(event->motion.x, event->motion.y);
	else return 1;
}

void Game::HandleMouseUpEvent(SDL_Event *event)
{
	if (logicGuard == 100 && !Bit_temp.empty())
	{
		if (startDrag)
		{
			startDrag = false;
			//if (event->button.state == SDL_RELEASED)
			this->ReleaseCard(event->motion.x, event->motion.y);
		}
	}
}

void Game::HandleFingerTouchDownEvent(SDL_Event *event)
{
#if defined(_ANDROID_)
	// CREATE A UNIQUE FINGERDOWN FUNCTION
	this->MouseDown(touchLocationPtr->x, touchLocationPtr->y);

	startDrag = true;
	this->bruteForceSearch();

#endif		
}

int Game::HandleFingerMoveEvent(SDL_Event *event)
{
#if defined(_ANDROID_)
	//if (event->motion.state == SDL_BUTTON(1) && startDrag)
	//	return this->InitDrag(event->motion.x, event->motion.y);
	//if(touchLocatiionPtr)
	if (startDrag)
		return this->InitDrag(touchLocationPtr->x, touchLocationPtr->y);
	else return 1;
#else
	return 1;
#endif
}

void Game::HandleFingerUpEvent(SDL_Event *event)
{
#if defined(_ANDROID_)
	if (logicGuard == 100 && !Bit_temp.empty())
	{
		if (startDrag)
		{
			startDrag = false;
			//if (event->button.state == SDL_RELEASED)
			this->ReleaseCard(touchLocationPtr->x, touchLocationPtr->y);
		}
	}
#endif
}

// Handling the card drag
int Game::InitDrag(float x, float y)
{
	//std::cout << "Initialising the drag function" << std::endl;
	//std::cout << "DRAG-QUEEN!" << std::endl;
	//std::cout << "xCoord: " << x << "," << " yCoord: " << y << std::endl;

	xpos = x;
	ypos = y;

	this->DoDrag(x, y);

	/*if (Bit_temp.size() != 0)
	{
		Bit_temp[0]->setPosition(x, y);
		std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
		std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;

		this->AssignCoords(x, y);
	}*/
	return 1;
}

int Game::DoDrag(float mx, float my)
{
	int x = mx;
	int y = my;

	if (startDrag == true && logicGuard == 100 && !Bit_temp.empty())
	{
		std::cout << "Entering the DoDrag function" << std::endl;

		std::cout << "SIZE OF TEMPORARY FUNCTION: " << Bit_temp.size() << std::endl;
		if (Bit_temp.size() != 0)
		{
			//std::cout << "SIZE2: " << temp2.size() << std::endl;
			//Bit_temp[0]->setPosition(glm::vec2(x, y));
			Bit_temp[0]->setPosition(x, y);
			std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
			std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;

			this->AssignCoords2(x, y);

//#if defined(_ANDROID_)
//			if (CardInTempCntr == true)
//			{
//				temp2[0]->Draw(*Renderer);
//			}
//#else

			//Bit_temp[0]->Draw(*Renderer);
//#endif
		}

		//std::cout << "TOKEN: " << token << std::endl;
// execute this only once
// until the guard conditions are reset
		std::cout << "Leaving the DoDrag function" << std::endl;

	}
	return 1;
}

void Game::ReleaseCard(float x, float y)
{
	if (logicGuard == 100 && !Bit_temp.empty())
	{
		std::cout << "Within ReleaseCard" << std::endl;
		std::cout << "xCoord: " << x << "," << " yCoord: " << y << std::endl;
		//moveTexture = std::make_pair(x, y);
		//std::cin.get();
		std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
		std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;
		std::cout << "Still within release card. Calling AssignCoords" << std::endl;
		this->AssignCoords2(x, y);

		//std::cin.get();
		this->State = GAME_SMOOTHMOVER;
		
		// restting xpos and ypos
		xpos = 0;
		ypos = 0;
	}
}

/**********************************************************************************************************/
/**********************************************************************************************************/
///<summary> These functions deal with the selection and movement of card objects </summay>

// the function that works out the region that the mouse clicks on
void Game::MouseDown(float x, float y)
{
	// this function checks to see which region the mouse is in
	// which region you touch
	// a returns a value depending on where this is
#if defined(_ANDROID_)
	if (this->State == GAME_INTRO)
	{
		if (x >= 0 && x <= WIDTH && y >= 0 && y <= HEIGHT)
		{
			this->State = GAME_ACTIVE;
        }
	}

	if (this->State == GAME_MENU)
	{
		this->MenuSelectionTouch(touchLocationPtr->x, touchLocationPtr->y);
	}

	if (this->State == GAME_INSTRUCTIONS)
	{
		this->InstructionExit(touchLocationPtr->x, touchLocationPtr->y);
	}

	if (this->State == GAME_SCORESYSTEMS)
	{
		this->ScoringSystemExit(touchLocationPtr->x, touchLocationPtr->y);
	}

#endif


	if (this->State == GAME_ACTIVE)
	{
		// BitPileL1		
		if (x >= (X1 + (BitPileL1.size() * LEFT_OFFSET))
			&& x <= ((X1 + CARD_WIDTH) + (BitPileL1.size() * LEFT_OFFSET))
			&& y >= Y1 && y <= Y1 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileL1_;
			
			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileL1_;

			this->MovingCardFromPileCntrToTempCntr(BitPileL1, iterPileL1, x, y);
		}
			
		// BitPileL2
		if (x >= (X1 + (BitPileL2.size() * LEFT_OFFSET))
			&& x <= ((X1 + CARD_WIDTH) + (BitPileL2.size() * LEFT_OFFSET))
			&& y >= Y2 && y <= Y2 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileL2_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileL2_;

			this->MovingCardFromPileCntrToTempCntr(BitPileL2, iterPileL2, x, y);
		}

		// BitPileL3
		if (x >= (X1 + (BitPileL3.size() * LEFT_OFFSET))
			&& x <= ((X1 + CARD_WIDTH) + (BitPileL3.size() * LEFT_OFFSET))
			&& y >= Y3 && y <= Y3 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileL3_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileL3_;
			
			this->MovingCardFromPileCntrToTempCntr(BitPileL3, iterPileL3, x, y);
		}
		
		// BitPileL4
		if (x >= (X1 + (BitPileL4.size() * LEFT_OFFSET))
			&& x <= ((X1 + CARD_WIDTH) + (BitPileL4.size() * LEFT_OFFSET))
			&& y >= Y4 && y <= Y4 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileL4_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileL4_;

			this->MovingCardFromPileCntrToTempCntr(BitPileL4, iterPileL4, x, y);
		}
		
		//TablePileR1
		if (x >= (X3 + (BitPileR1.size() * RIGHT_OFFSET))
			&& x <= ((X3 + CARD_WIDTH) + (BitPileR1.size() * RIGHT_OFFSET))
			&& y >= Y1 && y <= Y1 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileR1_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileR1_;

			this->MovingCardFromPileCntrToTempCntr(BitPileR1, iterPileR1, x, y);
		}

		// BitPileR2
		if (x >= (X3 + (BitPileR2.size() * RIGHT_OFFSET))
			&& x <= ((X3 + CARD_WIDTH) + (BitPileR2.size() * RIGHT_OFFSET))
			&& y >= Y2 && y <= Y2 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileR2_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileR2_;

			this->MovingCardFromPileCntrToTempCntr(BitPileR2, iterPileR2, x, y);
		}

		//BitPileR3
		if (x >= (X3 + (BitPileR3.size() * RIGHT_OFFSET))
			&& x <= ((X3 + CARD_WIDTH) + (BitPileR3.size() * RIGHT_OFFSET))
			&& y >= Y3 && y <= Y3 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileR3_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileR3_;

			this->MovingCardFromPileCntrToTempCntr(BitPileR3, iterPileR3, x, y);
		}

		//BitPileR4
		if (x >= (X3 + (BitPileR4.size() * RIGHT_OFFSET))
			&& x <= ((X3 + CARD_WIDTH) + (BitPileR4.size() * RIGHT_OFFSET))
			&& y >= Y4 && y <= Y4 + CARD_LENGTH)
		{
			this->TurnClockOn();

			// assigning the relevant candidate bitpattern to the BitHolder static variable
			BitHolder = _BitPileR4_;

			// assigning the relevant iterpile bitpattern to the pile1 static variable
			pile1 = _iterPileR4_;

			this->MovingCardFromPileCntrToTempCntr(BitPileR4, iterPileR4, x, y);
		}
		
		// Menu button
		//if (x >= (750) && x <= (795) && y >= 560 && y <= 595)
#if defined(_ANDROID_)
		//if(x >= (WIDTH * ADJUSTMENT_X) && x <= ((WIDTH * ADJUSTMENT_X) + (WIDTH * 0.0563) + (WIDTH * 0.0563)) && y >= (HEIGHT * 0.9333) && y <=  ((HEIGHT * 0.9333) + (HEIGHT * ADJUSTMENT_HEIGHT) + (HEIGHT * ADJUSTMENT_HEIGHT)))
		if (x >= (WIDTH * 0.7625) && x <= ((WIDTH * ADJUSTMENT_X) + (WIDTH * 0.0563) + (WIDTH * 0.0563)) && y >= (HEIGHT * 0.9333) && y <= ((HEIGHT * 0.9333) + (HEIGHT * ADJUSTMENT_HEIGHT) + (HEIGHT * ADJUSTMENT_HEIGHT)))
		{
			this->State = GAME_MENU;
		}
#else
		if (x >= (WIDTH * ADJUSTMENT_X) && x <= ((WIDTH * ADJUSTMENT_X) + (WIDTH * 0.0563)) && y >= (HEIGHT * 0.9333) && y <= ((HEIGHT * 0.9333) + (HEIGHT * ADJUSTMENT_HEIGHT)))
		{
			this->State = GAME_MENU;
		}
#endif 
	}
}

void Game::MovingCardFromPileCntrToTempCntr(std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1, float x, float y)
{
	if (Bit_temp.size() < 1 && doOnlyOnce < 1)  //guard conditions to stop multiple selection and erasion at once
	{

		//if (PileCntr.find(pileName)->second.empty())
		if(cntr.empty())
		{
			// check to see if the vector is empty
			// if so do nothing
			mouseClick = true;
		}
		else
		{
			// the actually element number - remember that the count starts from 0
			//int i = PileCntr.find(pileName)->second.size() - 1;
			int i = cntr.size() - 1;

			//size_t num = sizeof(iter1);
			//size_t count = (cntr1.size()/num);
			//std::cout << " count: " << count << std::endl;

			if (Bit_temp.size() < 1 && It_temp.size() < 1)
			{
				std::cout << "It_temp size(): " << It_temp.size() << std::endl;

				//std::cout << i << std::endl;
				//temp2.push_back(std::move(PileCntr.find(pileName)->second[i]));
				//std::cout << "SIZE: " << temp2.size() << std::endl;
				Bit_temp.push_back(std::move(cntr[i]));

				//It_temp.push_back(std::move(cntr1[count - 1]));
				//It_temp.push_back(std::move(cntr1[count]));

				std::cout << "It_temp size(): " << It_temp.size() << std::endl;

				//assigning the bit_pattern value to renderAtTheEnd
				renderAtTheEnd = Bit_temp[0]->getID();
			}

			// removes the last element of the container - pop_back()
			cntr.pop_back();

			//cntr1.pop_back();

			//count = (cntr1.size() / num);
			//std::cout << " count: " << count << std::endl;

			// PileCntr.find(pileName)->second.pop_back();
			//critical section code

			// assigning a string value to the token
			// token = pileName;

			//setting the logicGuard 
			logicGuard = 100;

			doOnlyOnce++;

			// this how intial co-ordinates are passed to the card selected
			this->AssignCoords(x, y);

			//temp2[0]->setPosition(glm::vec2(x, y));
			//Bit_temp[0]->setPosition(x, y);

			//this->Assign_CardObject_Pos(Bit_temp, 0);
			
//#if defined(_ANDROID_)
			// because android create a threading issue this workaround has to be used
//			CardInTempCntr = true;  // setting the boolean flag to true
//#else
			
			//temp2[0]->Draw(*Renderer);
			//Bit_temp[0]->Draw(*Renderer);
//#endif
		}
	}
}

void Game::AssignCoords(float x, float y)
{
	//temp2[0]->setPosition(glm::vec2(x, y));
	Bit_temp[0]->setPosition(x, y);
	std::cout << "Within AssignCoords: " << std::endl;
	std::cout << "x: " << Bit_temp[0]->getLocationX() << std::endl;
	std::cout << "y: " << Bit_temp[0]->getLocationY() << std::endl;

	this->Assign_CardObject_Pos(Bit_temp, 0);
}

void Game::AssignCoords2(float x, float y)
{
	
	//this->Update	//temp2[0]->setPosition(glm::vec2(x, y));
	Bit_temp[0]->setPosition(x, y);
	std::cout << "Within AssignCoords: " << std::endl;
	std::cout << "x: " << Bit_temp[0]->getLocationX() << std::endl;
	std::cout << "y: " << Bit_temp[0]->getLocationY() << std::endl;

	this->Update_CardObject_Pos(Bit_temp, 0);// _CardObject_Pos(Bit_temp, 0);
}

/*void Game::Update_CardObject_Pos(std::vector<CardBits*> &container, int num)
{
	if (!tempBitAssigned)
	{
		int count = 0;
		// search through the Deck vector to find a match
		for (std::vector<CardObject*>::iterator iter = Deck.begin(); iter != Deck.end(); ++iter, count++)
		{
			BitPattern temp1;
			temp1 = (*iter)->getID();
			tempBit = container[num]->getID();
			//std::cout << temp1 << "," << temp2 << std::endl;

			// if the two IDs match then send the copy of the coordinates over
			if (temp1 == tempBit)
			{
				// set the static index variable to the value staored in toe count variable
				index = count;

				std::cout << temp1 << "," << tempBit << std::endl;
				// dereferencing the pointer and the iterator
				(*iter)->Set(container[num]->GetCoords());
				std::cout << "Within Update_CardObject_Pos" << std::endl;
				std::cout << (*iter)->getX() << std::endl;
				std::cout << (*iter)->getY() << std::endl;


				std::cout << "CardObject position: ";
				(*iter)->DisplayPosition();
				std::cout << "Leaving Update_CardObject_Pos" << std::endl;
				//std::cin.get();
			}
		}

		tempBitAssigned = true;
	}
	else if (tempBitAssigned)
	{
		std::cout << "within the external iterator block" << std::endl;
		// assigned an iterator to 
		auto iter = Deck[index];
		(iter)->Set(container[num]->GetCoords());
		std::cout << "Within Update_CardObject_Pos" << std::endl;
		std::cout << (iter)->getX() << std::endl;
		std::cout << (iter)->getY() << std::endl;

		std::cout << "CardObject position: ";
		(iter)->DisplayPosition();
		std::cout << "Leaving Update_CardObject_Pos" << std::endl;
		//std::cin.get();
	}
}*/

void Game::Update_CardObject_Pos(std::vector<CardBits*> &container, int num)
{
	// assigned the variable _index_ with the value stored inside CardBits::getIndex()
	int _index_ = container[num]->getIndex();

	auto iter = Deck[_index_];
	(iter)->Set(container[num]->GetCoords());
	std::cout << "Within Update_CardObject_Pos" << std::endl;
	std::cout << (iter)->getX() << std::endl;
	std::cout << (iter)->getY() << std::endl;

	std::cout << "CardObject position: ";
	(iter)->DisplayPosition();
	std::cout << "Leaving Update_CardObject_Pos" << std::endl;
}



void Game::bruteForceAuxillary(int num, candidate pile, std::vector<CardBits*> &cntr)
{
	// create a base bit pattern for the first three nibbles 000 008
	BitPattern base_bit = { 0x000008 };

	// create a bit_mask to isolate the rank
	BitPattern bit_mask = { 0x0F0000 }; // 0000 1111 0000 0000 0000 0000

	// first assign to a temporary bitpattern holder, the bitpattern contained in the BitFoundation
	//BitPattern _temp_ = Bit_temp[0]->getID();
	BitPattern _temp_ = cntr[num - 1]->getID();

	//debugging
	//std::cout << std::hex << "temp: " << _temp_ << std::endl;

	// then mask out the lower four nibbles using the AND bitwise operator
	BitPattern suit = (_temp_ & bit_mask);    // ( _temp_ & 0x0F0000 )

	//debugging
	//std::cout << std::hex << "suit: " << suit << std::endl;

	// then create a new bit_pattern (2c 2h 2d 2s) by using the base_bit
	BitPattern duece = (suit | base_bit);

	//debugging
	//std::cout << std::hex << "duece: " << duece << std::endl;
	//std::cout << std::hex << "Bit_temp ID: " << Bit_temp[0]->getID() << std::endl;

	// then make the comparism
	if (Bit_temp[0]->getID() == duece)
	{
		// store the BitID if this vector 
		//std::cout << "Within bruteForceAuxilliary. Pile: " << pile << std::endl;
		Candidate.push_back(pile);
		//std::cout << Candidate.size() << std::endl;
	}	
}

void Game::bruteForceAuxillary2(int num, candidate &pile, std::vector<CardBits*> &cntr)
{
	// create a base bit pattern for the first three nibbles 000 008
	BitPattern base_bit = { 0x000008 };

	// create a bit_mask to isolate the suit
	BitPattern bit_mask = { 0x0F0000 }; // 0000 1111 0000 0000 0000 0000

	// get the bitpattern of the topmost card in BitFoundation
	//BitPattern _temp_ = Bit_temp[num - 1]->getID();
	BitPattern _temp_ = cntr[num - 1]->getID();

	//debugging
	//std::cout << std::hex << "temp: " << _temp_ << std::endl;

	// isolate the bit_pattern that denotes suit 0x08
	BitPattern suit = (_temp_ & bit_mask);    // ( _temp_ & 0x0F0000 )

	//debugging
	//std::cout << std::hex << "suit: " << suit << std::endl;

	// perform a leftwise bit shift on the base bit pattern ....
	BitPattern left_bitshift = { static_cast<unsigned int>(num)- 1 };

	// then shift the base_bit by the correct amount
	base_bit <<= left_bitshift;

	// then create a new bit_pattern by using the base_bit
	// this bit_pattern must be greater than 2c 2d 2h 2c
	// ... so bit_pattern (00008) <<= (i - 1)  this will shift the bits by the correct amount
	BitPattern rank_n_suit = (suit | base_bit);
	
	// ... so bit_pattern (00008) <<= (i - 1)  this will shift the bits by the correct amount
	//(rank_n_suit) <<= (rank_n_suit | left_bitshift);

	//debugging
	//std::cout << std::hex << "rank_n_suit: " << rank_n_suit << std::endl;

	// check if the bit pattern is equivalent to the bit pattern in the bit_temp ID
	if (Bit_temp[0]->getID() == rank_n_suit)
	{
		//std::cout << "Within bruteForceAuxilliary2. Pile: " << pile << std::endl;
		// if true send BitID to vector
		Candidate.push_back(pile);
	}
}

void Game::bruteForcePileAuxillary3(int num, candidate& pile, std::vector<CardBits*> &cntr)
{
	// create a bit_mask to isolate the rank
	BitPattern suit_mask = { 0x00FFFF };   // 0000 0000 1111 1111 1111 1111

	//...get the rank of the card that is being moved...
	BitPattern _temp_ = Bit_temp[0]->getID(); // e.g. 0000 0100 0000 0000 0100 0000

	// create a bit pattern that only shows the rank - use an AND bitwise operator
	BitPattern rank = (_temp_ & suit_mask);  // for example 0000 0000 0000 0000 0100 0000

    //debugging
	//std::cout << std::hex << "rank: " << rank << std::endl;

	// use a bitshift to create a rank that is one higher than the bit_pattern in Bit_temp
	BitPattern higher_rank = rank;
	BitPattern left_bitshift = { 1 };

	//debugging
	//std::cout << std::hex << "higher_rank: " << higher_rank << std::endl;

	// performing the leftwise bitshift
	(higher_rank) <<= (left_bitshift);  // for example 0000 0000 0000 0000 1000 0000

	//debugging
	//std::cout << std::hex << "higher_rank: " << higher_rank << std::endl;

	//get the rank of the topmost card from the BitPile
	BitPattern _BitPileTemp_ = cntr[num - 1]->getID();

	// create a bit pattern that only shows the rank - use an AND bitwise operator
	BitPattern _BitPileRank_ = (_BitPileTemp_ & suit_mask);

	//debugging
	//std::cout << std::hex << "_bitPile_rank: " << _BitPileRank_ << std::endl;

	// use a bitshift to create a rank that is one higher than the bit_pattern in cntr[num - 1]->getID();
	//BitPattern _HigherBitPileRank_ = _BitPileRank_;

	//debugging
	//std::cout << std::hex << "higher_bitPile_rank: " << _HigherBitPileRank_ << std::endl;

	// performing the leftwise bitshift
	//(_HigherBitPileRank_) <<= (left_bitshift);

	//debugging
	//std::cout << std::hex << "higher_bitPile_rank: " << _HigherBitPileRank_ << std::endl;


	// check to see if the card is the same rank or is one rank lower
	//if (cntr[num - 1]->getID() == rank || cntr[num - 1]->getID() == higher_rank)
	if (_BitPileRank_ == rank || _BitPileRank_ == higher_rank)
	{
		//std::cout << "Within bruteForceAuxilliary3. Pile: " << pile << std::endl;
		// send the bitID into a vector
		Candidate.push_back(pile);
	}
}

void Game::bruteForceKings(candidate& pile)
{
	// create a bit_mask to isolate the rank
	BitPattern suit_mask = { 0x00FFFF };   // 0000 0000 1111 1111 1111 1111

	// a bit_pattern for the rank of king
	BitPattern king = { 0x004000 }; // 0000 0000 0100 0000 0000 0000 0000

	//...get the rank of the card that is being moved...
	BitPattern _temp_ = Bit_temp[0]->getID(); // e.g. 0000 0100 0000 0000 0100 0000

	// create a bit pattern that only shows the rank - use an AND bitwise operator
	BitPattern rank = (_temp_ & suit_mask);  // for example 0000 0000 0000 0000 0100 0000

	if (rank == king)
	{
		//std::cout << "Within bruteForceKings. Pile: " << pile << std::endl;
		Candidate.push_back(pile);
	}
}

void Game::bruteForceSearch()
{
	//create a bit pattern for each iterPile, this bit_pattern will be used to return a card back to its original vector
	if (this->State == GAME_ACTIVE && logicGuard == 100 && doOnlyOnce == 1)
	{
		if (!Bit_temp.empty())
		{		
			if (!BitFoundation1.empty())
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitFoundation1.size();
										
				if (i == 1)
				{								
					this->bruteForceAuxillary(i, _BitFoundation1_, BitFoundation1);
				}
				else if (i > 1)
				{
					this->bruteForceAuxillary2(i, _BitFoundation1_, BitFoundation1);
				}
			}

		    if (!BitFoundation2.empty())
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitFoundation2.size();

				if (i == 1)
				{
					this->bruteForceAuxillary(i, _BitFoundation2_, BitFoundation2);
				}
				else if (i > 1)
				{
					this->bruteForceAuxillary2(i, _BitFoundation2_, BitFoundation2);
				}
			}

			if (!BitFoundation3.empty())
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitFoundation3.size();

				if (i == 1)
				{
					this->bruteForceAuxillary(i, _BitFoundation3_, BitFoundation3);
				}
				else if (i > 1)
				{
					this->bruteForceAuxillary2(i, _BitFoundation3_, BitFoundation3);
				}
			}

			if (!BitFoundation4.empty())
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitFoundation4.size();

				if (i == 1)
				{ 
					this->bruteForceAuxillary(i, _BitFoundation4_, BitFoundation4);
				}
				else if (i > 1)
				{
					this->bruteForceAuxillary2(i, _BitFoundation4_, BitFoundation4);
				}
			}

			//check to see if each BitPile is empty
			if (!BitPileL1.empty() && BitHolder != _BitPileL1_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileL1.size();

				this->bruteForcePileAuxillary3(i, _BitPileL1_, BitPileL1);
			}
			else if (BitPileL1.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileL1_);
			}

			if (!BitPileL2.empty() && BitHolder != _BitPileL2_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileL2.size();

				this->bruteForcePileAuxillary3(i, _BitPileL2_, BitPileL2);
			}
			else if (BitPileL2.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileL2_);
			}

			if (!BitPileL3.empty() && BitHolder != _BitPileL3_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileL3.size();

				this->bruteForcePileAuxillary3(i, _BitPileL3_, BitPileL3);
			}
			else if (BitPileL3.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileL3_);
			}

			if (!BitPileL4.empty() && BitHolder != _BitPileL4_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileL4.size();

				this->bruteForcePileAuxillary3(i, _BitPileL4_, BitPileL4);
			}
			else if (BitPileL4.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileL4_);
			}

			if (!BitPileR1.empty() && BitHolder != _BitPileR1_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileR1.size();

				this->bruteForcePileAuxillary3(i, _BitPileR1_, BitPileR1);
			}
			else if (BitPileR1.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileR1_);
			}

			if (!BitPileR2.empty() && BitHolder != _BitPileR2_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileR2.size();

				this->bruteForcePileAuxillary3(i, _BitPileR2_, BitPileR2);
			}
			else if (BitPileR2.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileR2_);
			}

			if (!BitPileR3.empty() && BitHolder != _BitPileR3_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileR3.size();

				this->bruteForcePileAuxillary3(i, _BitPileR3_, BitPileR3);
			}
			else if (BitPileR3.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileR3_);
			}

			if (!BitPileR4.empty() && BitHolder != _BitPileR4_)
			{
				// ...find the size of the vector<CardBits*>...
				int i = BitPileR4.size();

				this->bruteForcePileAuxillary3(i, _BitPileR4_, BitPileR4);
			}
			else if (BitPileR4.empty())
			{
				// if empty only a King can be inserted into this vector
				this->bruteForceKings(_BitPileR4_);
			}
			
			// if after all the above checks the Candidate vetor is still empty then populate it with the value of the 
			// orignal vector
			if(Candidate.empty())
			{
				// if none of the iterPiles are suitable then the cardbit is returned back to its original vector
				Candidate.push_back(BitHolder);
			}
			// calculating the score
			// this->Score += temp2[0]->getRank();
			//std::cout << "Within bruteForceSearch: " << std::endl;
			//std::cout << "Size: " << Candidate.size()<< std::endl;
			//int i = 0;
			//for (auto iter = Candidate.begin(); iter != Candidate.end(); ++iter, i++)
			//	std::cout << "Candidate pile["<< i <<"]: " << *iter << std::endl;
			
			//std::cin.get();
		}
	}
}



void Game::RecursiveRenderer()
{
	if (this->State == GAME_SMOOTHMOVER)
	{
		if (logicGuard == 100 && !Bit_temp.empty())
		{
			std::cout << "LogicGuard: " << logicGuard << std::endl;

			// boolean function to determine which distance to calculate
			//bool calculateD2 = false;

			// create a variable to hold the name bit_pattern of the Candidate bitpile
			static candidate pileReceive = 0;
			static candidate pileReceive2 = 0;
			// create two variables to hold the x and y coordinates of the CardBit 
			float posX = 0, posY = 0;

#if defined(_ANDROID_)
			posX = touchLocationPtr->x;
			posY = touchLocationPtr->y;
#endif
			// here we create two variables to hold the x and y coordinates of the CardBit
			glm::vec2 location(posX, posY);

			static float X_dist = 0.0f;
			static float Y_dist = 0.0f;

			static float distance = -1.0f;
			static float distance2 = -1.0f;

			// debugging
			std::cout << "Within RecursiveRenderer" << std::endl;
			//std::cout << "moveTexture.x: " << moveTexture.first << std::endl;
			//std::cout << "moveTexture.y: " << moveTexture.second << std::endl;
			//std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
			//std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;
			//std::cout << "X_dist: " << X_dist << std::endl;
			//std::cout << "Y_dist: " << Y_dist << std::endl;

			// while loop goes here
			while (!mouseClick)  // while mouse click is not false
			{
				// find the size of Candidate vector
				if (Candidate.size() > 1)
				{
					//std::cout << "Candidate size: " << Candidate.size() << std::endl;
					//first check to see if any CardBits can be sent to a BitFoundation
					for (int i = 0; i != (Candidate.size()); i++)
					{
						pileReceive = Candidate[i];

						if (pileReceive != _BitFoundation1_ || pileReceive != _BitFoundation2_ || pileReceive != _BitFoundation3_ || pileReceive != _BitFoundation4_)
						{
							std::cout << "RecursiveRenderer pile: " << pileReceive << std::endl;
							std::cout << "going to brexit" << std::endl;
							goto brexit;
						}
					}


					// iterate through the Candidate container and calculate the distances
					// the last element is purposely ignored
					for (int i = 0; i != (Candidate.size() - 1); i++)
					{
						pileReceive = Candidate[i];
						std::cout << "RecursiveRenderer pile: " << pileReceive << std::endl;
						std::cout << "not going to brexit" << std::endl;
						// check each BitPile to see if it candidate pattern is identical to that stored in 
						// the Candidate vector 
						this->RecursiveRendererAuxillary(i, _BitPileL1_, BitPileL1, X_dist, Y_dist, distance, distance2, false, LEFT_HAND_GAP, Y1, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileL2_, BitPileL2, X_dist, Y_dist, distance, distance2, false, LEFT_HAND_GAP, Y2, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileL3_, BitPileL3, X_dist, Y_dist, distance, distance2, false, LEFT_HAND_GAP, Y3, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileL4_, BitPileL4, X_dist, Y_dist, distance, distance2, false, LEFT_HAND_GAP, Y4, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileR1_, BitPileR1, X_dist, Y_dist, distance, distance2, false, RIGHT_HAND_GAP, Y1, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileR2_, BitPileR2, X_dist, Y_dist, distance, distance2, false, RIGHT_HAND_GAP, Y2, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileR3_, BitPileR3, X_dist, Y_dist, distance, distance2, false, RIGHT_HAND_GAP, Y3, posX, posY, location, pileReceive, pileReceive2);
						this->RecursiveRendererAuxillary(i, _BitPileR4_, BitPileR4, X_dist, Y_dist, distance, distance2, false, RIGHT_HAND_GAP, Y4, posX, posY, location, pileReceive, pileReceive2);


						// iterate through the remaining 
						for (int j = i + 1; j != Candidate.size(); j++)
						{
							pileReceive2 = Candidate[j];
							// check each BitPile to see if it candidate pattern is identical to that stored in 
							// the Candidate vector 
							this->RecursiveRendererAuxillary(j, _BitPileL1_, BitPileL1, X_dist, Y_dist, distance, distance2, true, LEFT_HAND_GAP, Y1, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileL2_, BitPileL2, X_dist, Y_dist, distance, distance2, true, LEFT_HAND_GAP, Y2, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileL3_, BitPileL3, X_dist, Y_dist, distance, distance2, true, LEFT_HAND_GAP, Y3, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileL4_, BitPileL4, X_dist, Y_dist, distance, distance2, true, LEFT_HAND_GAP, Y4, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileR1_, BitPileR1, X_dist, Y_dist, distance, distance2, true, RIGHT_HAND_GAP, Y1, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileR2_, BitPileR2, X_dist, Y_dist, distance, distance2, true, RIGHT_HAND_GAP, Y2, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileR3_, BitPileR3, X_dist, Y_dist, distance, distance2, true, RIGHT_HAND_GAP, Y3, posX, posY, location, pileReceive, pileReceive2);
							this->RecursiveRendererAuxillary(j, _BitPileR4_, BitPileR4, X_dist, Y_dist, distance, distance2, true, RIGHT_HAND_GAP, Y4, posX, posY, location, pileReceive, pileReceive2);

							// a comparisom test should be done on the distances here
							// perhaps a new value is stored for name and distance and only changed when a shorter distance is found 
							// comparison test is done here
							if (distance2  < distance && distance2 > -1)
							{
								distance = distance2;
								//name = it->first;  // assigning the name variable the string value of it->first
								pileReceive = pileReceive2;
								//std::cout << "DISTANCE: " << distance << std::endl;
								//std::cout << "NAMERECEIVE: " << nameReceive << std::endl;
							}
						}
					}
				}
				else
				{
					if (Candidate.empty())
					{
						std::cout << "ERROR: Candidate container is empty! " << std::endl;						
					}
					else
					{
						pileReceive = Candidate[0];      // the contents of Candidate[0] is assigned to pileRecieve
						//std::cout << "CandidatePile: " << pileReceive << std::endl;
					}
				}
			brexit:
				this->RecursiveRendererAuxillary3(pileReceive, _BitFoundation1_, BitFoundation1, X2, Y1, this->Xdest, this->Ydest, 0.0f, 0.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitFoundation2_, BitFoundation2, X2, Y2, this->Xdest, this->Ydest, 0.0f, 0.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitFoundation3_, BitFoundation3, X2, Y3, this->Xdest, this->Ydest, 0.0f, 0.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitFoundation4_, BitFoundation4, X2, Y4, this->Xdest, this->Ydest, 0.0f, 0.0f);

				this->RecursiveRendererAuxillary3(pileReceive, _BitPileL1_, BitPileL1, X1, Y1, this->Xdest, this->Ydest, LEFT_OFFSET, 1.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileL2_, BitPileL2, X1, Y2, this->Xdest, this->Ydest, LEFT_OFFSET, 1.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileL3_, BitPileL3, X1, Y3, this->Xdest, this->Ydest, LEFT_OFFSET, 1.0f);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileL4_, BitPileL4, X1, Y4, this->Xdest, this->Ydest, LEFT_OFFSET, 1.0f);

				this->RecursiveRendererAuxillary3(pileReceive, _BitPileR1_, BitPileR1, X3, Y1, this->Xdest, this->Ydest, 1.0f, RIGHT_OFFSET);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileR2_, BitPileR2, X3, Y2, this->Xdest, this->Ydest, 1.0f, RIGHT_OFFSET);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileR3_, BitPileR3, X3, Y3, this->Xdest, this->Ydest, 1.0f, RIGHT_OFFSET);
				this->RecursiveRendererAuxillary3(pileReceive, _BitPileR4_, BitPileR4, X3, Y4, this->Xdest, this->Ydest, 1.0f, RIGHT_OFFSET);

				//reseting the boolean mouseClick
				mouseClick = true;

				// reseting the static variables
			    pileReceive = 0, pileReceive2 = 0;

				X_dist = 0.0f;
			    Y_dist = 0.0f;

				distance = -1.0f;
				distance2 = -1.0f;
			}
		}

		//debugging
		std::cout << "Within RecursiveRenderer change state to GAME_UPDATE" << std::endl;
		//std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
		//std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;
		// std::cin.get();
		this->State = GAME_UPDATE;
	}
}

void Game::RecursiveRendererAuxillary(int num, candidate& pile, std::vector<CardBits*> &cntr, float Xdist, float Ydist,
	                                  float distance, float distance2, bool calculateD2, float pos_x, float pos_y, float Xpos,
	                                  float Ypos, glm::vec2 location, candidate pileReceive, candidate pileReceive2)
{
	if (Candidate[num] == pile)
	{
		if (cntr.empty())
		{
			// check to see if this cntr is empty
			// important when moving the Kings
			//posX = pos_x;
			//posY = pos_y;
			Xpos = pos_x;
			Ypos = pos_y;
		}
		else
		{
			// if not the get the position of the top most CardBit
			location.x = (cntr[cntr.size() - 1]->getLocationX());
			location.y = (cntr[cntr.size() - 1]->getLocationY());
			//std::cout << "Location x: " << location.x << std::endl;
			//std::cout << "Location y: " << location.y << std::endl;
		}

		this->RecursiveRendererAuxillary2(Xdist, Ydist, location, distance, distance2, calculateD2, pileReceive, pileReceive2);
	}
}

void Game::RecursiveRendererAuxillary2(float Xdist, float Ydist, glm::vec2 location, float distance, float distance2,
	                                   bool calculateD2, candidate pileReceive, candidate pileReceive2)
{
	 Xdist = (moveTexture.first - location.x);
	 Ydist = (moveTexture.second - location.y);

	 // use phythagoras' theorem to calculate the distance
	 if(calculateD2 == false)
		 distance = std::sqrt(std::pow(Xdist, 2.0) + std::pow(Ydist, 2.0));
	 else
		 distance2 = std::sqrt(std::pow(Xdist, 2.0) + std::pow(Ydist, 2.0));


	 //debugging
	 //std::cout << "X_dist: " << Xdist << std::endl;
	 //std::cout << "Y_dist: " << Ydist << std::endl;
	 //std::cout << "moveTexture.first: " << moveTexture.first << std::endl;
	 //std::cout << "moveTexture.second: " << moveTexture.second << std::endl;
	 //std::cout << "Location x: " << std::setprecision(3) << location.x << std::endl;
	 //std::cout << "Location y: " << std::setprecision(3) << location.y << std::endl;

	 if (calculateD2 == false)
	 {
		 std::cout << "Distance: " << distance << std::endl;
		 std::cout << "Candidate: " << pileReceive << std::endl;
	 }
	 else
	 {
		 std::cout << "Distance2: " << distance2 << std::endl;
		 std::cout << "Candidate: " << pileReceive << std::endl;
	 }
	// std::cin.get();
}


void Game::RecursiveRendererAuxillary3(candidate pileReceive, candidate &pile, std::vector<CardBits*> &cntr, float X, float Y, float Xdest, float Ydest, float left_offset, float right_offset) 
{
	// insert the Bit_temp object into the selected pile
	if (pileReceive == pile)
	{
		int i = cntr.size();

		//setting the values of the destination coordinates
		float offset = (float)i * left_offset * right_offset;
		this->Xdest = X + offset;
		this->Ydest = Y;

		Bit_temp[0]->getLocationX();
		Bit_temp[0]->getLocationY();

		BitofPile = pileReceive;
		std::cout << "Within RecurisveRendererAuxillairy3 BitofPile: " << BitofPile << std::endl;
	}
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_UPDATE)
	{
		int destinationX = this->Xdest;
		int destinationY = this->Ydest;
		//std::cout << "destinationX: " << destinationX << std::endl;
		//std::cout << "destinationY: " << destinationY << std::endl;
		//std::cout << "deltaTime: " << dt << std::endl;
		//debugging
		//std::cout << "Within Update" << std::endl;
		//std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
		//std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;

		//if (this->getClickA() == false && logicGuard == 100 && !temp2.empty())
		if (logicGuard == 100 && !Bit_temp.empty())
		{
			//debugging
			//std::cout << "Within Update" << std::endl;
			//std::cout << "Position.x: " << Bit_temp[0]->getLocationX() << std::endl;
			//std::cout << "Position.y: " << Bit_temp[0]->getLocationY() << std::endl;
			//std::cin.get();
			if (this->State == GAME_UPDATE)
			{
				// Update the card position as it traces across the screen
				glm::vec2 pos =  Bit_temp[0]->Move(dt, destinationX, destinationY);
				std::cout << "Within Update: " << std::endl;
				std::cout << "pos.x: " << pos.x << std::endl;
				std::cout << "pos.y: " << pos.y << std::endl;

				//temp2[0]->setPosition(glm::vec2(x, y));
				//Bit_temp[0]->setPosition(glm::vec2(pos.x, pos.y));

				//this->Assign_CardObject_Pos(Bit_temp, 0);

				this->AssignCoords2(pos.x, pos.y);

//#if defined(_ANDROID_)
				//set the flag to true
//				UpdatingCardPosition = true;
//#else

			    //temp2[0]->Draw(*Renderer);
				//this->RenderCardBit();
//#endif

			}
			if (Bit_temp[0]->getLocationX() == destinationX && Bit_temp[0]->getLocationY() == destinationY)
			{
				//std::cout << "destinationX: " << destinationX << "X: " << Bit_temp[0]->getLocationX() << std::endl;
				//std::cout << "destinationY: " << destinationY << "Y: " << Bit_temp[0]->getLocationY() << std::endl;
				//std::cout << "POSITION OBTAINED" << std::endl;
				//std::cin.get();

//#if defined(_ANDROID_)
				//resetting the flag back to false
//				UpdatingCardPosition = false;
//#endif

				this->State = GAME_PLACEINNEWPILE;
			}
		}
	}
}


// reset renderAtTheEnd in this function


// calculating the score of the CardBit by isolating the rank
void Game::score()
{
	if (!Bit_temp.empty())
	{
		// create a bit_mask to isolate the rank
		BitPattern suit_mask = { 0x00FFFF };   // 0000 0000 1111 1111 1111 1111

		// create a temporary BitPattern variable
		BitPattern temp;

		// assign the value of Bit_temp to temp
		temp = Bit_temp[0]->getID();

		// isolate the rank
		// create a bit pattern that only shows the rank - use an AND bitwise operator
		BitPattern rank = (temp & suit_mask);  // for example 0000 0000 0000 0000 0100 0000

		this->tempScore += rank;

		// updating the score with the rank of the cardbits
		for (int i = 0; i != 17; i++)
		{
			if ((double)tempScore / std::pow(2, i) == 1.0)
			{
				this->Score += i - 1;
			}
		}
		
		//reset tempScore
		tempScore = 0;

	}
	else
	{
		std::cout << "Error!" << std::endl;
	}
}


void Game::moveToPileAuxillairy(candidate pile, std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1)
{
	// use the BitofPile to retrieve the CardBit vector to insert into
	if (BitofPile == pile)
	{
		if (pile == _BitFoundation1_ || pile == _BitFoundation2_ || pile == _BitFoundation3_ || pile == _BitFoundation4_)
		{
			// for the foundation piles
			// updating the moves
			this->Moves += 1;

			//calling the score function
		    this->score();

			std::cout << "BitofPile is equal to Pile: " << pile << std::endl;
			this->mustExecute(cntr, cntr1);
		}
		else
		{
			// for all the other piles
			// updating the moves
			this->Moves += 1;
			//calculating the score 
			this->Score += 1;
			//this->State = GAME_ACTIVE;
			std::cout << "BitofPile is equal to Pile: " << pile << std::endl;
			this->mustExecute(cntr, cntr1);
		}
	}
}

void Game::moveToPile()
{
	if (logicGuard == 100 && !Bit_temp.empty())
	{
		if (this->State == GAME_PLACEINNEWPILE)
		{
			// resetting mouseClick
			mouseClick = false;
			while (!mouseClick)  // while mouse click is not false
			{
				//inseting the CardBit into the correct correct vector
				this->moveToPileAuxillairy(_BitFoundation1_, BitFoundation1, iterFoundation1);
				this->moveToPileAuxillairy(_BitFoundation2_, BitFoundation2, iterFoundation2);
				this->moveToPileAuxillairy(_BitFoundation3_, BitFoundation3, iterFoundation3);
				this->moveToPileAuxillairy(_BitFoundation4_, BitFoundation4, iterFoundation4);

				this->moveToPileAuxillairy(_BitPileL1_, BitPileL1, iterPileL1);
				this->moveToPileAuxillairy(_BitPileL2_, BitPileL2, iterPileL2);
				this->moveToPileAuxillairy(_BitPileL3_, BitPileL3, iterPileL3);
				this->moveToPileAuxillairy(_BitPileL4_, BitPileL4, iterPileL4);

				this->moveToPileAuxillairy(_BitPileR1_, BitPileR1, iterPileR1);
				this->moveToPileAuxillairy(_BitPileR2_, BitPileR2, iterPileR2);
				this->moveToPileAuxillairy(_BitPileR3_, BitPileR3, iterPileR3);
				this->moveToPileAuxillairy(_BitPileR4_, BitPileR4, iterPileR4);

				// all the boolean and static variables are reset here
				// reset the renderAtTheEnd BitPattern
				renderAtTheEnd = { 0x000000 };

				// reset static candidate BitHolder
				BitHolder = 0;

				// reset static candidate BitOfPile
				BitofPile = 0;

				// clearing the Candidate vector
				Candidate.clear();

				//reseting the boolean mouseClick
				mouseClick = true;

				//reseting the logicGuard
				logicGuard = 0;

				//resetting the tempBitAssign boolean flag
				tempBitAssigned = false;

				// resetting tempBit
				tempBit = { 0x000000 };


				std::cout << "Moves: " << this->Moves << std::endl;
				std::cout << "Score: " << this->Score << std::endl;
			}
		}
	}
	//this->Init_Iter_Vec();

	//this->Sort_Iter_Vec();

	// returning back to a previous state
	this->State = GAME_ACTIVE;
}

void Game::mustExecute(std::vector<CardBits*> &cntr, std::vector<CardObject*> &cntr1)
{
	bool mustExecute = false;
	while (!mustExecute)
	{
		//(itC->second).push_back(std::move(temp2[0]));
		cntr.push_back(std::move(Bit_temp[0]));

		Bit_temp.clear();        //clearing the Bit_temp vector



		// moving an iterator from one container to another
		//this->movingTheIterators();
		//cntr1.push_back(std::move(It_temp[0]));

		//It_temp.clear();        // clearing the It_temp vector

		mustExecute = true;
		//std::cin.get();
	}
}




/*void Game::setClickA(const bool **selectA)
{
	ClickA = **selectA;
}*/

/*bool Game::getClickA(void)
{
	return ClickA;
}*/


// stopwatch functions
void Game::clockStatus()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_SMOOTHMOVER || this->State == GAME_UPDATE)
	{
		if (this->ClockState == OFF)
		{
			std::cout << "The timer is off " << std::endl;
			Timer::instance()->display();
			//std::cin.get();
		}
		else if (this->ClockState == ON)
		{
			std::cout << " The timer is on " << std::endl;

			// creating a execute-once-then-lock-routine
			std::cout << "CLOCK STATUS: " << CLOCK << std::endl;
			while (!CLOCK)
			{
				Timer::instance()->reset();
				CLOCK = true;
			}
		}
	}
}

void Game::ActivatedClock()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_SMOOTHMOVER || this->State == GAME_UPDATE)
	{
		if (this->ClockState == ON)
		{
			Timer::instance()->getTimefromSeconds(Timer::instance()->getElapsed());
			//Timer::instance()->display();
			//std::cin.get();
		}
	}
}

void Game::setClockToFalse()
{
	CLOCK = false;
}

void Game::TurnClockOn()
{
	if (this->ClockState == OFF)
	{
		this->ClockState = ON;
	}
}

// Menu Functions
void Game::MenuSelection(float x, float y)
{
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= 100 && y < 200)
	{
		this->State = GAME_INSTRUCTIONS;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= 200 && y < 300)
	{
		this->State = GAME_SCORESYSTEMS;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= 300 && y < 400)
	{
		this->State = GAME_ACTIVE;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= 400 && y < 500)
	{
		this->State = GAME_RESTART;
	}	
}

void Game::MenuSelectionTouch(float x, float y)
{
#if defined(_ANDROID_)
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= (HEIGHT * 0.1667) && y < (HEIGHT * 0.3333))
	{
		this->State = GAME_INSTRUCTIONS;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= (HEIGHT * 0.3333) && y < (HEIGHT * 0.5))
	{
		this->State = GAME_SCORESYSTEMS;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= (HEIGHT * 0.5) && y < (HEIGHT * 0.6667))
	{
		this->State = GAME_ACTIVE;
	}
	if (x >= (WIDTH - WIDTH) && x <= (WIDTH) && y >= (HEIGHT * 0.6667) && y < (HEIGHT * 0.8333))
	{
		this->State = GAME_RESTART;
	}
#endif
}

void Game::InstructionExit(int x, int y)
{
	//if (x >= (650) && x <= (800) && y >= 570 && y <= 600)
	if(x >= (WIDTH * 0.8125) && x <= (WIDTH) && y >= (HEIGHT * 0.95) && y <= (HEIGHT))
	{
		this->State = GAME_MENU;
	}
}

void Game::ScoringSystemExit(int x, int y)
{
	//if (x >= (650) && x <= (800) && y >= 570 && y <= 600)
	if (x >= (WIDTH * 0.8125) && x <= (WIDTH) && y >= (HEIGHT * 0.95) && y <= (HEIGHT))
	{
		this->State = GAME_MENU;
	}
}




// defining the overloaded bitwise or operator
// overloading the bitwise or operator
// Type operator|(const Type &lhs, const Type &rhs); // Bitwise or
BitPattern operator|(const BitPattern &lhs, const BitPattern &rhs)
{
	BitPattern result = { lhs.bits | rhs.bits };
	return result;
}

// overloading the right shift assignment operator
// Assign right shift
BitPattern &operator>>=(BitPattern &lhs, const BitPattern &rhs)
{
	BitPattern shift = { lhs.bits >>= rhs.bits };
	return shift;
}

// overloading the left shift assignment operator
// Assign the left shift
BitPattern &operator<<=(BitPattern &lhs, const BitPattern &rhs)
{
	BitPattern shift = { lhs.bits <<= rhs.bits };
	return shift;
}

// overloading the extraction operator
// left shift
std::ostream &operator<<(std::ostream &os, const BitPattern &out)
{
	return (os << out.bits);
}

//overloading the bitwise AND operator
BitPattern operator&(const BitPattern &lhs, const BitPattern &rhs)
{
	BitPattern AND = { lhs.bits & rhs.bits };
	return AND;
}

// overloading the equivalence operator
bool operator==(const BitPattern &lhs, const BitPattern &rhs)
{
	if (lhs.bits == rhs.bits)
		return true;
	else
		return false;
}

// overloading the != operator
bool operator!=(const BitPattern &lhs, const BitPattern &rhs)
{
	if (lhs.bits != rhs.bits)
		return true;
	else
		return false;
}

// overloading the incremetal addition operator
GLuint &operator+=(GLuint &lhs, const BitPattern &rhs)
{
	lhs += rhs.bits;
	return lhs;
}


