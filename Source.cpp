//main.cpp
#if !defined(_ANDROID_)
#define GLEW_STATIC
#include <GL/glew.h>
#else
//#include <android/native_activity.h>
#include <jni.h>
#include <android/log.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/configuration.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "firebase/admob.h"
#include "firebase/admob/banner_view.h"
#include "firebase/admob/interstitial_ad.h"
#include "firebase/admob/native_express_ad_view.h"
#include "firebase/admob/rewarded_video.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#endif

#include <SDL2\SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <cstdlib>
//#include <conio.h>
#include <cmath>
#include <cassert>                // assert


//other includes
#include "Game.h"
#include "ResourceManager.h"
#include "StopWatch.h" 

/*
// A simple listener that logs changes to a BannerView.
class LoggingBannerViewListener : public firebase::admob::BannerView::Listener {
 public:
 //ctor
  LoggingBannerViewListener() {}
  
  // public member function - this overrides a virtual function from the base class
  // called OnPresentationStateChanged
  void OnPresentationStateChanged(firebase::admob::BannerView* banner_view, firebase::admob::BannerView::PresentationState state) override {
		printf ("BannerView PresentationState has changed to %d.", state);
  }
  
  // public member function - this overrides a virtual function from the base class
  // called OnBoundingBoxChnaged 
  void OnBoundingBoxChanged(firebase::admob::BannerView* banner_view, firebase::admob::BoundingBox box) override {
		printf ("BannerView BoundingBox has changed to (x: %d, y: %d, width: %d, height %d).", box.x, box.y, box.width, box.height);
  }
};


// A simple listener that logs changes to an InterstitialAd.
class LoggingInterstitialAdListener  : public firebase::admob::InterstitialAd::Listener {
 public:
  //ctor
  LoggingInterstitialAdListener() {}
  
  // public member function
  void OnPresentationStateChanged(firebase::admob::InterstitialAd* interstitial_ad, firebase::admob::InterstitialAd::PresentationState state) override {
		printf ("InterstitialAd PresentationState has changed to %d.", state);
  }
};


// A simple listener that logs changes to a NativeExpressAdView.
class LoggingNativeExpressAdViewListener  : public firebase::admob::NativeExpressAdView::Listener {
 public:
  //ctor
  LoggingNativeExpressAdViewListener() {}
  
  // public member function
  void OnPresentationStateChanged(firebase::admob::NativeExpressAdView* native_express_ad_view, firebase::admob::NativeExpressAdView::PresentationState state) override {
		printf ("NativeExpressAdView PresentationState has changed to %d.", state);				 
  }
  
  void OnBoundingBoxChanged(
      firebase::admob::NativeExpressAdView* native_express_ad_view,
      firebase::admob::BoundingBox box) override {
		printf ("NativeExpressAdView BoundingBox has changed to (x: %d, y: %d, width: %d, height %d).",box.x, box.y, box.width, box.height);
  }
};

// A simple listener that logs changes to rewarded video state.
class LoggingRewardedVideoListener : public firebase::admob::rewarded_video::Listener {
 public:
  //ctor
  LoggingRewardedVideoListener() {}
  
  // public member function
  void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
		printf ("Rewarding user with %f %s.", reward.amount, reward.reward_type.c_str());				 
       }
  
  
  void OnPresentationStateChanged(
      firebase::admob::rewarded_video::PresentationState state) override {
	       printf("Rewarded video PresentationState has changed to %d.", state);
        }
};

// The AdMob app IDs for the test app.
#if defined(__ANDROID__)
const char* kAdMobAppID = "ca-app-pub-3940256099942544~3347511713";
#else
const char* kAdMobAppID = "ca-app-pub-3940256099942544~1458002511";
#endif

// These ad units IDs have been created specifically for testing, and will
// always return test ads.
#if defined(__ANDROID__)
const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
const char* kNativeExpressAdUnit = "ca-app-pub-3940256099942544/1072772517";
const char* kRewardedVideoAdUnit = "ca-app-pub-3940256099942544/2888167318";
#else
const char* kBannerAdUnit = "ca-app-pub-3940256099942544/2934735716";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/4411468910";
const char* kNativeExpressAdUnit = "ca-app-pub-3940256099942544/2562852117";
const char* kRewardedVideoAdUnit = "ca-app-pub-3940256099942544/6386090517";
#endif

// Standard mobile banner size is 320x50.
static const int kBannerWidth = 320;
static const int kBannerHeight = 50;

// The native express ad's width and height.
static const int kNativeExpressAdWidth = 320;
static const int kNativeExpressAdHeight = 220;

// Sample keywords to use in making the request.
static const char* kKeywords[] = {"AdMob", "C++", "Fun"};

// Sample test device IDs to use in making the request.
static const char* kTestDeviceIDs[] = {"2077ef9a63d2b398840261c8221a0c9b",
                                       "098fe087d987c9a878965454a65654d7"};

// Sample birthday value to use in making the request.
static const int kBirthdayDay = 10;
static const int kBirthdayMonth = 11;
static const int kBirthdayYear = 1976;




#if defined(__ANDROID__)

//create the Firebase app
firebase::App* app;


JNIEXPORT void JNICALL Java_com_CastleExperimental_ik_CastleExperimental_initFirebase(JNIEnv* env, jobject thiz)
{
	
    // Create the Firebase app.
	// firebase::App* app = firebase::App::Create(firebase::AppOptions(), env, thiz);
	app = firebase::App::Create(firebase::AppOptions(), env, thiz);
	
	firebase::admob::Initialize(*app, kAdMobAppID);
	
	//printf("Created the Firebase App %x.", static_cast<int>(reinterpret_cast<intptr_t>(app)));
}

// Android ad unit IDs
// const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111";
// const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";


//::app;

#endif  // defined(__ANDROID__)
*/

//The Width of the screen
#if defined(_ANDROID_)
GLuint WIDTH = 1280;
GLuint HEIGHT = 720;
// Screen Dimensions
//SDL_Rect gScreenRectangle = { 0, 0, static_cast<int>(WIDTH), static_cast<int>(HEIGHT) };

//Touch Variables
SDL_Point touchLocation = { static_cast<int>(WIDTH) / 2, static_cast<int>(HEIGHT) / 2 };
//SDL_Point touchLocation = { gScreenRectangle.w / 2, gScreenRectangle.h / 2 };

// assigning the address to the pointer
SDL_Point *touchLocationPtr = &touchLocation;
//SDL_Point *touchLocationPtr.x = &touchLocation.x;
//SDL_Point *touchLocationPtr.y = &touchLocation.y;
#else
GLuint WIDTH = 800;
GLuint HEIGHT = 600;
#endif

//defining the global random function
int random(int count) {
	return static_cast<int>((count*static_cast<double>(std::rand())) / (RAND_MAX + 1.0));
} 

#define MS_PER_UPDATE 0.016

// forward declaration
unsigned getCurrentSysTime();

extern auto currentTime();

//creating an instance of the game object
std::string gameName("Castle - A Card Game by Nuerofuzz");

//creating an instance of the game object
Game Castle(WIDTH, HEIGHT);



SDL_Window * window;
SDL_GLContext context;

//boolean flags that control the flow of the game
bool shouldQuit = false;     // to quit the game loop
bool startDrag = false;      // flag to allow the drag of a card

// filtering events prototype
int FilteringEvents(void* unused, SDL_Event* ev);

SDL_Event event1;
SDL_Event eventCallback(SDL_Event *e)
{
	event1 = *e;
	return event1;
}

#if defined(_ANDROID_)
int SDL_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
/*	
	printf("Created the Firebase App %x.", static_cast<int>(reinterpret_cast<intptr_t>(app)));
	
	printf("Initializing the AdMob with Firebase API.");
	
#ifdef _ANDROID_
		
	firebase::admob::AdRequest request;
	
	// Create an ad size for the BannerView.
	firebase::admob::AdSize banner_ad_size;
	banner_ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
	banner_ad_size.width = kBannerWidth;
	banner_ad_size.height = kBannerHeight;

	
//firebase::admob::BannerView* banner = new firebase::admob::BannerView();

#endif//
*/	
	
	
	std::srand(static_cast<unsigned>(time(0))); // Seed the random generator

	window = nullptr;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

#ifdef _ANDROID_
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#if defined(_ANDROID_)
	SDL_DisplayMode currentDisplayMode;
	int success = SDL_GetCurrentDisplayMode(0, &currentDisplayMode);
	assert(success == 0);
	WIDTH = currentDisplayMode.w;
	HEIGHT = currentDisplayMode.h;
	//gScreenRectangle.w = currentDisplayMode.w;
	//gScreenRectangle.h = currentDisplayMode.h;
#endif

	window = SDL_CreateWindow(gameName.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	assert(window != nullptr);

	SDL_SetHint("SDL_HINT_ORIENTATIONS", "LandscapeRight LandscapeLeft");

	context = SDL_GL_CreateContext(window);
	assert(context != nullptr);

#ifndef _ANDROID_
	SDL_GL_SetSwapInterval(1); // enable vsync
#endif

	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

#ifndef _ANDROID_
	glewExperimental = GL_TRUE;
	glewInit();
#endif
	glGetError();   //Call it once to catch glewInit() bug, all other errors are now form our application

	//OpenGL configuration
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialise the game
	Castle.Init();

	Castle.InitBits();

	Castle.InitCards();

	//setting the game to the initialisation stage
	Castle.setGameState(GAME_INIT);

	Castle.PopulateDeck();

	Castle.shuffleBits_();

	Castle.distributeBits_();

	Castle.Init_Iter_Vec();

	Castle.Sort_Iter_Vec();

	//Castle.Render();

	//start game within menu state
	Castle.setGameState(GAME_INTRO);
	//Castle.setGameState(GAME_ACTIVE);

	// the stopwatch isn't counting at this stage
	Castle.setTimerState(OFF);

	//setting the MoveToPile state
	Castle.setMoveToPile(FINDPILE);

	//Filter for mouse down, mouse move, and mouse up events
	SDL_SetEventFilter(FilteringEvents, NULL);

	// getthe number of clock ticks since the SDL clock was initialised
	unsigned int previous = SDL_GetTicks();
	// double lag = 0.0;


	/*shouldQuit = false;
	unsigned int lastFrame = SDL_GetTicks();
	while (!shouldQuit)
	{
		//Calculate delta time
		unsigned int currentFrame = SDL_GetTicks();
		unsigned int deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				//key_callback(window, event.key.keysym.sym, event.type);
				break;
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				//touch_callback(window, event.tfinger.x, event.tfinger.y, event.type);
				break;
			default:
				break;

			}

		}

		//deltaTime = 0.001f
		//Manage User Input
		const float deltaSeconds = ((float)deltaTime) / 1000.0f;
		//Ricochet.ProcessInput(deltaSeconds);

		//Update Game state
		//Ricochet.Update(deltaSeconds);

		//Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Castle.Render();

		SDL_GL_SwapWindow(window);

	}*/
	
	SDL_Event event;
	shouldQuit = false;	
	while (!shouldQuit)
	{
		// get the number of clock ticks sice the previous frame
		double current = SDL_GetTicks();
		GLfloat elapsed = current - previous;
		//previous = current;
		//lag += elapsed;

		//std::cout << "previous: " << previous << std::endl;
		//std::cout << "current: " << current << std::endl;
		//std::cout << "elapsed: " << elapsed << std::endl;

		

		//Castle.processInput();

		//Castle.Update(elapsed);
		
		/*while (lag >= MS_PER_UPDATE)
		{
			Castle.Update();
			lag -= MS_PER_UPDATE;
		}*/

			
		//Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		while (SDL_PollEvent(&event))
		{
				eventCallback(&event);
		}



		Castle.processInput(&event1, elapsed);

		previous = current;

		//create a touch callback function here - similar to mouseEvent()
       
		SDL_GL_SwapWindow(window);
			
	}

	SDL_Quit();

	

	return 0;
}

//create the Filter events function here
int FilteringEvents(void* unused, SDL_Event* ev)
{
#if !defined(_ANDROID_)

	Castle.Quit(ev);

	if (ev->type == SDL_MOUSEBUTTONDOWN)
	{	
		Castle.HandleMouseDownEvent(ev);
		return 1;
	}
	if (ev->type == SDL_MOUSEMOTION)
	{
		Castle.HandleMouseMoveEvent(ev);
		return 1;
	}
	if (ev->type == SDL_MOUSEBUTTONUP)
	{
		Castle.HandleMouseUpEvent(ev);
		return 1;
	}

#else
	// include preprocessing directives that will only be included if this is running on an Android device

	// within this preprocessing Android block include the finger processing events
	if (ev->type == SDL_FINGERDOWN)
	{
		touchLocation.x = ev->tfinger.x * WIDTH;
		touchLocation.y = ev->tfinger.y * HEIGHT;
		//touchLocation.x = ev->tfinger.x * gScreenRectangle.w;
		//touchLocation.y = ev->tfinger.y * gScreenRectangle.h;
		Castle.HandleFingerTouchDownEvent(ev);
	}
	if (ev->type == SDL_FINGERMOTION)
	{
		touchLocation.x = ev->tfinger.x * WIDTH;
		touchLocation.y = ev->tfinger.y * HEIGHT;
		Castle.HandleFingerMoveEvent(ev);
		return 1;
	}
	if (ev->type == SDL_FINGERUP)
	{
		touchLocation.x = ev->tfinger.x * WIDTH;
		touchLocation.y = ev->tfinger.y * HEIGHT;
		Castle.HandleFingerUpEvent(ev);
	}

#endif
	return 1;
}

// function to get the time from the system
unsigned getCurrentSysTime()
{
	time_t  timev;                   /* time_t is data type
									 defined in ctime or time.h */
	time(&timev);                    /* using the time function,
									 we store the number of seconds
									 from Jan 1 1990 (or any other reference)
									 */
	return timev;                    /* Returns the time */
}  